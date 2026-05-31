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

    // JMP test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 0x3005;
    op_jmp(0b1100000001000000);

    if (registers[R_PC] == 0x3005) {
        printf("JMP Test: Pass (PC: 0x%x)\n", registers[R_PC]);
    } else {
        printf("JMP Test: Fail (PC: 0x%x)\n", registers[R_PC]);
    }

    // JSR test
    registers[R_PC] = 0x3000;
    op_jsr(0b0100100000000011);

    if ((registers[R_PC] == 0x3003) && (registers[R_R7] == 0x3000)) {
        printf("JSR Test: Pass (PC: 0x%x, R7: 0x%x)\n", registers[R_PC], registers[R_R7]);
    } else {
        printf("JSR Test: Fail (PC: 0x%x, R7: 0x%x)\n", registers[R_PC], registers[R_R7]);
    }

    // JSSR test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 5;
    op_jsr(0b0100000001000000);

    if ((registers[R_PC] == 0x3005) && (registers[R_R7] == 0x3000)) {
        printf("JSSR Test: Pass (PC: 0x%x, R7: 0x%x)\n", registers[R_PC], registers[R_R7]);
    } else {
        printf("JSSR Test: Fail (PC: 0x%x, R7: 0x%x)\n", registers[R_PC], registers[R_R7]);
    }

    // LD test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 0;
    memory[0x3005] = 10;
    op_ld(0b0010001000000101);

    if (registers[R_R1] == 10) {
        printf("LD Test: Pass (R1: %d)\n", registers[R_R1]);
    } else {
        printf("LD Test: Fail (R1: %d)\n", registers[R_R1]);
    }

    // LDI test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 0;
    memory[0x3005] = 0x4000;
    memory[0x4000] = 20;
    op_ldi(0b1010001000000101);

    if (registers[R_R1] == 20) {
        printf("LDI Test: Pass (R1: %d)\n", registers[R_R1]);
    } else {
        printf("LDI Test: Fail (R1: %d)\n", registers[R_R1]);
    }

    // LDR test
    registers[R_R1] = 0;
    registers[R_R2] = 0x3000;
    memory[0x3004] = 30;
    op_ldr(0b0110001010000100);

    if (registers[R_R1] == 30) {
        printf("LDR Test: Pass (R1: %d)\n", registers[R_R1]);
    } else {
        printf("LDR Test: Fail (R1: %d)\n", registers[R_R1]);
    }

    // LEA test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 0;
    op_lea(0b1110001000000101);

    if (registers[R_R1] == 0x3005) {
        printf("LEA Test: Pass (R1: 0x%x)\n", registers[R_R1]);
    } else {
        printf("LEA Test: Fail (R1: 0x%x)\n", registers[R_R1]);
    }

    // ST test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 50;
    memory[0x3005] = 0;
    op_st(0b0011001000000101);

    if (memory[0x3005] == 50) {
        printf("ST Test: Pass (Memory[0x3005]: %d)\n", memory[0x3005]);
    } else {
        printf("ST Test: Fail (Memory[0x3005]: %d)\n", memory[0x3005]);
    }

    // STI test
    registers[R_PC] = 0x3000;
    registers[R_R1] = 50;
    memory[0x3005] = 0x3008;
    memory[0x3008] = 0;
    op_sti(0b1011001000000101);

    if (memory[0x3008] == 50) {
        printf("STI Test: Pass (Memory[0x3008]: %d)\n", memory[0x3008]);
    } else {
        printf("STI Test: Fail (Memory[0x3008]: %d)\n", memory[0x3008]);
    }

    // STR test
    registers[R_R1] = 50;
    registers[R_R2] = 0x3000;
    memory[0x3005] = 0;
    op_str(0b0111001010000101);

    if (memory[0x3005] == 50) {
        printf("STR Test: Pass (Memory[0x3005]: %d)\n", memory[0x3005]);
    } else {
        printf("STR Test: Fail (Memory[0x3005]: %d)\n", memory[0x3005]);
    }
}