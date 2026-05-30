#include "lc3.h"
#include <stdio.h>
#include <stdint.h>

//int main() {
//  printf("Hello World!");
//}

uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> bit_count - 1) & 1) { // Check the leftmost bit. Whether the number is positive/negative
        x |= (0xFFFF << bit_count); // Define a 16-bit binary number with 16 - bit_count amount of 1's and bit_count 0's and OR with x
    }
    return x;
}

void update_flags(uint16_t r) {
    if (registers[r] == 0) {
        registers[R_COND] = 0b010;  // zero
    } else if (registers[r] >> 15) {
        registers[R_COND] = 0b100;  // negative
    } else {
        registers[R_COND] = 0b001;  // positive
    }
}

// Instructions
void op_add(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t source_register_one = (instruction >> 6) & 0x7;
    uint16_t immediateFlag = (instruction >> 5) & 0x1;

    if (immediateFlag) {
        uint16_t immediate_value = instruction & 0x1F;
        uint16_t immediate_value_sign_extended = sign_extend(immediate_value, 5);
        registers[destination] = registers[source_register_one] + immediate_value_sign_extended;
    } else {
        uint16_t source_register_two = instruction & 0x7;
        registers[destination] = registers[source_register_one] + registers[source_register_two];
    }
}

void op_and(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t source_register_one = (instruction >> 6) & 0x7;
    uint16_t immediateFlag = (instruction >> 5) & 0x1;

    if (immediateFlag) {
        uint16_t immediate_value = instruction & 0x1F;
        uint16_t immediate_value_sign_extended = sign_extend(immediate_value, 5);
        registers[destination] = registers[source_register_one] & immediate_value_sign_extended;
    } else {
        uint16_t source_register_two = instruction & 0x7;
        registers[destination] = registers[source_register_one] & registers[source_register_two];
    }
}

void op_not(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t source_register_one = (instruction >> 6) & 0x7;

    registers[destination] = ~(registers[source_register_one]);
}

void op_br(uint16_t instruction) {
    uint16_t conditions = (instruction >> 9) & 0x7;
    uint16_t pc_offset = instruction & 0x1FF;

    if (conditions & registers[R_COND]) {
        registers[R_PC] += pc_offset;
    }
}