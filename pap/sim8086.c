#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdint.h>

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

#define DEBUG 1 

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
    { // lmao i will do the closing bracket
      // in the printf function. 
        "[bx + si", 
        "[bx + di", 
        "[bp + si", 
        "[bp + di",
        "[si", 
        "[di", 
        "[bp", 
        "[bx"
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

//    unsigned char disp_lo[2];
//    unsigned char disp_hi[2];
    int16_t u_disp;

    unsigned char addr_lo;
    unsigned char addr_hi;

    int16_t data;

    int bytesread; //debug
} opcode; 


void printInstruction(opcode op)
{
#if DEBUG
//    printf("mod %d\nd %d\nw %d\nreg %d\nrm %d\n", 
//           op.mod, op.d, op.w, op.reg, op.rm);
#endif


    // d = 1 instruction destination is specified
    // in reg field
    // d = 0 instruction source is specified in 
    // reg field
    // mov <dest> <source>

    switch (op.idx)
    {
        case 2: // immediate to register
            printf("mov %s,%d\n", 
                   REG_TABLE[op.reg][op.w],
                   op.data);
            break;
        case 0:
            // register to register instruction
            switch (op.mod)
            {
                case 0:
                    if (op.d == 0)
                    {
                        printf("mov %s,%s\n",
                               RM_TABLE[0][op.rm],
                               REG_TABLE[op.rm][op.w]);
                    } else {
                        printf("mov %s,%s\n",
                               REG_TABLE[op.rm][op.w],
                               RM_TABLE[0][op.rm]);
                    }
                    break;
                case 1:
                case 2:
                    if (op.d == 0)
                    {
                        printf("mov %s %d], %s \n",
                               RM_TABLE[1][op.rm],
                               op.u_disp,
                               REG_TABLE[op.reg][op.w]
                               );
                    } else {
                        printf("mov %s, %s %d]\n",
                               REG_TABLE[op.reg][op.w],
                               RM_TABLE[1][op.rm],
                               op.u_disp
                               );
                    }
                    break;
                case 3:
                    printf("mov %s,%s\n",
                           REG_TABLE[op.rm][op.w],
                           REG_TABLE[op.reg][op.w]);
                    break;
            } 
            break;
    }
}

opcode decodeInstruction(unsigned char buffer[], int bytesread)
{
    opcode oc = {.idx=-1, .d=-1, .w=-1, .mod=-1, 
        .reg=-1, .rm=-1, .bytesread=0};

    unsigned char reg;
    unsigned char dw;


    unsigned char opcode_a = buffer[bytesread] >> 2; 
    switch (opcode_a)
    {
        case 0x22: // mov register/memory to/from register
            oc.idx = 0;
            oc.d = buffer[bytesread] & 0b00000010;
            oc.w = buffer[bytesread] & 0b00000001;
            // second byte
            oc.mod = buffer[bytesread + 1] >> 6; 
            oc.reg = (buffer[bytesread + 1] & 0b00111000) >> 3;
            oc.rm = (buffer[bytesread + 1] & 0b00000111);
            // checking if optional bytes are needed to
            // be read (disp lo) (disp hi); 
            // mod == 01 +D8
            // mod == 10 +D16
            oc.bytesread = 2;
            if (oc.mod == 1)
            {
                int16_t temp = buffer[bytesread + 2] & 0b0000000011111111;
                if (buffer[bytesread + 2] & 0b0000000010000000) 
                    temp = temp | 0b1111111100000000;
                // sign bit extension
                oc.u_disp = temp;
                oc.bytesread = 3;
            } 
            else if (oc.mod == 2)
            {
                oc.u_disp = buffer[bytesread + 3] << 8 | buffer[bytesread + 2];
                oc.bytesread = 4;
            }
            break;
    }

    // immediate to register/memory
    unsigned char opcode_b = buffer[bytesread] >> 1;
    switch (opcode_b)
    {
        case 0x63: 
            oc.idx = 2;
            oc.w = buffer[bytesread] & 0b00000001;
            // second byte [data]
            oc.mod = buffer[bytesread + 1] >> 6; 
            oc.reg = 0;
            oc.rm = buffer[bytesread + 1] & 0b00000111;
            oc.bytesread = 2;
            break;
    }

    unsigned char opcode_c = buffer[bytesread] >> 3;
    switch (opcode_c)
    {
        case 0x16: // mov immediate to register
            oc.idx = 2;
            oc.reg = (buffer[bytesread] & 0b00000111);
            oc.d = -1;
            oc.w = 0;
            // second byte [data]
            int8_t temp = buffer[bytesread + 1];
//            oc.data = 0xFF << 8 | buffer[bytesread + 1];
            oc.data = temp;
            oc.bytesread = 2;
            break;
        case 0x17:
            oc.idx = 2;
            oc.reg = (buffer[bytesread] & 0b00000111);
            oc.d = -1;
            oc.w = 1;
            oc.data = buffer[bytesread + 2] << 8 | buffer[bytesread + 1]; 
            // second byte [data]
            // oc.data[0] = buffer[bytesread + 1];
            // oc.data[1] = buffer[bytesread + 2];
            oc.bytesread = 3;
            break;
    }
    assert(oc.idx != -1);
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
