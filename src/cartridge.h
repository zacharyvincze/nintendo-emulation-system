#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <vector>
#include <stdint.h>
#include <string>
#include <fstream>

#include "bitwise.h"
#include "definitions.h"

enum class Mirroring {
	HORIZONTAL, VERTICAL
};

enum class TVSystem {
	NTSC, PAL
};

class Cartridge {
public:
	Cartridge(const std::string filename);
	uint8_t read(unsigned int address);

	u8 getMapperNumber();
private:
	std::vector<uint8_t> data;
	unsigned int prg_rom_size;
	unsigned int chr_rom_size;
	unsigned int prg_ram_size;

	// Flags 6
	Mirroring mirroring;
	bool battery_backed;
	bool trainer_present;
	bool ignore_mirroring_control;

	// Flags 7
	bool vs_unisystem;
	bool playchoice_10;

	// Flags 9
	TVSystem tv_system;

	uint8_t mapper_number;
};

#endif // CARTRIDGE_H