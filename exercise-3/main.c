#include <stdio.h>
#include <string.h>

void f();

int main() {
	char buf[] = "Hello, world!";
	printf("%#p\n", buf);
	puts(buf);
	f();
	puts(buf);
	return 0;
}
