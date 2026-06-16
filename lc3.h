#ifndef LC3_H
#define LC3_H
#define MEMORY_MAX (1 << 16)

#include <stdint.h>
#include <stdio.h>
#include <Windows.h>

extern HANDLE hStdin;

// Hardware/Operating System
uint16_t  memory[MEMORY_MAX];

// Registers
enum {
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
};

// Keyboard input flags
enum
{
    MR_KBSR = 0xFE00, /* keyboard status */
    MR_KBDR = 0xFE02  /* keyboard data */
};

// Conditional flags
enum {
    FL_POS = 1 << 0, 
    FL_ZRO = 1 << 1, 
    FL_NEG = 1 << 2
};

// Op codes
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

// Trap Vector
enum {
  TRAP_GETC = 0X20, // Get character from keyboard, not echoed onto terminal
  TRAP_OUT = 0X21, // Output a character
  TRAP_PUTS = 0X22, // Output a word string
  TRAP_IN = 0X23, // Get character from keyboard, echoed onto terminal
  TRAP_PUTSP = 0X24, // Output a byte string
  TRAP_HALT = 0X25 // Halt the program
};

extern int running;
uint16_t registers[R_COUNT];
extern uint16_t registers[];

// Instructions/methods
uint16_t sign_extend(uint16_t x, int bit_count);
void update_flags(uint16_t instruction);

uint16_t swap16(uint16_t x);
void read_image_file(FILE *file);
int read_image(const char *image_path);
uint16_t mem_read(uint16_t address);

// Functions I don't understand
void disable_input_buffering();
void restore_input_buffering();
uint16_t check_key();
void handle_interrupt(int signal);

void op_add(uint16_t instruction);
void op_and(uint16_t instruction);
void op_not(uint16_t instruction);
void op_br(uint16_t instruction);
void op_jmp(uint16_t instruction);
void op_jsr(uint16_t instruction);
void op_ld(uint16_t instruction);
void op_ldi(uint16_t instruction);
void op_ldr(uint16_t instruction);
void op_lea(uint16_t instruction);
void op_st(uint16_t instruction);
void op_sti(uint16_t instruction);
void op_str(uint16_t instruction);

void op_trap(uint16_t instruction);
void trap_getc();
void trap_out();
void trap_puts();
void trap_in();
void trap_putsp();
void trap_halt();

#endif