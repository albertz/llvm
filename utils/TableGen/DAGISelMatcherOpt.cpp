//===- DAGISelMatcherOpt.cpp - Optimize a DAG Matcher ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the DAG Matcher optimizer.
//
//===----------------------------------------------------------------------===//

#include "DAGISelMatcher.h"
#include "llvm/ADT/DenseMap.h"
#include <vector>
using namespace llvm;

static void ContractNodes(OwningPtr<Matcher> &MatcherPtr) {
  // If we reached the end of the chain, we're done.
  Matcher *N = MatcherPtr.get();
  if (N == 0) return;
  
  // If we have a scope node, walk down all of the children.
  if (ScopeMatcher *Scope = dyn_cast<ScopeMatcher>(N)) {
    for (unsigned i = 0, e = Scope->getNumChildren(); i != e; ++i) {
      OwningPtr<Matcher> Child(Scope->takeChild(i));
      ContractNodes(Child);
      Scope->resetChild(i, Child.take());
    }
    return;
  }
  
  // If we found a movechild node with a node that comes in a 'foochild' form,
  // transform it.
  if (MoveChildMatcher *MC = dyn_cast<MoveChildMatcher>(N)) {
    Matcher *New = 0;
    if (RecordMatcher *RM = dyn_cast<RecordMatcher>(MC->getNext()))
      New = new RecordChildMatcher(MC->getChildNo(), RM->getWhatFor());
    
    if (CheckTypeMatcher *CT= dyn_cast<CheckTypeMatcher>(MC->getNext()))
      New = new CheckChildTypeMatcher(MC->getChildNo(), CT->getType());
    
    if (New) {
      // Insert the new node.
      New->setNext(MatcherPtr.take());
      MatcherPtr.reset(New);
      // Remove the old one.
      MC->setNext(MC->getNext()->takeNext());
      return ContractNodes(MatcherPtr);
    }
  }
  
  if (MoveChildMatcher *MC = dyn_cast<MoveChildMatcher>(N))
    if (MoveParentMatcher *MP = 
          dyn_cast<MoveParentMatcher>(MC->getNext())) {
      MatcherPtr.reset(MP->takeNext());
      return ContractNodes(MatcherPtr);
    }
  
  ContractNodes(N->getNextPtr());
}

static void FactorNodes(OwningPtr<Matcher> &MatcherPtr) {
  // If we reached the end of the chain, we're done.
  Matcher *N = MatcherPtr.get();
  if (N == 0) return;
  
  // If this is not a push node, just scan for one.
  ScopeMatcher *Scope = dyn_cast<ScopeMatcher>(N);
  if (Scope == 0)
    return FactorNodes(N->getNextPtr());
  
  // Okay, pull together the children of the scope node into a vector so we can
  // inspect it more easily.  While we're at it, bucket them up by the hash
  // code of their first predicate.
  SmallVector<Matcher*, 32> OptionsToMatch;
  typedef DenseMap<unsigned, std::vector<Matcher*> > HashTableTy;
  HashTableTy MatchersByHash;
  
  for (unsigned i = 0, e = Scope->getNumChildren(); i != e; ++i) {
    // Factor the subexpression.
    OwningPtr<Matcher> Child(Scope->takeChild(i));
    FactorNodes(Child);
    
    if (Matcher *N = Child.take()) {
      OptionsToMatch.push_back(N);
      MatchersByHash[N->getHash()].push_back(N);
    }
  }
  
  SmallVector<Matcher*, 32> NewOptionsToMatch;

  // Now that we have bucketed up things by hash code, iterate over sets of
  // matchers that all start with the same node.  We would like to iterate over
  // the hash table, but it isn't in deterministic order, emulate this by going
  // about this slightly backwards.  After each set of nodes is processed, we
  // remove them from MatchersByHash.
  for (unsigned i = 0, e = OptionsToMatch.size();
       i != e && !MatchersByHash.empty(); ++i) {
    // Find the set of matchers that start with this node.
    Matcher *Optn = OptionsToMatch[i];
    
    // Find all nodes that hash to the same value.  If there is no entry in the
    // hash table, then we must have previously processed a node equal to this
    // one.
    HashTableTy::iterator DMI = MatchersByHash.find(Optn->getHash());
    if (DMI == MatchersByHash.end()) {
      delete Optn;
      continue;
    }

    std::vector<Matcher*> &HashMembers = DMI->second;
    assert(!HashMembers.empty() && "Should be removed if empty");

    // Check to see if this node is in HashMembers, if not it was equal to a
    // previous node and removed.
    std::vector<Matcher*>::iterator MemberSlot =
      std::find(HashMembers.begin(), HashMembers.end(), Optn);
    if (MemberSlot == HashMembers.end()) {
      delete Optn;
      continue;
    }
    
    // If the node *does* exist in HashMembers, then we've confirmed that it
    // hasn't been processed as equal to a previous node.  Process it now, start
    // by removing it from the list of hash-equal nodes.
    HashMembers.erase(MemberSlot);
    
    // Scan all of the hash members looking for ones that are equal, removing
    // them from HashMembers, adding them to EqualMatchers.
    SmallVector<Matcher*, 8> EqualMatchers;
    
    // Scan the vector backwards so we're generally removing from the end to
    // avoid pointless data copying.
    for (unsigned i = HashMembers.size(); i != 0; --i) {
      if (!HashMembers[i-1]->isEqual(Optn)) continue;
      
      EqualMatchers.push_back(HashMembers[i-1]);
      HashMembers.erase(HashMembers.begin()+i-1);  
    }
    EqualMatchers.push_back(Optn);
    
    // Reverse the vector so that we preserve the match ordering.
    std::reverse(EqualMatchers.begin(), EqualMatchers.end());
    
    // If HashMembers is empty at this point, then we've gotten all nodes with
    // the same hash, nuke the entry in the hash table.
    if (HashMembers.empty())
      MatchersByHash.erase(Optn->getHash());
    
    // Okay, we have the list of all matchers that start with the same node as
    // Optn.  If there is more than one in the set, we want to factor them.
    if (EqualMatchers.size() == 1) {
      NewOptionsToMatch.push_back(Optn);
      continue;
    }
    
    // Factor these checks by pulling the first node off each entry and
    // discarding it.  Take the first one off the first entry to reuse.
    Matcher *Shared = Optn;
    Optn = Optn->takeNext();
    EqualMatchers[0] = Optn;

    // Skip the first node.  Leave the first node around though, we'll delete it
    // on subsequent iterations over OptionsToMatch.
    for (unsigned i = 1, e = EqualMatchers.size(); i != e; ++i)
      EqualMatchers[i] = EqualMatchers[i]->takeNext();
    
    Shared->setNext(new ScopeMatcher(&EqualMatchers[0], EqualMatchers.size()));

    // Recursively factor the newly created node.
    FactorNodes(Shared->getNextPtr());
    
    NewOptionsToMatch.push_back(Shared);
  }

  // Reassemble a new Scope node.
  assert(!NewOptionsToMatch.empty() && "where'd all our children go?");
  if (NewOptionsToMatch.size() == 1)
    MatcherPtr.reset(NewOptionsToMatch[0]);
  else {
    Scope->setNumChildren(NewOptionsToMatch.size());
    for (unsigned i = 0, e = NewOptionsToMatch.size(); i != e; ++i)
      Scope->resetChild(i, NewOptionsToMatch[i]);
  }
}

Matcher *llvm::OptimizeMatcher(Matcher *TheMatcher) {
  OwningPtr<Matcher> MatcherPtr(TheMatcher);
  ContractNodes(MatcherPtr);
  FactorNodes(MatcherPtr);
  return MatcherPtr.take();
}