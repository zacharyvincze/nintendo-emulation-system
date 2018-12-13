#include "cartridge.h"

namespace {
	const uint16_t PGR_ROM_CODE = 0x4;
	const uint16_t CHR_ROM_CODE = 0x5;
	const uint16_t PRG_RAM_CODE = 0x8;
	const uint16_t FLAGS_6_CODE = 0x6;
	const uint16_t FLAGS_7_CODE = 0x7;
	const uint16_t FLAGS_9_CODE	= 0x9;

	const uint8_t HEADER_SIZE	= 0x10;
}

Cartridge::Cartridge(const std::string filename) {
	printf("\n+----------------------+\n");
	printf("|READING CARTRIDGE DATA|\n");
	printf("+----------------------+\n\n");

	// Attempt to read from filename into data vector
	std::ifstream rom_file;
	rom_file.open(filename.c_str(), std::ios::binary);
	if (!rom_file.is_open()) {
		printf("ERROR: Unable to open %s!\n", filename.c_str());
		exit(-1);
	}

	rom_file.seekg(0, std::ios::end);
	size_t end = rom_file.tellg();
	rom_file.seekg(0, std::ios::beg);
	size_t begin = rom_file.tellg();
	size_t file_size = end-begin;

	printf("ROM FILE SIZE: %lu bytes\n", file_size);

	data.resize(file_size);
	rom_file.read(reinterpret_cast<char*>(data.data()), file_size);

	// Read header and set data accordingly
	prg_rom_size = data[PGR_ROM_CODE] * 16;
	chr_rom_size = data[CHR_ROM_CODE] * 8;
	prg_ram_size = data[PRG_RAM_CODE] * 8;
	if (prg_ram_size == 0) prg_ram_size = 8;	// Compatibility

	// Flags 6
	if (bitwise::check_bit(data[FLAGS_6_CODE], 0)) mirroring = Mirroring::VERTICAL;
	battery_backed = bitwise::check_bit(data[FLAGS_6_CODE], 1);
	trainer_present = bitwise::check_bit(data[FLAGS_6_CODE], 2);
	ignore_mirroring_control = bitwise::check_bit(data[FLAGS_6_CODE], 3);
	uint8_t lower_mapper_number = (data[FLAGS_6_CODE] >> 4) & (0xF);

	// Flags 7
	uint8_t upper_mapper_number = (data[FLAGS_7_CODE] >> 4) & (0xF);

	// Flags 9
	if (bitwise::check_bit(data[FLAGS_9_CODE], 0)) tv_system = TVSystem::PAL;

	mapper_number = bitwise::combine_nibbles(upper_mapper_number, lower_mapper_number);

	printf("PRG ROM SIZE: %iKB\n", prg_rom_size);
	printf("CHR ROM SIZE: %iKB\n", chr_rom_size);
	printf("PRG RAM SIZE: %iKB\n", prg_ram_size);
	printf("MAPPER NUMBER: %i\n", mapper_number);
}

uint8_t Cartridge::read(unsigned int address) {
	// Mapper 0 16KB (First bank is mapped to 0x8000-0xBFFF and mirrored to 0xC000-0xFFFF)
	if (0x8000 <= address && address <= 0xBFFF) {
		return data[address-0x8000+HEADER_SIZE];
	}
	if (0xC000 <= address && address <= 0xFFFF) {
		return data[address-0xC000+HEADER_SIZE];
	}
	printf("Unable to read data from cartridge at address %04X\n", address);
	return 0x00;
}