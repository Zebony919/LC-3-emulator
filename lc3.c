#include <stdio.h>
#include <stdint.h>

#define MEMORY_MAX (1 << 16)

uint16_t sign_extend(uint16_t x, int bit_count);
void update_flags(uint16_t r);

enum
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC, 
    R_COND,
    R_COUNT
    // All registers values are 1 + previous_value based of the default value of c
    // So R_R4 = 4 and R_COUNT = 10
};

enum
{
    OP_BR = 0, 
    OP_ADD,    
    OP_LD,    
    OP_ST,    
    OP_JSR,   
    OP_AND,   
    OP_LDR,    
    OP_STR,   
    OP_RTI,   
    OP_NOT,   
    OP_LDI,   
    OP_STI,   
    OP_JMP,   
    OP_RES,   
    OP_LEA,   
    OP_TRAP   
};

enum
{
    FL_POS = 1 << 0, 
    FL_ZRO = 1 << 1, 
    FL_NEG = 1 << 2, 
};

uint16_t  memory[MEMORY_MAX];
uint16_t reg[R_COUNT];

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("lcs [image-file1] ...\n");
        exit(2);
    }

    for (int i = 1; i < argc; i++) {
        if (!read_image(argv[i])) {
            printf("failed to load image: %s\n", argv[i]);
        }
    }

    reg[R_COND] = FL_ZRO;

    enum {PC_START = 0x3000};
    reg[R_PC] = PC_START;

    int running = 1;
    while (running) {
        uint16_t instr = mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12;

        switch (op)
        {
            case OP_ADD:
                break;
            case OP_AND:
                break;
            case OP_NOT:
                break;
            case OP_BR:
                break;
            case OP_JMP:
                break;
            case OP_JSR:
                break;
            case OP_LD:
                break;
            case OP_LDI:
                break;
            case OP_LDR:
                break;
            case OP_LEA:
                break;
            case OP_ST:
                break;
            case OP_STI:
                break;
            case OP_STR:
                break;
            case OP_TRAP:
                break;
            case OP_RES:
            case OP_RTI:
            default:
                break;
        }


    }
}

uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> bit_count - 1) & 1) { // Check the leftmost bit. Whether the number is positive/negative
        x |= (0xFFFF << bit_count); // Define a 16-bit binary number with 16 - bit_count amount of 1's and bit_count 0's and OR with x
    }
    return x;
}

void update_flags(uint16_t r) { // Check the leftmost bit to see it's sign
    if (reg[r] == 0) {
        reg[R_COND] = FL_ZRO;
    } else if (reg[r] << 15) {
        reg[R_COND] = FL_NEG;
    } else {
        reg[R_COND] = FL_POS;
    }
}