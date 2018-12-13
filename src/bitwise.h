#ifndef BITWISE_H
#define BITWISE_H

#include <stdint.h>
#include "definitions.h"

namespace bitwise {
	extern bool check_bit(u8 value, u8 bit);
	extern u8 combine_nibbles(u8 upper, u8 lower);
	extern u16 combine_bytes(u8 lower, u8 upper);
	extern u8 set_bit(u8 value, u8 bit);
	extern u8 clear_bit(u8 value, u8 bit);
	extern bool get_bit(u8 value, u8 bit);
	extern u8 set_bit_to(u8 value, u8 bit, bool on);
};

#endif // BITWISE_H