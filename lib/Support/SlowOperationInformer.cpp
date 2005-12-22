//===-- SlowOperationInformer.cpp - Keep the user informed ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the SlowOperationInformer class for the LLVM debugger.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/SlowOperationInformer.h"
#include "llvm/System/Alarm.h"
#include <iostream>
#include <sstream>
#include <cassert>
using namespace llvm;

SlowOperationInformer::SlowOperationInformer(const std::string &Name)
  : OperationName(Name), LastPrintAmount(0) {
  sys::SetupAlarm(1);
}

SlowOperationInformer::~SlowOperationInformer() {
  sys::TerminateAlarm();
  if (LastPrintAmount) {
    // If we have printed something, make _sure_ we print the 100% amount, and
    // also print a newline.
    std::cout << std::string(LastPrintAmount, '\b') << "Progress "
              << OperationName << ": 100%  \n";
  }
}

/// progress - Clients should periodically call this method when they are in
/// an exception-safe state.  The Amount variable should indicate how far
/// along the operation is, given in 1/10ths of a percent (in other words,
/// Amount should range from 0 to 1000).
void SlowOperationInformer::progress(unsigned Amount) {
  int status = sys::AlarmStatus();
  if (status == -1) {
    std::cout << "\n";
    LastPrintAmount = 0;
    throw "While " + OperationName + ", operation cancelled.";
  }

  // If we haven't spent enough time in this operation to warrant displaying the
  // progress bar, don't do so yet.
  if (status == 0)
    return;

  // Delete whatever we printed last time.
  std::string ToPrint = std::string(LastPrintAmount, '\b');

  std::ostringstream OS;
  OS << "Progress " << OperationName << ": " << Amount/10;
  if (unsigned Rem = Amount % 10)
    OS << "." << Rem << "%";
  else
    OS << "%  ";

  LastPrintAmount = OS.str().size();
  std::cout << ToPrint+OS.str() << std::flush;
}
