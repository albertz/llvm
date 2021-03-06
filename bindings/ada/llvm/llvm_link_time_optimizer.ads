-- This file is generated by SWIG. Do *not* modify by hand.
--

with Interfaces.C.Extensions;


package LLVM_link_time_Optimizer is

   -- lto_symbol_attributes
   --
   type lto_symbol_attributes is (
      LTO_SYMBOL_ALIGNMENT_MASK,
      LTO_SYMBOL_PERMISSIONS_RODATA,
      LTO_SYMBOL_PERMISSIONS_CODE,
      LTO_SYMBOL_PERMISSIONS_DATA,
      LTO_SYMBOL_PERMISSIONS_MASK,
      LTO_SYMBOL_DEFINITION_REGULAR,
      LTO_SYMBOL_DEFINITION_TENTATIVE,
      LTO_SYMBOL_DEFINITION_WEAK,
      LTO_SYMBOL_DEFINITION_UNDEFINED,
      LTO_SYMBOL_DEFINITION_WEAKUNDEF,
      LTO_SYMBOL_DEFINITION_MASK,
      LTO_SYMBOL_SCOPE_INTERNAL,
      LTO_SYMBOL_SCOPE_HIDDEN,
      LTO_SYMBOL_SCOPE_DEFAULT,
      LTO_SYMBOL_SCOPE_PROTECTED,
      LTO_SYMBOL_SCOPE_MASK);

   for lto_symbol_attributes use
     (LTO_SYMBOL_ALIGNMENT_MASK       => 31,
      LTO_SYMBOL_PERMISSIONS_RODATA   => 128,
      LTO_SYMBOL_PERMISSIONS_CODE     => 160,
      LTO_SYMBOL_PERMISSIONS_DATA     => 192,
      LTO_SYMBOL_PERMISSIONS_MASK     => 224,
      LTO_SYMBOL_DEFINITION_REGULAR   => 256,
      LTO_SYMBOL_DEFINITION_TENTATIVE => 512,
      LTO_SYMBOL_DEFINITION_WEAK      => 768,
      LTO_SYMBOL_DEFINITION_UNDEFINED => 1024,
      LTO_SYMBOL_DEFINITION_WEAKUNDEF => 1280,
      LTO_SYMBOL_DEFINITION_MASK      => 1792,
      LTO_SYMBOL_SCOPE_INTERNAL       => 2048,
      LTO_SYMBOL_SCOPE_HIDDEN         => 4096,
      LTO_SYMBOL_SCOPE_DEFAULT        => 6144,
      LTO_SYMBOL_SCOPE_PROTECTED      => 8192,
      LTO_SYMBOL_SCOPE_MASK           => 14336);

   pragma Convention (C, lto_symbol_attributes);

   type lto_symbol_attributes_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.lto_symbol_attributes;

   type lto_symbol_attributes_view is access all
     LLVM_link_time_Optimizer.lto_symbol_attributes;

   -- lto_debug_model
   --
   type lto_debug_model is (LTO_DEBUG_MODEL_NONE, LTO_DEBUG_MODEL_DWARF);

   for lto_debug_model use
     (LTO_DEBUG_MODEL_NONE  => 0,
      LTO_DEBUG_MODEL_DWARF => 1);

   pragma Convention (C, lto_debug_model);

   type lto_debug_model_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.lto_debug_model;

   type lto_debug_model_view is access all
     LLVM_link_time_Optimizer.lto_debug_model;

   -- lto_codegen_model
   --
   type lto_codegen_model is (
      LTO_CODEGEN_PIC_MODEL_STATIC,
      LTO_CODEGEN_PIC_MODEL_DYNAMIC,
      LTO_CODEGEN_PIC_MODEL_DYNAMIC_NO_PIC);

   for lto_codegen_model use
     (LTO_CODEGEN_PIC_MODEL_STATIC         => 0,
      LTO_CODEGEN_PIC_MODEL_DYNAMIC        => 1,
      LTO_CODEGEN_PIC_MODEL_DYNAMIC_NO_PIC => 2);

   pragma Convention (C, lto_codegen_model);

   type lto_codegen_model_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.lto_codegen_model;

   type lto_codegen_model_view is access all
     LLVM_link_time_Optimizer.lto_codegen_model;

   -- LTOModule
   --
   type LTOModule is new Interfaces.C.Extensions.opaque_structure_def;

   type LTOModule_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.LTOModule;

   type LTOModule_view is access all LLVM_link_time_Optimizer.LTOModule;

   -- lto_module_t
   --
   type lto_module_t is access all LLVM_link_time_Optimizer.LTOModule;

   type lto_module_t_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.lto_module_t;

   type lto_module_t_view is access all LLVM_link_time_Optimizer.lto_module_t;

   -- LTOCodeGenerator
   --
   type LTOCodeGenerator is new Interfaces.C.Extensions.opaque_structure_def;

   type LTOCodeGenerator_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.LTOCodeGenerator;

   type LTOCodeGenerator_view is access all
     LLVM_link_time_Optimizer.LTOCodeGenerator;

   -- lto_code_gen_t
   --
   type lto_code_gen_t is access all LLVM_link_time_Optimizer.LTOCodeGenerator;

   type lto_code_gen_t_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.lto_code_gen_t;

   type lto_code_gen_t_view is access all
     LLVM_link_time_Optimizer.lto_code_gen_t;

   -- llvm_lto_status_t
   --
   type llvm_lto_status_t is (
      LLVM_LTO_UNKNOWN,
      LLVM_LTO_OPT_SUCCESS,
      LLVM_LTO_READ_SUCCESS,
      LLVM_LTO_READ_FAILURE,
      LLVM_LTO_WRITE_FAILURE,
      LLVM_LTO_NO_TARGET,
      LLVM_LTO_NO_WORK,
      LLVM_LTO_MODULE_MERGE_FAILURE,
      LLVM_LTO_ASM_FAILURE,
      LLVM_LTO_NULL_OBJECT);

   for llvm_lto_status_t use
     (LLVM_LTO_UNKNOWN              => 0,
      LLVM_LTO_OPT_SUCCESS          => 1,
      LLVM_LTO_READ_SUCCESS         => 2,
      LLVM_LTO_READ_FAILURE         => 3,
      LLVM_LTO_WRITE_FAILURE        => 4,
      LLVM_LTO_NO_TARGET            => 5,
      LLVM_LTO_NO_WORK              => 6,
      LLVM_LTO_MODULE_MERGE_FAILURE => 7,
      LLVM_LTO_ASM_FAILURE          => 8,
      LLVM_LTO_NULL_OBJECT          => 9);

   pragma Convention (C, llvm_lto_status_t);

   type llvm_lto_status_t_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.llvm_lto_status_t;

   type llvm_lto_status_t_view is access all
     LLVM_link_time_Optimizer.llvm_lto_status_t;


   -- llvm_lto_t
   --
   type llvm_lto_t is access all Interfaces.C.Extensions.void;

   type llvm_lto_t_array is
     array (Interfaces.C.size_t range <>)
            of aliased LLVM_link_time_Optimizer.llvm_lto_t;

   type llvm_lto_t_view is access all
     LLVM_link_time_Optimizer.llvm_lto_t;


end LLVM_link_time_Optimizer;
