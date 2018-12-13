#include "bitwise.h"


namespace bitwise {
	extern bool check_bit(u8 value, u8 bit) {
		return (value >> bit) & 0x1;
	};
	extern u8 combine_nibbles(u8 upper, u8 lower) {
		return static_cast<u8>((upper << 4) | (lower));
	};
	extern u16 combine_bytes(u8 lower, u8 upper) {
		return static_cast<u16>((upper << 8) | (lower));
	}
	extern u8 set_bit(u8 value, u8 bit) {
		return value | (1UL << bit);
	};
	extern u8 clear_bit(u8 value, u8 bit) {
		return value & ~(1UL << bit);
	};
	extern bool get_bit(u8 value, u8 bit) {
		return value >> bit & 1;
	};
	extern u8 set_bit_to(u8 value, u8 bit, bool on) {
		if (on) return set_bit(value, bit);
		else return clear_bit(value, bit);
	};
};