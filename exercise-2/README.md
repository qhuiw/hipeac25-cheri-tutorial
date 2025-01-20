# Exercise: Adapt a C Program to CHERI C

This exercise presents an example C program that includes capability-related
issues that might appear as bugs in software initially developed for non-CHERI
architectures. The example C program is `cat(1)` from CheriBSD (and hence
FreeBSD) modified to introduce the issues that we want to investigate.

1. Compile the program for CheriABI and the hybrid ABI, and save the compiler
   output to analyse it later

   ```
   make
   ```

3. Run the cat program for both ABIs with an arbitrary file, e.g.:

   ```
   ./cat-baseline /etc/resolv.conf
   ```

   ```
   ./cat-cheri /etc/resolv.conf
   ```

4. Analyse the ambiguous provenance issue reported by the compiler and
   try to fix it

   * The `write(2)` system call expects a valid pointer in its second argument

   * Run the program with CHERI GDB and a breakpoint set for the `write` symbol.
     Once CHERI GDB hits the breakpoint, you can use the `backtrace` GDB
     command to see what function calls `write(2)`.
     You can do this by entering GDB: `gdb ./cat-cheri`
     Then once inside set the breakpoint using: `break methods.c:70`
     Finally run our vulnerable code: `run /etc/resolv.conf`

   * What is the value of the `buf` argument and why?

   * After fixing the issue, repeat the CHERI GDB session to see what value
     the second `write(2)` system call argument has now

5. Run the cat program in the verbose mode (`-n`) for both ABIs with
   an arbitrary file, e.g.:

   ```
   ./cat-baseline -n /etc/resolv.conf
   ```

   ```
   ./cat-cheri -n /etc/resolv.conf
   ```

6. Analyse the loss of provenance issue reported by the compiler and try to fix
   it

   * The `getc()` is a macro that expects a valid `FILE` pointer

     The macro accesses fields of a `FILE` object.

   * Run the program with GDB to repeat the crash. Again, use the `backtrace`
     GDB command to see what is the crash context

   * What is the value of the `fp` argument and why?
