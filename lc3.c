#include "lc3.h"
#include <stdio.h>
#include <stdint.h>

int running = 1;

// Special Functions
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

uint16_t swap16(uint16_t x) {
    return (x << 8) | (x >> 8);
}

void read_image_file(FILE *file) {
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);
    origin = swap16(origin);

    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t *p = memory + origin;

    size_t read = fread(p, sizeof(uint16_t), max_read, file);

    while (read-- > 0) {
        *p = swap16(*p);
        p++;
    }
}

int read_image(const char *image_path) {
    FILE *file = fopen(image_path, "rb");
    if (!file) return 0;
    read_image_file(file);
    fclose(file);
    return 1;
}

uint16_t mem_read(uint16_t address) {
    if (address == MR_KBSR) {
        if (check_key()) {
            memory[MR_KBSR] = (1 << 15);  // set ready bit only
        } else {
            memory[MR_KBSR] = 0;
        }
    } else if (address == MR_KBDR) {
        memory[MR_KBDR] = getchar();  // read character here instead
    }
    return memory[address];
}

// Instructions
char *reg_names[] = {"R_R0", "R_R1", "R_R2", "R_R3", "R_R4", "R_R5", "R_R6", "R_R7"};

void op_add(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t source_register_one = (instruction >> 6) & 0x7;
    uint16_t immediateFlag = (instruction >> 5) & 0x1;

    if (immediateFlag) {
        uint16_t immediate_value = sign_extend((instruction & 0x1F), 5);
        registers[destination] = registers[source_register_one] + immediate_value;
        // printf("ADD %s=(%d), %s=(%d), %d\n", reg_names[destination], registers[destination], 
        //                                     reg_names[source_register_one], registers[source_register_one], 
        //                                     immediate_value);
    } else {
        uint16_t source_register_two = instruction & 0x7;
        registers[destination] = registers[source_register_one] + registers[source_register_two];
        //printf("ADD %s=(%d), %s=(%d), %s=(%d)\n", reg_names[destination], registers[destination], 
        //                                          reg_names[source_register_one], registers[source_register_one], 
        //                                          reg_names[source_register_two], registers[source_register_two]);
    }

    //printf("Destination Register (%s) now equals %d\n\n", reg_names[destination], registers[destination]);
}

void op_and(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t source_register_one = (instruction >> 6) & 0x7;
    uint16_t immediateFlag = (instruction >> 5) & 0x1;

    if (immediateFlag) {
        uint16_t immediate_value = sign_extend((instruction & 0x1F), 5);
        registers[destination] = registers[source_register_one] & immediate_value;
        //printf("AND %s=(%d), %s=(%d), %d\n", reg_names[destination], registers[destination], 
        //                                     reg_names[source_register_one], registers[source_register_one], 
        //                                     immediate_value);
    } else {
        uint16_t source_register_two = instruction & 0x7;
        registers[destination] = registers[source_register_one] & registers[source_register_two];
        //printf("AND %s=(%d), %s=(%d), %s=(%d)\n", reg_names[destination], registers[destination], 
        //                                          reg_names[source_register_one], registers[source_register_one], 
        //                                          reg_names[source_register_two], registers[source_register_two]);
    }

    //printf("Destination Register (%s) now equals %d\n\n", reg_names[destination], registers[destination]);
}

void op_not(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t source_register_one = (instruction >> 6) & 0x7;

    registers[destination] = ~(registers[source_register_one]);
    //printf("NOT %s=(%d)\n", reg_names[destination], registers[destination]);
    //printf("Destination Register (%s) now equals %d\n\n", reg_names[destination], registers[destination]);
}

void op_br(uint16_t instruction) {
    uint16_t conditions = (instruction >> 9) & 0x7;
    uint16_t pc_offset = instruction & 0x1FF;

    if (conditions & registers[R_COND]) {
        registers[R_PC] += pc_offset;
    }
}

void op_jmp(uint16_t instruction) {
    uint16_t base_register = (instruction >> 6) & 0x7;
    registers[R_PC] = registers[base_register];
}

void op_jsr(uint16_t instruction) {
    uint16_t mode = (instruction >> 11) & 0x1;
    registers[R_R7] = registers[R_PC];

    if (mode) {
        uint16_t pc_offset = sign_extend((instruction & 0x7FF), 11);
        registers[R_PC]  += pc_offset;
    } else {
        uint16_t base_register = (instruction >> 6) & 0x7;
        registers[R_PC]  += registers[base_register];
    }
}

void op_ld(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t pc_offset = sign_extend((instruction & 0x1FF), 9);

    registers[destination] = memory[registers[R_PC] + pc_offset];
}

void op_ldi(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t pc_offset = sign_extend((instruction & 0x1FF), 9);

    registers[destination] = memory[memory[registers[R_PC] + pc_offset]];
}

void op_ldr(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t base_register = (instruction >> 6) & 0x7;
    uint16_t offset = sign_extend((instruction & 0x3F), 6);

    registers[destination] = memory[registers[base_register] + offset];
}

void op_lea(uint16_t instruction) {
    uint16_t destination = (instruction >> 9) & 0x7;
    uint16_t pc_offset = sign_extend((instruction & 0x1FF), 9);

    registers[destination] = registers[R_PC] + pc_offset;
}

void op_st(uint16_t instruction) {
    uint16_t source_register = (instruction >> 9) & 0x7;
    uint16_t pc_offset = sign_extend((instruction & 0x1FF), 9);

    memory[registers[R_PC] + pc_offset] = registers[source_register];
}

void op_sti(uint16_t instruction) {
    uint16_t source_register = (instruction >> 9) & 0x7;
    uint16_t pc_offset = sign_extend((instruction & 0x1FF), 9);

    memory[memory[registers[R_PC] + pc_offset]] = registers[source_register];
}

void op_str(uint16_t instruction) {
    uint16_t source_register = (instruction >> 9) & 0x7;
    uint16_t base_register = (instruction >> 6) & 0x7;
    uint16_t offset = sign_extend((instruction & 0x3F), 6);
    uint16_t address = (uint16_t)(registers[base_register] + offset);

    memory[address] = registers[source_register];
}

void op_trap(uint16_t instruction) {
    uint16_t trap_vector = instruction & 0xFF;
    registers[R_R7] = registers[R_PC];

    switch (trap_vector) {
        case TRAP_GETC:
            trap_getc();
            break;
        case TRAP_OUT:
            trap_out();
            break;
        case TRAP_PUTS:
            trap_puts();
            break;
        case TRAP_IN:
            trap_in();
            break;
        case TRAP_PUTSP:
            trap_putsp();
            break;
        case TRAP_HALT:
            trap_halt();
            break;
    }
}

// Trap instructions
void trap_getc() {
    printf("Enter a character: ");
    registers[R_R0] = (uint16_t) getchar();     
    update_flags(R_R0);
}

void trap_out() {
    printf("Outputted Character: ");
    putc((char) registers[R_R0], stdout);
    printf("\n");
    fflush(stdout);
};

void trap_puts() {
    uint16_t *c = memory + registers[R_R0];

    printf("Outputted String: ");
    while (*c) {
        putc((char) *c, stdout);
        c++;
    }
    printf("\n");
    fflush(stdout);
};

void trap_in() {
    printf("Enter a character: ");
    char c = getchar();
    putc(c, stdout);
    registers[R_R0] = (uint16_t) c;
    update_flags(R_R0);
};

void trap_putsp() {
    uint16_t *c = memory + registers[R_R0];

    printf("Outputted String: ");
    while (*c) {
        char char1 = (*c) & 0xFF;
        putc(char1, stdout);
        char char2 = (*c) >> 8;
        if (char2) putc(char2, stdout);
        c++;
    }
    printf("\n");
    fflush(stdout);
};

void trap_halt() {
    puts("STATUS: HALT");
    fflush(stdout);
    running = 0;
};