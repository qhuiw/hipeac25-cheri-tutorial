# Answers

## Introducing heap-allocator bounds

2. GDB will show a CHERI tag violation resulting from `memset()` overwriting
   the `a_next` field in the second allocation entry, which is tripped over by
   a later call to `alloc_allocate()`:

```
Starting program: /root/cheri-allocator
Allocator initialised
Allocating memory
Allocation returned 0x104550
Preparing to overflow 0x104550
Overflowed allocation 0x104550
Freeing allocation 0x104550
Allocation 0x104550 freed
Allocating memory
Allocation returned 0x104550
Allocating memory
Allocation returned 0x1045e0
Allocating memory

Program received signal SIGPROT, CHERI protection violation.
Capability tag fault caused by register c0.
alloc_allocate () at cheri-allocator.c:83
83              alloc_nextfree = alloc->a_next;
(gdb) p alloc
$1 = (struct alloc_storage *) 0x4141414141414141 [w,0x41414141414141-0x41414141414141] (invalid,sealed)
```

3. When compiling for CHERI C, use `cheri_bounds_set()` to set bounds on the
   returned pointer:

```
        /* Return pointer to allocated memory. */
#ifdef __CHERI_PURE_CAPABILITY__
        return (cheri_bounds_set(alloc->a_bytes, ALLOC_SIZE));
#else
        return (alloc->a_bytes);
#endif
```

4. With this change, the `memset()` call in `main()` triggers a bounds
   violation exception on overflow:

```
Starting program: /root/cheri-allocator
Allocator initialised
Allocating memory
Allocation returned 0x104550
Preparing to overflow 0x104550

Program received signal SIGPROT, CHERI protection violation.
Capability bounds fault.
main () at cheri-allocator.c:131
131             memset(ptr1 + ALLOC_SIZE, 'A', sizeof(void *));
```

## Reaching allocator metadata

6. Following this change, `alloc_free()` crashes with a bounds violation,
   due to reaching outside the bounds of the passed memory allocation:

```
Starting program: /root/cheri-allocator
Allocator initialised
Allocating memory
Allocation returned 0x104420
Freeing allocation 0x104420

Program received signal SIGPROT, CHERI protection violation.
Capability bounds fault.
0x0000000000110e2c in alloc_free (ptr=0x131230 <alloc_array+16> [rwRW,0x131230-0x1312b0]) at cheri-allocator.c:106
106             alloc->a_next = alloc_nextfree;
(gdb) bt
0x0000000000110ae0 in alloc_free (ptr=0x1310b0 <alloc_array+16> [rwRW,0x1310b0-0x131130]) at cheri-allocator.c:106
106             alloc->a_next = alloc_nextfree;
(gdb) p alloc
$1 = (struct alloc_storage *) 0x131220 <alloc_array> [rwRW,0x131230-0x1312b0]
```

7. We need to create a new capability, derived from `alloc_array` but with the
   address generated from pointer to the memory being freed.
   One way to do this is using the `cheri_address_get()` and
   `cheri_address_set()`, reading the address from one capability and setting
   it on the other:

```
#ifdef __CHERI_PURE_CAPABILITY__
        /*
         * Generate a new pointer to the allocation that is derived from the
         * one passed by the consumer.
         */
        ptr = cheri_address_set(alloc_array, cheri_address_get(ptr));
#endif
```

   Note that this is not a complete solution to providing spatial safety here:
   software could still accidentally pass an out-of-bounds pointer.
