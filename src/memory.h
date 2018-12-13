#ifndef MEMORY_H
#define MEMORY_H

#include "definitions.h"
#include "cartridge.h"

/*
Memory class to map all read and writes to memory to proper emulated
locations.
*/

class Memory {
public:
	Memory(Cartridge& cartridge);

	u8 readByte(u16 address);
	void writeByte(u8 byte, u16 address);

private:
	Cartridge& cartridge;
	u8 data[0x10000];
};

#endif // MEMORY_H