#ifndef LC3_H
#define LC3_H
#define MEMORY_MAX (1 << 16)

#include <stdint.h>

// Hardware/Operating System
uint16_t  memory[MEMORY_MAX];

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

enum {
  TRAP_GETC = 0X20,
  TRAP_OUT = 0X21,
  TRAP_PUTS = 0X22,
  TRAP_IN = 0X23,
  TRAP_PUTSP = 0X24,
  TRAP_HALT = 0X25
};

uint16_t registers[R_COUNT];
extern uint16_t registers[];

// Instructions/methods
uint16_t sign_extend(uint16_t x, int bit_count);
void update_flags(uint16_t instruction);

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

#endif