Example: CHERI C/C++ porting issues

This example demonstrates 3 example issues you might encounter when porting
C/C++ software to CHERI C/C++ due to its semantics differences:

* Loss of capability provenance due to an invalid data type usage

* Ambiguous capability provenance due to an invalid data type of a binary
  operator's operand

* Underligned capability access due to an incorrect pointer alignment

Take the following steps to run this example:

1. Compile the `compiler-warnings` program and read compiler warnings:

   ```
   make
   ```

2. Run the baseline program variant:

   ```
   ./compiler-warnings-baseline
   ```

   A C/C++ program incompatible with CHERI C/C++ does not cause any crashes on
   non-CHERI architectures or when compiled for a baseline ABI on
   a CHERI-extended architecture.

3. Run the cheri program variant:

   ```
   ./compiler-warnings-cheri
   ```

   When compiled for the pure-capability ABI, the program crashes due to
   incorrect capability manipulations that are disallowed by CHERI C/C++.

4. Debug the first crash:

   * Enter the debugger:

     ```
     gdb ./compiler-warnings-cheri
     ```

   *  Run the program in GDB:

      ```
      run
      ```

   * Print the contents of `x`:

      ```
      print x
      ```

      The `x` variable is not a correct CHERI capability as it only includes
      an integer part with a pointer address.

   * Disassemble the failing instruction:

     ```
     disassemble $pcc,+4
     ```

     It is a byte store instruction using a capability with an immediate
     offset.

   * Check the contents of the instruction's operands:

     ```
     info register <a register name>
     ```

     The capability operand used in the store is invalid.

   * Exit GDB:

     ```
     quit
     ```

5. Fix the first crash:

   * Memory accesses in the pure-capability ABI must use valid capabilities.
     The `long` data type is not a data type that can hold a capability in CHERI
     C/C++. A developer must use a pointer type, `uintptr_t` or `intptr_t` if a
     variable is supposed to hold an integer or a pointer

   * Change the data type of the `x` argument in the `loss_of_provenance()`
     function to `uintptr_t`

   * Change the cast in the `loss_of_provenance()` function call to `uintptr_t`

   * Recompile the program

     ```
     make
     ```

   * Run the cheri program variant again to observe the crash is gone:

     ```
     ./compile-warnings-cheri
     ```

6. Debug the second crash:

   * Enter the debugger:

     ```
     gdb ./compiler-warnings-cheri
     ```

   *  Run the program in GDB:

      ```
      run
      ```

   * Print the contents of `newptr`:

      ```
      print newptr
      ```

      The `newptr` variable is not a correct CHERI capability as it only
      includes an integer part with a pointer address.

   * Disassemble the failing instruction:

     ```
     disassemble $pcc,+4
     ```

     It is a byte store instruction using a capability.

   * Check the contents of the instruction's operands:

     ```
     info register <a register name>
     ```

     The capability operand used in the store is invalid.

   * Exit GDB:

     ```
     quit
     ```

7. Fix the second crash:

   * A capability is always created by deriving a new value from a previously
     created source capability. In case of a binary operator, the compiler does
     not know which capability operand to use for the source capability if both
     operands are capabilities. By default, the compiler uses the left-hand side
     capability-operator and emits a warning about that

   * In this case, the right-hand side capability-operand of the binary `|`
     operator should be used for the source capability. Cast the left-hand side
     to an integer data type, e.g. to `size_t`:

     ```
     newptr = (void *)((size_t)(ptr1 & 0x3) | ptr2);
     ```

   * Recompile the program

     ```
     make
     ```

   * Run the cheri program variant again to observe the crash is gone:

     ```
     ./compile-warnings-cheri
     ```

8. Debug the third crash:

   * Enter the debugger:

     ```
     gdb ./compiler-warnings-cheri
     ```

   *  Run the program in GDB:

      ```
      run
      ```

   * Print the contents of `obj.data`:

      ```
      print obj.data
      ```

      The `data` field is not a correct CHERI capability as it does not have
      a valid tag.

   * Print the address of `obj.data`:

     ```
     print &obj.data
     ```

     The `data` field is not aligned to the capability size (16 bytes on Arm
     Morello). When storing the capability in `obj.data`, the capability
     contents were correctly stored but the capability tag was not set.

   * Disassemble the failing instruction:

     ```
     disassemble $pcc,+4
     ```

     It is a byte store instruction using a capability.

   * Check the contents of the instruction's operands:

     ```
     info register <a register name>
     ```

     The capability operand used in the store is invalid.

   * Exit GDB:

     ```
     quit
     ```

8. Fix the third crash:

   * A capability must be aligned to the pointer size (16 bytes on Arm Morello)
     in CHERI C/C++. Each capability-aligned address has a corresponding tag in
     a non-addressable address space. A misaligned capability does not have a
     tag by definition and hence is invalid

   * Change the code to make the `data` field aligned to the pointer size.
     There are several ways to achieve that:

     * Remove the `packed` attribute, if possible

     * Align the `data` field using the attribute:

       ```
       char *data __attribute__((aligned(16)));
       ```

     * Change the structure contents to match the alignment:

       ```
       struct __attribute__((packed)) {
           char x[16];
           char *data;
       } obj;
       ```

   * Recompile the program

     ```
     make
     ```

     Note that the compiler will still print warnigns in some of above cases and
     will inform how to silent the warnings if the developer is sure what is
     doing.

   * Run the cheri program variant again to observe the crash is gone:

     ```
     ./compile-warnings-cheri
     ```
