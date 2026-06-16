#include <stdio.h>
#include <stdint.h>

uint16_t swap16(uint16_t x) {
    return (x << 8) | (x >> 8);
}

int main() {
    FILE *f = fopen("test.obj", "wb");

    uint16_t origin = swap16(0x3000);
    fwrite(&origin, sizeof(uint16_t), 1, f);

    uint16_t instructions[] = {
        swap16(0b0101000000100000),  // AND R0, R0, #0
        swap16(0b0001000000100001),  // ADD R0, R0, #1
        swap16(0b1111000000100010),  // TRAP PUTS
        swap16(0b1111000000100101),  // HALT
    };

    fwrite(&instructions, sizeof(uint16_t), sizeof(instructions) / sizeof(uint16_t), f);
    fclose(f);
}