/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2024 Konrad Witaszczyk
 */
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

char *cp;

int
main(int argc, char *argv[])
{

	cp = malloc(sizeof(*cp));
	if (cp == NULL)
		err(1, "malloc");
	fprintf(stderr, "Pointer after allocation %#p\n", cp);
	*cp = 'c';			/* Allocated. */
	free(cp);
	*cp = 'c';			/* Freed, but in quarantine. */
	malloc_revoke();
	fprintf(stderr, "Pointer after revocation %#p\n", cp);
	printf("%c\n", *cp);		/* Revoked. */
	exit(1);
}
