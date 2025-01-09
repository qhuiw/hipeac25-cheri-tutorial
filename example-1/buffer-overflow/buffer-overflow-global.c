/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <assert.h>
#include <stdio.h>

/*
 * Definition of ptraddr_t.
 * Used to indicate a virtual address without provenance information,
 * can't be deferenced.
 */
#include <stddef.h>

#ifdef __CHERI_PURE_CAPABILITY__
/*
 * CHERI compiler builtins for direct capability manipulation.
 * E.g. cheri_address_get(void *p), cheri_length_get(void *p),
 * cheri_tag_get(void *p), cheri_bounds_set(void *p, size_t length).
 */
#include <cheriintrin.h>
#endif

#ifndef nitems
#define	nitems(x)	(sizeof((x)) / sizeof((x)[0]))
#endif

char buffer[128];
char c;

#pragma weak fill_buf
void
fill_buf(char *buf, size_t len)
{
	for (size_t i = 0; i <= len; i++)
		buf[i] = 'b';
}

int
main(void)
{
#ifdef __CHERI_PURE_CAPABILITY__
	/* Ensure that `buffer` does not require padding. */
	assert(sizeof(buffer) == cheri_representable_length(sizeof(buffer)));
#endif
	/* Ensure that overflowing `buffer` by 1 will hit `c`. */
	assert((ptraddr_t)&buffer[nitems(buffer)] == (ptraddr_t)&c);

	c = 'c';
	printf("c = %c\n", c);

	/* Will cause a CHERI exception when overflowing */
	fill_buf(buffer, sizeof(buffer));

	printf("c = %c\n", c);

	return 0;
}
