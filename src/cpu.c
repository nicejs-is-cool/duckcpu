// This emulator is 64-BITS ONLY.

// external/standard libs
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// funny defintions
#ifdef DEBUG
#define LOG_DEBUG printf
#define _DUCKCPU_REGISTER_NAMES
#else
static void noop() {}
#define LOG_DEBUG noop
#endif
// internal includes
#include "registers.h"

enum CPU_STATUS_CODES {
	OK,
	INVALID_OPCODE,
	SEGFAULT,
	HALTED
};

struct Memory { // todo: segment memory into pages
	uint64_t memSize;
	uint8_t* ptr;
};

struct Processor {
	struct RegisterFile registers;
	struct Memory memory;
};
void cpu_init(struct Processor* proc, uint64_t memSize) {
	proc->memory.memSize = memSize;
	proc->memory.ptr = malloc(proc->memory.memSize);
	memset(proc->memory.ptr, 0, proc->memory.memSize);
	// the shit i have to do to get this to work...
	// clear the registers
	memset((void*)(&(proc->registers)), 0, sizeof(proc->registers));
}

uint8_t cpu_mem_getbyte(struct Memory* mem, uint64_t addr) {
	if (addr > mem->memSize) { return 0; } // todo: make this segfault
	return mem->ptr[addr];
}
uint64_t cpu_mem_getlong(struct Memory* mem, uint64_t addr) {
	if (addr > (mem->memSize/8)) { return 0; } // todo: make this segfault
	uint64_t a = *((uint64_t*)(mem->ptr+addr));
	return a;
}
uint32_t cpu_mem_getint(struct Memory* mem, uint64_t addr) {
	if (addr > (mem->memSize/4)) { return 0; } // todo: make this segfault
	return *((uint32_t*)(mem->ptr+addr));
}
uint16_t cpu_mem_getshort(struct Memory* mem, uint64_t addr) {
	if (addr > (mem->memSize/2)) { return 0; } // todo: make this segfault
	return *((uint16_t*)(mem->ptr+addr));
}

float cpu_mem_getfloat(struct Memory* mem, uint64_t addr) {
	if (addr > (mem->memSize/sizeof(float))) { return 0; } // todo: make this segfault
	return *((float*)(mem->ptr+addr));
}
double cpu_mem_getdouble(struct Memory* mem, uint64_t addr) {
	if (addr > (mem->memSize/sizeof(double))) { return 0; } // todo: make this segfault
	return *((double*)(mem->ptr+addr));
}
#define CPU_ADV_PC(x) proc->registers.pc += x;

void cpu_instruction_rmo_from_mem(struct Processor* proc, uint64_t caddr, uint8_t op2) {
	uint8_t registerId = cpu_mem_getbyte(&proc->memory, caddr+1);
	uint64_t memoryAddr = cpu_mem_getlong(&proc->memory, caddr+2);
	CPU_ADV_PC(9);
	switch (op2 & 0b0111) {
		case 0b0000: { // long
			LOG_DEBUG("long) ");
			uint64_t memoryContents = cpu_mem_getlong(&proc->memory, memoryAddr);
			
			cpu_reg_set(registerId, &(proc->registers), 
				(void*)(&memoryContents));
			break;
		}
		case 0b0001: { // integer
			LOG_DEBUG("int) ");
			uint32_t memoryContents = cpu_mem_getint(&proc->memory, memoryAddr);
			cpu_reg_set(registerId, &(proc->registers), 
				(void*)(&memoryContents));
			break;
		}
		case 0b0010: { // short
			LOG_DEBUG("short) ");
			uint32_t memoryContents = cpu_mem_getshort(&proc->memory, memoryAddr);
			cpu_reg_set(registerId, &(proc->registers), 
				(void*)(&memoryContents));
			break;
		}
		case 0b0011: { // byte
			LOG_DEBUG("byte) ");
			uint32_t memoryContents = cpu_mem_getbyte(&proc->memory, memoryAddr);
			cpu_reg_set(registerId, &(proc->registers), 
				(void*)(&memoryContents));
			break;
		}
		case 0b0100: { // float
			LOG_DEBUG("float) ");
			uint32_t memoryContents = cpu_mem_getfloat(&proc->memory, memoryAddr);
			cpu_reg_set(registerId, &(proc->registers), 
				(void*)(&memoryContents));
		}
		case 0b0101: { // double
			LOG_DEBUG("double) ");
			uint32_t memoryContents = cpu_mem_getdouble(&proc->memory, memoryAddr);
			cpu_reg_set(registerId, &(proc->registers), 
				(void*)(&memoryContents));
		}
	}
	LOG_DEBUG("%s addr=%p\n", cpu_reg_getname(registerId), memoryAddr);
}
// chat is it bad practice to use macros like this
#define CPU_READ_ARGB(name, offset) uint8_t name = cpu_mem_getbyte(&proc->memory, caddr+offset);
bool cpu_instruction_mov(struct Processor* proc, uint64_t caddr, uint8_t op2) {
	bool shouldCast = false;
	bool shouldCopy = false;
	if ((op2 & 0b1000) != 0) shouldCast = true;
	if ((op2 & 0b0100) != 0) shouldCopy = true;

	uint8_t reg1 = cpu_mem_getbyte(&proc->memory, caddr+1);
	uint8_t reg2 = cpu_mem_getbyte(&proc->memory, caddr+2);
	
	uint64_t reg1v = 0;
	uint64_t reg2v = 0;
	// Getting
	if (!cpu_reg_get(reg1v, reg1, &proc->registers)) {
	return false;
	}
	if (!cpu_reg_get(reg2v, reg2, &proc->registers)) {
		return false;
	}

	if (shouldCopy) {
		if (shouldCast) {
			
			if (reg2 >= REGISTER_FA && reg2 <= REGISTER_LFB) {
				
			} else {
				LOG_DEBUG(OPTIMIZATION_WARNING("mov", "Unnecessary cast at address %p"), caddr);
			}
		}
		if (!cpu_reg_set(reg2, &proc->registers, reg1v)) return false;
		
		return true;
	}
	// Swapping with no casting.
	if (!cpu_reg_set(reg2, &proc->registers, reg1v)) {
		return false;
	}
	if (!cpu_reg_set(reg1, &proc->registers, reg2v)) {
		return false;
	}
	CPU_ADV_PC(2);
	return true;
}
bool cpu_instruction_noa(struct Processor* proc, uint64_t caddr, uint8_t op2) {
	switch (op2) {
		case 0:
			LOG_DEBUG("noa.noop()\n");
			break;
		case 1:
			CPU_READ_ARGB(r1, 1);
			CPU_READ_ARGB(r2, 2);
			LOG_DEBUG("noa.lea(%d, %d)\n", r1, r2);
			CPU_ADV_PC(2);
			break;
		case 2:
			LOG_DEBUG("noa.invalid(%d)\n", op2);
			break;
	}
	return true;
}
enum CPU_STATUS_CODES cpu_step(struct Processor* proc) {
	uint64_t caddr = proc->registers.pc; // current address
	uint8_t opcode = cpu_mem_getbyte(&proc->memory, caddr);
	uint8_t op1 = opcode >> 4; // get the first 4 bits of the opcode
	uint8_t op2 = opcode & 0x0f; // get the last 4 bits of the opcode
	proc->registers.pc++;
	LOG_DEBUG("op1: %d, op2: %d, opc: %d, caddr: %p\n", op1, op2, opcode, caddr);
	switch (op1) { // hopefully gcc optimizes this into a jump table lmao
		case 0: {
			cpu_instruction_noa(proc, caddr, op2);
			break;
		}
		case 1: {
			break; // TODO
		}
		case 2: {
			LOG_DEBUG("rmo(");
			if ((op2 & 0b1000) == 0) {
				// FROM memory
				LOG_DEBUG("from,");
				cpu_instruction_rmo_from_mem(proc, caddr, op2);
			} else {
				// TO memory
				LOG_DEBUG("to,");
			}
			break;
		}
		case 3: {
			LOG_DEBUG("mov(");
			cpu_instruction_mov(proc, caddr, op2);
			break;
		}
		default: {
			return INVALID_OPCODE;
		}
	}
	return OK;
}
