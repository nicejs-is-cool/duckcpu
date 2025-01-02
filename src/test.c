#include <stdio.h>
#include <inttypes.h>

#define PAGE_SIZE 4096

struct Memory {
	uint64_t pageCount;
	char (*page)[PAGE_SIZE];
};

int main() {

	printf("%d", sizeof(struct Memory));
}