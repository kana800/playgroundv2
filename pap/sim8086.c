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

// REG_TABLE[w][idx]
char* REG_TABLE[2][8] = {
    {"al", "cl", "dl", "bl", "ah", "ch", "dh", "ah"},
    {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}
};

char* RM_TABLE[3][8] = {
    {"[bx + si]", "[bx + di]", "[bp + sp]", "[bp + di]", "si", "di", "", "bx"},
    {"[bx + si]", "[bx + di]", "[bp + sp]", "[bp + di]", "si", "di", "", "bx"},
    {"[bx + si]", "[bx + di]", "[bp + sp]", "[bp + di]", "si", "di", "", "bx"}
};

char* OPCODE_TABLE[1] = {"mov"};

typedef struct __opcode__ {
    int idx;
    int d;
    int w;
    int mod;
    int reg;
    int rm;
} opcode; 

void decodeOptionalBytes(
    unsigned char buffer[], size_t bytesread)
{

}

opcode decodeOpCodeAddr(
    unsigned char buffer[], size_t bytesread)
{
    opcode oc;
    unsigned char opcode_b = buffer[0] >> 2; 
    switch (opcode_b)
    {
        case 0b00100010: // mov
            oc.idx = 0;
            break;
        default:
            oc.idx = -1;
            break;
    }

    unsigned char dw = buffer[0] << 6;
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
        default:
            oc.d = -1;
            oc.w = -1;
            break;
    }

    assert(oc.d != -1);
    assert(oc.w != -1);

    unsigned char mod = buffer[1] & 11000000;
    unsigned char reg = (buffer[1] << 2 ) & 11100000;
    unsigned char rm = buffer[1] << 5;

    switch (mod)
    {
        case 0b00000000:
            oc.mod = 0;
            break;
        case 0b01000000:
            oc.mod = 1;
            break;
        case 0b10000000:
            oc.mod = 2;
            break;
        case 0b11000000:
            oc.mod = 3;
            break;
        default:
            oc.mod = -1;
            break;
    }

    assert(oc.mod != -1);

    switch (reg)
    {
        case 0b00000000:
            oc.reg = 0;
            break;
        case 0b00100000:
            oc.reg = 1;
            break;
        case 0b01000000:
            oc.reg = 2;
            break;
        case 0b01100000:
            oc.reg = 3;
            break;
        case 0b10000000:
            oc.reg = 4;
            break;
        case 0b10100000:
            oc.reg = 5;
            break;
        case 0b11000000:
            oc.reg = 6;
            break;
        case 0b11100000:
            oc.reg = 7;
            break;
        default:
            oc.reg = -1;
    }

    assert(oc.reg != -1);

    switch (rm)
    {
        case 0b00000000:
            oc.rm = 0;
            break;
        case 0b00100000:
            oc.rm = 1;
            break;
        case 0b01000000:
            oc.rm = 2;
            break;
        case 0b01100000:
            oc.rm = 3;
            break;
        case 0b10000000:
            oc.rm = 4;
            break;
        case 0b10100000:
            oc.rm = 5;
            break;
        case 0b11000000:
            oc.rm = 6;
            break;
        case 0b11100000:
            oc.rm = 7;
            break;
        default:
            oc.rm = -1;
    }

    assert(oc.rm != -1);
}


int main(int argc, char* argv[]) 
{
    unsigned char buffer[6];

    FILE* fptr = fopen(argv[1], "rb");
    int count = 0;
    size_t bytesread = 0;
    while (!feof(fptr)) {
        bytesread = fread(buffer, sizeof(unsigned char), 6, fptr);
        opcode oc = decodeOpCodeAddr(buffer, bytesread);
        // decode(buffer, bytesread);
        count += 1;
    }
    fclose(fptr);

    return 0;
}
