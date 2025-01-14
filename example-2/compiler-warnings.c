#include <sys/types.h>
#include <stdio.h>

void
loss_of_provenance(long x){
	*(char *)x = 'A';
}

void
ambiguous_provenance(uintptr_t ptr1, uintptr_t ptr2)
{
	char *newptr;

	newptr = (void *)((ptr1 & 0x3) | ptr2);
	*(char *)((uintptr_t)newptr & ~0x3) = 'A';
}

void
underaligned_capability(char *ptr)
{
	struct __attribute__((packed)) {
		char x;
		char *data;
	} obj;

	obj.data = ptr;
	*obj.data = 'A';
}

int
main(void)
{
	char a;

	printf("Running loss_of_provenance():\n");
	loss_of_provenance((long)&a);
	printf("done.\n");

	printf("Running ambiguous_provenance():\n");
	ambiguous_provenance(0x1, (uintptr_t)&a);
	printf("done.\n");

	printf("Running underaligned_capability():\n");
	underaligned_capability(&a);
	printf("done.\n");

	return (0);
}
