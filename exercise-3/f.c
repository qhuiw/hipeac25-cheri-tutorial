#include <stdio.h>

void f() {
	char *p, c;
	size_t off;

	asm ("mov	%0, csp" : "=C" (p));
	printf("sp = %#p\n", p);

	printf("Enter an offset: ");
	scanf("%zu", &off);

	printf("Enter a character: ");
	scanf(" %c", &c);

	p[off] = c;
}
