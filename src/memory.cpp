#include "memory.h"

Memory::Memory(Cartridge& cartridge) : cartridge(cartridge) {

}

u8 Memory::readByte(u16 address) {
	/*
	CPU memory map
	$0000-$07FF:	2KB internal RAM
	$0800-$0FFF:
	$1000-$17FF:	Mirrors of $0000-%0800
	$1800-$1FFF:
	$2000-$2007:	NES PPU registers
	$2008-$3FFF:	Mirrors of $2000-2007 (repeats every 8 bytes)
	$4000-$4017:	NES APU and IO registers
	$4018-$401F:	APU and IO functionality that is normally disabled
	$4020-$FFFF:	Cartridge space: PRG ROM, PRG RAM, and mapper registers
	*/

	// Internal RAM and RAM mirrors
	if (0x0000 <= address && address <= 0x07FF) {
		u8 byte = data[address];
		printf("\033[31;1m[READ] Internal RAM: %02X,%04X\033[0m\n", byte, address);
		return byte;
		// RAM allocation strategies located at https://wiki.nesdev.com/w/index.php/Sample_RAM_map
		// Stack is located from $01A0 to $01FF
	}
	if (0x0800 <= address && address <= 0x0FFF) {
		u8 byte = data[address-0x0800];
		// printf("\033[31;1m[READ] Internal RAM mirror: %02X,%04X\033[0m\n", byte, address);
		return byte;
	}
	if (0x1000 <= address && address <= 0x17FF) {
		u8 byte = data[address-0x1000];
		// printf("\033[31;1m[READ] Internal RAM mirror: %02X,%04X\033[0m\n", byte, address);
		return byte;
	}
	if (0x1800 <= address && address <= 0x1FFF) {
		u8 byte = data[address-0x1800];
		// printf("\033[31;1m[READ] Internal RAM mirror: %02X,%04X\033[0m\n", byte, address);
		return byte;
	}

	// NES PPU registers and mirrors
	if (0x2000 <= address && address <= 0x2007) {
		u8 byte = data[address];
		// printf("\033[31;1m[READ] NES PPU: %02X,%04X\033[0m\n", byte, address);
		return byte;
	}
	
	// Mirror NES PPU registers every 8 bytes
	if (0x2008 <= address && address <= 0x3FFF) {
		u8 byte = data[(address%8)+0x2000];
		// printf("\033[31;1m[READ] NES PPU Mirror: %02X,%04X\033[0m\n", byte, address);
		return byte;
	}


	// APU and IO registers
	if (0x4000 <= address && address <= 0x4017) {
		u8 byte = data[address];
		// printf("\033[31;1m[READ] IO and APU: %02X,%04X\033[0m\n", byte, address);
		return data[address];
	}

	// Cartridge space ROM, this depends on the mapper.
	if (0x4020 <= address && address <= 0xFFFF) {
		// Mapper hardware is now implemented in the cartridge class to stay more organized
		// (and more true to the hardware I guess) just give an address and the cartridge
		// class should be able to handle the rest of the logic.
		u8 byte = cartridge.read(address);
		// printf("\033[31;1m[READ] PRG ROM: %02X,%04X\033[0m\n", byte, address);
		return byte;
	}

	// printf("\033[31;1m[READ] Unknown memory location: %04X\033[0m\n", address);
	return 0x00;
}

void Memory::writeByte(u8 byte, u16 address) {
	// Internal RAM and RAM mirrors
	if (0x0000 <= address && address <= 0x07FF) {
		data[address] = byte;
		printf("\033[31;1m[WRITE] Internal RAM: %02X,%04X\033[0m\n", byte, address);
		return;
	}
	if (0x0800 <= address && address <= 0x0FFF) {
		data[address-0x0800] = byte;
		// printf("\033[31;1m[WRITE] Internal RAM Mirror: %02X,%04X\033[0m\n", byte, address);
		return;
	}
	if (0x1000 <= address && address <= 0x17FF) {
		data[address-0x1000] = byte;
		// printf("\033[31;1m[WRITE] Internal RAM Mirror: %02X,%04X\033[0m\n", byte, address);
		return;
	}
	if (0x1800 <= address && address <= 0x1FFF) {
		data[address-0x1800] = byte;
		// printf("\033[31;1m[WRITE] Internal RAM Mirror: %02X,%04X\033[0m\n", byte, address);
		return;
	}

	// NES PPU registers and mirrors
	if (0x2000 <= address && address <= 0x2007) {
		data[address] = byte;
		// printf("\033[31;1m[WRITE] NES PPU: %02X,%04X\033[0m\n", byte, address);
		return;
	}

	// NES PPU mirrors every 8 bytes
	if (0x2008 <= address && address <= 0x3FFF) {
		data[(address%8)+0x2000] = byte;
		// printf("\033[31;1m[WRITE] NES PPU Mirror: %02X,%04X\033[0m\n", byte, address);
		return;
	}

	// APU and IO registers
	if (0x4000 <= address && address <= 0x4017) {
		data[address] = byte;
		// printf("\033[31;1m[WRITE] IO and APU: %02X,%04X\033[0m\n", byte, address);
		return;
	}

	// TODO: Some cartridge mappers contain on board ram that can be written to

	// printf("[WRITE] Unknown memory location: %04X\n", address);
}