#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

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
    {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "AH"},
    {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"}
};

char* RM_TABLE[3][8] = {
    {"[BX + SI]", "[BX + DI]", "[BP + SP]", "[BP + DI]", "SI", "DI", "", "BX"},
    {"[BX + SI]", "[BX + DI]", "[BP + SP]", "[BP + DI]", "SI", "DI", "", "BX"},
    {"[BX + SI]", "[BX + DI]", "[BP + SP]", "[BP + DI]", "SI", "DI", "", "BX"}
};

void decode(unsigned char buffer[])
{
    unsigned char opcode = buffer[0] >> 2; 
    unsigned char dw = buffer[0] << 6;
    unsigned char d,w;
    // 11 011 001
    // 01 100 100
    unsigned char mod = buffer[1] & 11000000;
    unsigned char reg = (buffer[1] << 2 ) & 11100000;
    unsigned char rm = buffer[1] << 5;

    switch (dw)
    {   
        case 0x00:
            d = 0;
            w = 0;
            break;
        case 0x40:
            d = 0;
            w = 1;
            break;
        case 0x80:
            d = 1;
            w = 0;
            break;
        case 0xC0:
            d = 1;
            w = 1;
            break;
    }

    int m;
    switch (mod)
    {
        case 0x00:
            m = 0;
            break;
        case 0x40:
            m = 1;
            break;
        case 0x80:
            m = 2;
            break;
        case 0xC0:
            m = 3;
            break;
    }

    int reg_idx;
    switch (reg)
    {
        case 0x00:
            reg_idx = 0;
            break;
        case 0x20:
            reg_idx = 1;
            break;
        case 0x40:
            reg_idx = 2;
            break;
        case 0x60:
            reg_idx = 3;
            break;
        case 0x80:
            reg_idx = 4;
            break;
        case 0xA0:
            reg_idx = 5;
            break;
        case 0xC0:
            reg_idx = 6;
            break;
        case 0xE0:
            reg_idx = 7;
            break;
    }
    
    int rm_idx;
    switch (rm)
    {
        case 0x00:
            rm_idx = 0;
            break;
        case 0x20:
            rm_idx = 1;
            break;
        case 0x40:
            rm_idx = 2;
            break;
        case 0x60:
            rm_idx = 3;
            break;
        case 0x80:
            rm_idx = 4;
            break;
        case 0xA0:
            rm_idx = 5;
            break;
        case 0xC0:
            rm_idx = 6;
            break;
        case 0xE0:
            rm_idx = 7;
            break;
    }

    switch (opcode)
    {
        case 0x22: // MOV
            if (m == 3)
            {
                printf("MOV %s,%s\n",REG_TABLE[w][rm_idx],REG_TABLE[w][reg_idx]);
            }
            break;
    }

}


int main(int argc, char* argv[]) 
{
    unsigned char buffer[6];

    FILE* fptr = fopen(argv[1], "rb");
    int count = 0;
    while (!feof(fptr)) {
        fread(buffer, sizeof(unsigned char), 2, fptr);
        printf("%d %x %x\n", count, buffer[0], buffer[1]);
//        while (fgets(buffer, 6, fptr)) {
        decode(buffer);
        count += 1;
    }
     
    fclose(fptr);

    return 0;
}
