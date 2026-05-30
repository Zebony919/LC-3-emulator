#include "lc3.h"
#include <stdio.h>
#include <stdint.h>

int main() {

    // ADD test
    registers[R_R1] = 0;
    registers[R_R2] = 2;
    registers[R_R3] = 3;
    op_add(0b0001001010000011);

    if (registers[R_R1] == 5) {
        printf("ADD Test: Pass (Output: %d)\n", registers[R_R1]);
    } else {
        printf("ADD Test: Fail (Output: %d)\n", registers[R_R1]);
    }

    // AND test
    registers[R_R1] = 0;
    registers[R_R2] = 15;
    registers[R_R3] = 8;
    op_and(0b0101001010000011);
    
    if (registers[R_R1] == 8) {
        printf("AND Test: Pass (Output: %d)\n", registers[R_R1]);
    } else {
        printf("AND Test: Fail (Output: %d)\n", registers[R_R1]);
    }

    // NOT test
    registers[R_R1] = 0;
    registers[R_R2] = 4;
    op_not(0b1001001010111111);

    if (registers[R_R1] == (uint16_t)~4) {
        printf("NOT Test: Pass (Output: %d)\n", registers[R_R1]);
    } else {
        printf("NOT Test: Fail (Output: %d)\n", registers[R_R1]);
    }

    // BR test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 1;
    update_flags(R_R1); // set R_COND to positive for test purposes
    op_br(0b0000001000000011);

    if (registers[R_PC] == 0x3003) {
        printf("BR Test: Pass (PC: 0x%x)\n", registers[R_PC]);
    } else {
        printf("BR Test: Fail (PC: 0x%x)\n", registers[R_PC]);
    }
}