# Example: use after free

This example demonstrates how CHERI revocation defeats use-after-reallocation
vulnerabilities.

1. Run `make` to compile `use-after-free.c` for the baseline aarch64 and
   CHERI Morello architectures. This generates the
   `use-after-free-baseline` and `use-after-free-cheri` binaries.

2. Run both versions. Observe that the CHERI version crashes with "In-address
   space security exception"

3. Run the CHERI version under `gdb` and examine the crash in more detail.
   Observe that the `cp` capability is invalid at the time of access.

## Source Files

**use-after-free.c**
```C
{{#include use-after-free.c}}
```
