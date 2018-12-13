#include "register.h"

uint8_t ByteRegister::value() {
	return reg_value;
}

void ByteRegister::set(uint8_t value) {
	reg_value = value;
}

void ByteRegister::decrement() {
	reg_value--;
}

void ByteRegister::increment() {
	reg_value++;
}

void ByteRegister::reset() {
	set(0x00);
}

uint16_t WordRegister::value() {
	return reg_value;
}

void WordRegister::set(uint16_t value) {
	reg_value = value;
}

void WordRegister::decrement() {
	reg_value--;
}

void WordRegister::increment() {
	reg_value++;
}