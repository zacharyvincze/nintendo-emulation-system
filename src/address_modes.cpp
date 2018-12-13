#include "cpu.h"

u8 CPU::zero_page() {
	return static_cast<u8>(get_byte_from_pc());
}

u8 CPU::zero_page_x() {
	return static_cast<u8>(get_byte_from_pc() + regX.value());
}

u8 CPU::zero_page_y() {
	return static_cast<u8>(get_byte_from_pc() + regY.value());
}

u16 CPU::pre_indexed_indirect() {
	u8 address_pointer = get_byte_from_pc() + regX.value();
	u8 low_address_byte = memory.readByte(address_pointer);
	u8 high_address_byte = memory.readByte(address_pointer+1);
	return static_cast<u16>(((high_address_byte << 8) | low_address_byte));
}

u16 CPU::post_indexed_indirect() {	// TODO: Add additional cpu cycles if page is crossed or branch is taken
	u8 address_pointer = get_byte_from_pc();
	u8 low_address_byte = memory.readByte(address_pointer);
	u8 high_address_byte = memory.readByte(address_pointer+1);
	u16 address = ((high_address_byte << 8) | low_address_byte);
	address += regY.value();
	return static_cast<u16>(address);
}

u16 CPU::absolute() {
	return static_cast<u16>(get_word_from_pc());
}

u16 CPU::absolute_x() {	// TODO: add additional cpu cycles if page is crossed or branch is taken
	return static_cast<u16>(get_word_from_pc() + regX.value());
}

u16 CPU::absolute_y() {	// TODO: add additional cpu cycles if page is crossed or branch is taken
	return static_cast<u16>(get_word_from_pc() + regY.value());
}