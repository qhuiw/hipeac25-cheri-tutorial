1. Compile the program for the hybrid ABI

2. Compile the program for CheriABI

   Save the compiler output to analyse it later.

3. Run the cat program for both ABIs with an arbitrary file, e.g.:

   ./cat /etc/resolv.conf

4. Analyse the ambiguous provenance issue reported by the compiler and
   try to fix it

   * The write(2) system call expects a valid pointer in its second argument

   * Run the program with CHERI GDB and a breakpoint set for the write symbol

   * What is the value of the buf argument and why?

   * After fixing the issue, repeat the CHERI GDB session to see what value
     the second write(2) system call argument has now

5. Run the cat program in the verbose mode (`-n`) for both ABIs with
   an arbitrary file, e.g.:

   ./cat -n /etc/resolv.conf

6. Analyse the loss of provenance issue repoted by the compiler and
   try to fix it

   * The getc() is a macro that expects a valid FILE pointer

     The macro accesses fields of a FILE object.

   * Run the program with CHERI GDB to repeat the crash

   * What is the value of the fp argument and why?
