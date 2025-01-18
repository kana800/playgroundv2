#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

/* 8086 instruction size is 1 to 6 bytes 
 * The first 6 bits determines the OpCode (operation code)
 * mov -> 100010 
 * [OpCode] | D | W | MOD | REG | R/M | lowerorderbitplacement | higerorderbitplacement
 * 6 bits   | 1 | 1 |  2  |  3  |  3  |
 * D -> direction ( D=0 ) direction is from the register
 *   -> direction ( D=1 ) direction to the register
 * W -> word ( W=0 ) only a byte is being transferred 
 *   -> word ( W=1 ) the whole world is being transferred
 */

#define DEBUG 0 

// REG_TABLE[w][idx]
char* REG_TABLE[8][2] = {
    {"al", "ax"}, 
    {"cl", "cx"}, 
    {"dl", "dx"}, 
    {"bl", "bx"},
    {"ah", "sp"}, 
    {"ch", "bp"}, 
    {"dh", "si"}, 
    {"bh", "di"}
};


char* RM_TABLE[2][8] = {
    {
        "[bx + si]", 
        "[bx + di]", 
        "[bp + si]", 
        "[bp + di]", 
        "si", 
        "di", 
        "", 
        "bx"
    },
    {
        "[bx + si]", 
        "[bx + di]", 
        "[bp + si]", 
        "[bp + di]",
        "si", 
        "di", 
        "bp", 
        "bx"
    }
};

char* OPCODE_TABLE[1] = {"mov"};

typedef struct __opcode__ {
    int idx;
    int d;
    int w;
    int mod;
    int reg;
    int rm;
    int bytesread; //debug
} opcode; 


void printInstruction(opcode op)
{
#if DEBUG
//    printf("mod %d\nd %d\nw %d\nreg %d\nrm %d\n", 
//           op.mod, op.d, op.w, op.reg, op.rm);
#endif

    switch (op.mod)
    {
        case 0:
            // immediate to register
            if (op.reg == 6)
            {

            } else {
                printf("mov %s,%s",RM_TABLE[0][op.rm],RM_TABLE[op.reg][op.w]);
            }
            break;
        case 3:
            // register to register instruction
            printf("mov %s,%s\n",REG_TABLE[op.rm][op.w],REG_TABLE[op.reg][op.w]);
            break;
    }
}

opcode decodeInstruction(unsigned char buffer[], int bytesread)
{
    opcode oc = {.idx=-1, .d=-1, .w=-1, .mod=-1, 
        .reg=-1, .rm=-1, .bytesread=0};

    unsigned char reg;
    unsigned char dw;


    unsigned char opcode_b = buffer[bytesread] >> 2; 
    switch (opcode_b)
    {
        case 0x22: // mov register/memory to/from register
            oc.idx = 0;
            break;
        case 0x2c: // mov immediate to register
            oc.idx = 1;
            break;
        default:
            oc.idx = -1;
            break;
    }

    assert(oc.idx != -1);

    if (oc.idx == 1)
    {
        reg = (buffer[bytesread] & 0b00000111);
        dw = (buffer[bytesread] &  0b00010000) >> 5;
    }

    // not valid opcode
    // bytesread will sent to default
    // if (oc.idx == -1) return oc;
    dw = buffer[bytesread] << 6;
    switch (dw)
    {
        case 0b00000000:
            oc.d = 0;
            oc.w = 0;
            break;
        case 0b01000000:
            oc.d = 0;
            oc.w = 1;
            break;
        case 0b10000000:
            oc.d = 1;
            oc.w = 0;
            break;
        case 0b11000000:
            oc.d = 1;
            oc.w = 1;
            break;
    }

    assert(oc.d != -1);
    assert(oc.w != -1);

    // READING 2ND BYTE
    unsigned char mod = buffer[bytesread + 1] >> 6;
    oc.reg = (buffer[bytesread + 1] & 0b00111000) >> 3;
    oc.rm = (buffer[bytesread + 1] & 0b00000111);

    switch (mod)
    {
        case 0b00:
            oc.mod = 0;
            break;
        case 0b01:
            oc.mod = 1;
            break;
        case 0b10:
            oc.mod = 2;
            break;
        case 0b11:
            oc.mod = 3;
            break;
    }

    assert(oc.mod != -1);
    assert(oc.reg != -1);
    assert(oc.rm != -1);

    // successfully read two bytes
    oc.bytesread += 2;


    return oc;
}


int main(int argc, char* argv[]) 
{
    FILE* fptr = fopen(argv[1], "rb");
    if (!fptr) return -1;

    // getting the file size
    fseek(fptr, 0L, SEEK_END);
    int len = ftell(fptr);

    // making the buffer
    unsigned char* buffer = (unsigned char *)
        malloc(sizeof(unsigned char) * len);
    fseek(fptr, 0L, SEEK_SET);

    size_t bytesread_f = fread(buffer, sizeof(unsigned char), len, fptr);

    if (bytesread_f != len)
    {
        fprintf(stderr, 
                "short read of '%s': expected %d bytes\n", argv[1], len);
        return -1;
    }

#if DEBUG
    printf("sizeof file %d, bytesread %ld\n",len, bytesread_f);
#endif

    if (ferror(fptr))
    {
        fprintf(stderr, "error inidicator set");
        return -1;
    }

    opcode oc;
    // reading the first two bytes default
    int bytesread = 0;

    printf("bits 16\n");

    // decoding the first two bytes 
    while (len > bytesread)
    {
        oc = decodeInstruction(buffer, bytesread);
        printInstruction(oc);
        bytesread = bytesread + oc.bytesread;
    }

    fclose(fptr);
    free(buffer);
    return 0;
}
