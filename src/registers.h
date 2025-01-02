#include <inttypes.h>
#include "optional_features_helper.h"
#ifdef _DUCKCPU_REGISTER_NAMES
// reason its optional is because not everything that uses this emulator is gonna need this extra code
// heck the only thing thats gonna use this in the emulator is only included if -DDEBUG is on

// le lookup table
static const char* DUCKCPU_REGISTER_NAMES[] = {
	"nul",
	"acc",
	"la",
	"lb",
	"fa",
	"fb",
	"lfa",
	"lfb",
	"pc",
	"sf",
	"sp"
};
const char* cpu_reg_getname(uint8_t id) { // this could probably be inlined lol
	if (id > sizeof(DUCKCPU_REGISTER_NAMES)/sizeof(char*)) return "invalid";
	return DUCKCPU_REGISTER_NAMES[id];
}
#else
// https://stackoverflow.com/questions/10049981/why-does-gcc-keep-empty-functions
const char* cpu_reg_getname(); // just so it works with -O0
inline const char* cpu_reg_getname() NOT_IMPLEMENTED("cpu_reg_getname");
#endif

struct RegisterFile {
	char nul;
	uint64_t acc;
	uint64_t la;
	uint64_t lb;
	float fa;
	float fb;
	double lfa;
	double lfb;
	uint64_t pc;
	uint64_t sf;
	uint64_t sp;
};

static enum Register {
	REGISTER_NUL,
	REGISTER_ACC,
	REGISTER_LA,
	REGISTER_LB,
	REGISTER_FA,
	REGISTER_FB,
	REGISTER_LFA,
	REGISTER_LFB,
	REGISTER_PC,
	REGISTER_SF,
	REGISTER_SP
};

// Will cut the lower 32-bits when writing to a 32-bit register
bool cpu_reg_set(uint8_t id, struct RegisterFile* rf, void* value) {
	switch (id) { // there's gotta be a better to do this man
		case 0: { // nul
			return true;
		}
		case 1: { // acc
			rf->acc = *((uint64_t*)value);
			return true;
		}
		case 2: { // la
			rf->la = *((uint64_t*)value);
			return true;
		}
		case 3: { // lb
			rf->lb = *((uint64_t*)value);
			return true;
		}
		case 4: { // fa
			rf->fa = *((float*)value);
			return true;
		}
		case 5: { // fb
			rf->fb = *((float*)value);
			return true;
		}
		case 6: { // lfa
			rf->lfa = *((double*)value);
			return true;
		}
		case 7: { // lfb
			rf->lfb = *((double*)value);
			return true;
		}
		case 8: { // pc
			rf->pc = *((uint64_t*)value);
			return true;
		}
		case 9: { // sf
			rf->sf = *((uint64_t*)value);
			return true;
		}
		case 10: { // sp
			rf->sp = *((uint64_t*)value);
			return true;
		}
	}
	return false;
}

bool cpu_reg_get(void* output, uint8_t id, struct RegisterFile* rf) {
	switch (id) { // there's gotta be a better to do this man
		case 0: { // nul
			*(uint64_t*)output = 0;
			return true;
		}
		case 1: { // acc
			*(uint64_t*)output = rf->acc;
			return true;
		}
		case 2: { // la
			*(uint64_t*)output = rf->la;
			return true;
		}
		case 3: { // lb
			*(uint64_t*)output = rf->lb;
			return true;
		}
		case 4: { // fa
			*(float*)output = rf->fa;
			return true;
		}
		case 5: { // fb
			*(float*)output = rf->fb;
			return true;
		}
		case 6: { // lfa
			*(double*)output = rf->lfa;
			return true;
		}
		case 7: { // lfb
			*(uint64_t*)output = rf->lfb;
			return true;
		}
		case 8: { // pc
			*(uint64_t*)output = rf->pc;
			return true;
		}
		case 9: { // sf
			*(uint64_t*)output = rf->sf;
			return true;
		}
		case 10: { // sp
			*(uint64_t*)output = rf->sp;
			return true;
		}
	}
	return false;
}

void cpu_reg_cast(uint8_t registerIdX, uint8_t registerIdY, void* output) {
	
}
