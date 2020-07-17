#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

const int MAX_8080_RAM = (1 << 16) - 1;

typedef struct Intel8080 {
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t F;
    uint8_t H;
    uint8_t L;
    uint16_t PC;
    uint16_t SP;
    uint8_t SF;
    uint8_t ZF;
    uint8_t AF;
    uint8_t PF;
    uint8_t CF;
    uint8_t *mem;
    uint16_t mem_size;
    uint32_t cycle_count;
    bool halted;
} Intel8080;

void cpu_8080_init(Intel8080 *cpu, uint8_t *mem, uint16_t mem_size)
{
    cpu->A = 0;
    cpu->B = 0;
    cpu->C = 0;
    cpu->D = 0;
    cpu->E = 0;
    cpu->F = 0;
    cpu->H = 0;
    cpu->L = 0;
    cpu->PC = 0;
    cpu->SP = 0;
    cpu->SF = 0;
    cpu->ZF = 0;
    cpu->AF = 0;
    cpu->PF = 0;
    cpu->CF = 0;
    cpu->mem = mem;
    cpu->mem_size = mem_size;
    cpu->cycle_count = 0;
    cpu->halted = false;
}

#define UNIMPLEMENTED(op) { \
    fprintf(stderr, "Unimplemented op code 0x%02x\n", op); \
    exit(1); \
}

void cpu_8080_dump(Intel8080 *cpu)
{
    printf("\033[36mPC=%02x A=%02x B=%02x C=%02x D=%02x E=%02x H=%02x L=%02x S=%d Z=%d A=%d P=%d C=%d\033[m\n",
            cpu->PC,
            cpu->A,
            cpu->B,
            cpu->C,
            cpu->D,
            cpu->E,
            cpu->H,
            cpu->L,
            cpu->SF,
            cpu->ZF,
            cpu->AF,
            cpu->PF,
            cpu->CF);
}

void cpu_8080_dump_mem(Intel8080 *cpu, uint16_t start, uint16_t len)
{
    for (int i = 0; i < len; i++) {
        uint8_t val = *(cpu->mem + start + i);
        int ansicol = 30;
        if (val != 0) {
            ansicol = 34;
        }
        printf("\033[%d;1m%02x\033[m ", ansicol, val);
        if (i % 16 == 15) {
            printf("\n");
        }
    }
    printf("\n");
}

void cpu_8080_reset(Intel8080 *cpu)
{
    cpu->PC = 0;
    while (!cpu->halted) {
        assert(cpu->PC < cpu->mem_size);
        // FETCH INSTRUCTION
        uint8_t op = cpu->mem[cpu->PC];
        // ADVANCE PC
        cpu->PC += 1;
        // DECODE INSTRUCTION
        // DISPATCH INSTRUCTION
        switch (op) {
            case 0x00: // NOP
            case 0x10: // NOP
            case 0x20: // NOP
            case 0x30: // NOP
                cpu->cycle_count += 4;
                break;
            case 0x32: // STA a16
                cpu->cycle_count += 13;
                {
                    uint8_t lo = cpu->mem[cpu->PC + 0];
                    uint8_t hi = cpu->mem[cpu->PC + 1];
                    uint16_t addr = (hi << 8) | lo;
                    cpu->mem[addr] = cpu->A;
                    printf("\033[33;1m%s 0x%04x\033[m\n", "STA", addr);
                }
                cpu->PC += 2;
                break;
            case 0x3a: // LDA a16
                cpu->cycle_count += 13;
                {
                    uint8_t lo = cpu->mem[cpu->PC + 0];
                    uint8_t hi = cpu->mem[cpu->PC + 1];
                    uint16_t addr = (hi << 8) | lo;
                    cpu->A = cpu->mem[addr];
                    printf("\033[33;1m%s 0x%04x\033[m\n", "LDA", addr);
                }
                cpu->PC += 2;
                break;
            case 0x47: // MOV B,A
                cpu->cycle_count += 5;
                cpu->B = cpu->A;
                printf("\033[33;1m%s\033[m\n", "MOV B,A");
                break;
            case 0x76: // HLT
                cpu->cycle_count += 7;
                cpu->halted = true;
                printf("\033[33;1m%s\033[m\n", "HLT");
                break;
            case 0x80: // ADD B
                cpu->cycle_count += 4;
                cpu->CF = ((uint16_t)cpu->A + (uint16_t)cpu->B) > 255;
                cpu->A = cpu->A + cpu->B;
                cpu->SF = (cpu->A & 0x80) > 1;
                cpu->ZF = cpu->A == 0;
                printf("\033[33;1m%s\033[m\n", "ADD B");
                break;
            default:
                UNIMPLEMENTED(op);
        }
        cpu_8080_dump(cpu);
        cpu_8080_dump_mem(cpu, 0, 0x20);
    }
}

void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        perror("could not malloc");
        exit(1);
    }
    return ptr;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s num1 num2\n", argv[0]);
        return 1;
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    uint8_t program[] = {
        0x3a, 0x10, 0x00,   // LDA 0010h
        0x47,               // MOV B, A
        0x3a, 0x11, 0x00,   // LDA 0011h
        0x80,               // ADD B
        0x32, 0x12, 0x00,   // STA 0012h
        0x76,               // HLT
        0x00,               // NOP
        0x00,               // NOP
        0x00,               // NOP
        0x00,               // NOP
    };

    uint8_t *mem = xmalloc(MAX_8080_RAM);
    memcpy(mem, program, sizeof(program));

    mem[0x0010] = (uint8_t)num1;
    mem[0x0011] = (uint8_t)num2;

    Intel8080 cpu = {0};
    cpu_8080_init(&cpu, mem, (uint16_t)MAX_8080_RAM);
    cpu_8080_reset(&cpu);

    printf("%d\n", mem[0x0012]);

    return 0;
}
