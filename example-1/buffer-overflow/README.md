# Example: buffer overflow

This example demonstrates inter-object *globals* buffer overflows on baseline and
CHERI-enabled ARM Morello architecture.

1. Run `make` to compile `buffer-overflow-global.c` for the baseline aarch64 and
   CHERI Morello architectures. This generates the
   `buffer-overflow-global-baseline` and `buffer-overflow-cheri` binaries.

2. Run both versions. Observe that the CHERI version crashes with "In-address
   space security exception".

3. Run the CHERI version under `gdb` and examine the crash
   in more detail. Observe that the failing instruction attempts to write
   past the length of the `buffer` capability.

## Source Files

**buffer-overflow-global.c**
```C
{{#include buffer-overflow-global.c}}
```
