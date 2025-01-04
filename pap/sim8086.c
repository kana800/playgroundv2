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

char* REG_W_0[8] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "AH"};
char* REG_W_1[8] = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};


int main(int argc, char* argv[]) 
{
    unsigned char buffer[6];

    FILE* fptr = fopen(argv[1], "rb");
    if (fptr) {
        fread(buffer, sizeof(buffer), 1, fptr);
    }

    unsigned char opcode = buffer[0] >> 2; 
    unsigned char dw = buffer[0] << 6;
    unsigned char d,w;
    // 11 011 001
    // 01 100 100
    unsigned char mod = buffer[1] & 11000000;
    unsigned char reg = (buffer[1] << 2 ) & 11100000;
    unsigned char rm = buffer[1] << 5;
    printf("%x %x", buffer[0], buffer[1]);
    printf("buffer1 %x", buffer[1]);
    printf("mod %x reg %x\n", mod, reg);

    switch (opcode)
    {
        case 0x22:
            printf("MOV\n"); // prints a series of bytes
            break;
    }

    switch (dw)
    {   
        case 0x00:
            printf("8 bits is being transferred from the register (D=0,W=0)\n");
            d = 0;
            w = 0;
            break;
        case 0x40:
            printf("16 bits is being transferred from the register (D=0,W=1)\n");
            d = 0;
            w = 1;
            break;
        case 0x80:
            printf("8 bits is being transferred to the register (D=1,W=0)\n");
            d = 1;
            w = 0;
            break;
        case 0xC0:
            printf("16 bits is being transferred to the register (D=1,W=1)");
            d = 1;
            w = 1;
            break;
    }

    switch (mod)
    {
        case 0x00:
            printf("memory mode with no displacement\n");
            break;
        case 0x40:
            printf("memory mode with 8 bit displacement\n");
            break;
        case 0x80:
            printf("memory mode with 16 bit displacement\n");
            break;
        case 0xC0:
            printf("register mode\n");
            break;
    }

    int idx;
    switch (reg)
    {
        case 0x00:
            idx = 0;
            break;
        case 0x20:
            idx = 1;
            break;
        case 0x40:
            idx = 2;
            break;
        case 0x60:
            idx = 3;
            break;
        case 0x80:
            idx = 4;
            break;
        case 0xA0:
            idx = 5;
            break;
        case 0xC0:
            idx = 6;
            break;
        case 0xE0:
            idx = 7;
            break;
    }
    
    switch (rm)
    {
        case 0x00:
            idx = 0;
            break;
        case 0x20:
            idx = 1;
            break;
        case 0x40:
            idx = 2;
            break;
        case 0x60:
            idx = 3;
            break;
        case 0x80:
            idx = 4;
            break;
        case 0xA0:
            idx = 5;
            break;
        case 0xC0:
            idx = 6;
            break;
        case 0xE0:
            idx = 7;
            break;
    }
     

    fclose(fptr);

    return 0;
}
