#include "lc3.h"
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

    memory[0x3000] = 0b0001001010000011;
    memory[0x3001] = 0b1111000000100101;

    registers[R_R1] = 0;
    registers[R_R2] = 5;
    registers[R_R3] = 3;

    registers[R_COND] = FL_ZRO;
    enum { PC_START = 0x3000 };
    registers[R_PC] = PC_START;

    while (running) {
        uint16_t instruction = memory[registers[R_PC]++];
        uint16_t op_code = instruction >> 12;

        switch (op_code)
        {
            case OP_ADD:
                op_add(instruction);
                break;
            case OP_AND:
                op_and(instruction);
                break;
            case OP_NOT:
                op_not(instruction);
                break;
            case OP_BR:
                op_br(instruction);
                break;
            case OP_JMP:
                op_jmp(instruction);
                break;
            case OP_JSR:
                op_jsr(instruction);
                break;
            case OP_LD:
                op_ld(instruction);
                break;
            case OP_LDI:
                op_ldi(instruction);
                break;
            case OP_LDR:
                op_ldr(instruction);
                break;
            case OP_LEA:
                op_lea(instruction);
                break;
            case OP_ST:
                op_st(instruction);
                break;
            case OP_STI:
                op_sti(instruction);
                break;
            case OP_STR:
                op_str(instruction);
                break;
            case OP_TRAP:
                op_trap(instruction);
                break;
            case OP_RES:
            case OP_RTI:
            default:
                abort();
                break;
        }
    }
}