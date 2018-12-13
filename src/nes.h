#ifndef NES_H
#define NES_H

#include "cpu.h"
#include "memory.h"

class NES {
public:
	NES(CPU& cpu, Memory& memory);
private:
	CPU& cpu;
	Memory& memory;
};

#endif // NES_H