#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

/*
 * Memory map
 * 0x0000 - 0x1FFF -- ROM
 * 0x2000 - 0x7FFF -- RAM
 * 0x8000 - 0xFFFF -- I/O
 */

#define MEM_MAP_IO_READ  0xD000
#define MEM_MAP_IO_WRITE 0xD001

const int MAX_8080_RAM = (1 << 16) - 1;

typedef union RegisterPair {
    struct {
        uint8_t H;
        uint8_t L;
    } Pair;
    uint16_t HL;
} RegisterPair;

typedef struct Bus {
    uint8_t *mem; // not owned
} Bus;

uint8_t bus_read(Bus *bus, uint16_t addr)
{
    if (addr == MEM_MAP_IO_READ) {
        int input_byte = getchar();
        if (input_byte == EOF) {
            fprintf(stderr, "got EOF\n");
            exit(1); // FIXME
        }
        return (uint8_t)input_byte;
    }
    uint8_t result = bus->mem[addr];
    return result;
}

void bus_write(Bus *bus, uint16_t addr, uint8_t val)
{
    if (addr == MEM_MAP_IO_WRITE) {
        puts("\033[32m");
        putchar((int)val);
        puts("\033[0m");
    } else {
        bus->mem[addr] = val;
    }
}

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
    Bus *bus;
    uint32_t cycle_count;
    bool stopped;
} Intel8080;

void cpu_8080_init(Intel8080 *cpu, Bus *bus)
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
    cpu->bus = bus;
    cpu->cycle_count = 0;
    cpu->stopped = false;
}

#define UNIMPLEMENTED(op) { \
    fprintf(stderr, "Unimplemented op code 0x%02x\n", op); \
    exit(1); \
}

#ifdef DUMP_STATE_EACH_CYCLE
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
    // TODO: ascii view
    for (int i = 0; i < len; i++) {
        uint8_t val = bus_read(cpu->bus, start + i);
        int ansicol = 37; // white for zero
        if (val != 0) {
            ansicol = 34; // blue
        }
        printf("\033[%dm%02x\033[m ", ansicol, val);
        if (i % 16 == 15) {
            printf("\n");
        }
    }
    printf("\n");
}

#endif

typedef struct Instruction {
    int cycles;
    char *inst;
    int size;
    void (*exec)(Intel8080 *cpu);
} Instruction;

Instruction instructions[] = {
    {
        4,
        "NOP",
        1,
        NULL,
    },
};

#include "mnemonics-table.inc"
#include "sizes-table.inc"
//TODO: count cycles
//#include "cycles-table.inc"

void cpu_8080_reset(Intel8080 *cpu)
{
    cpu->PC = 0;
    uint16_t addr;
    while (!cpu->stopped) {
        // FETCH INSTRUCTION
        uint8_t op = bus_read(cpu->bus, cpu->PC);
        // DECODE INSTRUCTION
        const char *mnemonic = mnemonics_8080[op];
        printf("\033[33;1m%s\033[0m\n", mnemonic);
        // ADVANCE PC
        uint8_t size = inst_sizes_8080[op];
        cpu->PC += 1;
        size -= 1;
        addr = 0;
        switch (size) {
            case 0:
                break;
            case 1:
                addr = (uint16_t)bus_read(cpu->bus, cpu->PC + 1);
                cpu->PC += 1;
                break;
            case 2:
                addr = (bus_read(cpu->bus, cpu->PC + 1) << 8) |
                        bus_read(cpu->bus, cpu->PC + 0);
                cpu->PC += 2;
                break;
            default:
                // shouldn't get here
                assert(false);
        }
        // DISPATCH INSTRUCTION
        switch (op) {
            case 0x00: // NOP
            case 0x10: // NOP
            case 0x20: // NOP
            case 0x30: // NOP
                break;
            case 0x32: // STA a16
                bus_write(cpu->bus, addr, cpu->A);
                break;
            case 0x3a: // LDA a16
                cpu->A = bus_read(cpu->bus, addr);
                break;
            case 0x47: // MOV B,A
                cpu->B = cpu->A;
                break;
            case 0x76: // HLT
                cpu->stopped = true;
                break;
            case 0x80: // ADD B
                cpu->CF = ((uint16_t)cpu->A + (uint16_t)cpu->B) > 255;
                cpu->A = cpu->A + cpu->B;
                cpu->SF = (cpu->A & 0x80) > 1;
                cpu->ZF = cpu->A == 0;
                break;
            default:
                UNIMPLEMENTED(op);
        }
#ifdef DUMP_STATE_EACH_CYCLE
        cpu_8080_dump(cpu);
        cpu_8080_dump_mem(cpu, 0, 0x20);
#endif
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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s rom\n", argv[0]);
        return 1;
    }

    char *pathname = argv[1];
    int fd = open(pathname, O_RDONLY);
    if (fd == -1) {
        perror("opening rom file");
        return 1;
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) == -1) {
        perror("fstat on rom file");
        return 1;
    }

    if (statbuf.st_size > MAX_8080_RAM) {
        fprintf(stderr, "can only address 64KB RAM\n");
        return 1;
    }

    uint16_t rom_length = statbuf.st_size;

    void *rom_contents = mmap(NULL, rom_length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (rom_contents == MAP_FAILED) {
        perror("mmap on rom file");
        return 1;
    }

    uint8_t *mem = xmalloc(MAX_8080_RAM);
    memcpy(mem, rom_contents, rom_length);

    Bus bus = {0};
    bus.mem = mem;
    Intel8080 cpu = {0};
    cpu_8080_init(&cpu, &bus);
    cpu_8080_reset(&cpu);

    free(mem);
    munmap(rom_contents, rom_length);
    close(fd);

    return 0;
}
