#include "lc3.h"
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* windows only */
#include <Windows.h>
#include <conio.h>  // _kbhit

HANDLE hStdin = INVALID_HANDLE_VALUE;
DWORD fdwMode, fdwOldMode;

void disable_input_buffering() {
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &fdwOldMode); /* save old mode */
    fdwMode = fdwOldMode
            ^ ENABLE_ECHO_INPUT  /* no input echo */
            ^ ENABLE_LINE_INPUT; /* return when one or
                                    more characters are available */
    SetConsoleMode(hStdin, fdwMode); /* set new mode */
    FlushConsoleInputBuffer(hStdin); /* clear buffer */
}

void restore_input_buffering() {
    SetConsoleMode(hStdin, fdwOldMode);
}

uint16_t check_key() {
    return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}

void handle_interrupt(int signal) {
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    if (argc < 2) {
        printf("Usage: lc3.exe [image-file]\n");
        exit(2);
    }

    if (!read_image(argv[1])) {
        printf("Failed to load image at: %s", argv[1]);
        exit(1);
    }

    registers[R_COND] = FL_ZRO;
    enum { PC_START = 0x3000 };
    registers[R_PC] = PC_START;

    memory[0x0001] = 'H';
    memory[0x0002] = 'E';
    memory[0x0003] = 'L';
    memory[0x0004] = 'L';
    memory[0x0005] = 'O';
    memory[0x0006] = '\0';

    while (running) {
        uint16_t instruction = mem_read(registers[R_PC]++);
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

    restore_input_buffering();
}