#include "cpu.c"
#include <string.h>

static const uint8_t program[16] = {0b00100000, 2, 0x45, 0, 0, 0, 0, 0, 0, 0, 0b00000001, 69, 42, 0, 0, 0};

int main() {
	struct Processor* proc = malloc(sizeof(struct Processor));
	//printf("int=%d, float=%d, double=%d, long=%d, long long=%d\n", sizeof(int), sizeof(float), sizeof(double), sizeof(long), sizeof(long long));
	cpu_init(proc, 2048); // 2K of memory for testing
	// copy program to memory
	memcpy(proc->memory.ptr, program, 16);
	proc->memory.ptr[69] = 0xff; // le test data
	proc->memory.ptr[70] = 0xff; // le test data
	proc->memory.ptr[71] = 0xff; // le test data
	proc->memory.ptr[72] = 0xff; // le test data
	proc->memory.ptr[73] = 0xff; // le test data
	proc->memory.ptr[74] = 0xff; // le test data
	proc->memory.ptr[75] = 0xff; // le test data
	proc->memory.ptr[76] = 0xff; // le test data
	
	printf("value: %llu\n", proc->registers.la);
	cpu_step(proc);
	printf("value: %llu\n", proc->registers.la);
	cpu_step(proc);
	return 0;
}
