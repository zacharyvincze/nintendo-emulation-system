#include <stdio.h>

#include "cartridge.h"
#include "memory.h"
#include "cpu.h"
#include "nes.h"

int main(int argc, char **argv) {
	if (argc != 2)  {
		printf("Usage: nes <rom>\n");
		return -1;
	}

	// Initialize all NES components
	Cartridge cartridge = Cartridge(argv[1]);
	Memory memory = Memory(cartridge);
	CPU cpu = CPU(memory);
	NES nes = NES(cpu, memory);

	return 0;
}