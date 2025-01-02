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

int main(int argc, char* argv[]) 
{
    unsigned char buffer[6];

    FILE* fptr = fopen(argv[1], "rb");
    FILE* diasm = fopen("disasm", "w");
    if (fptr) {
        fread(buffer, sizeof(buffer), 1, fptr);
    }

    unsigned char opcode = buffer[0] >> 2; 
    unsigned char dw = buffer[0] << 6;
    switch (opcode)
    {
        case 0x22:
            printf("MOV"); // prints a series of bytes
            break;
    }
    printf("1st byte: %x\n", buffer[0]);

    fclose(fptr);
    fclose(diasm);

    return 0;
}
