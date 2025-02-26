#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdint.h>

#define DEBUG 1 

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

char* RM_TABLE[3][8] = {
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
    },
    { 
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

int8_t GENERAL_REGISTERS[8][2] = {
    {0,0}, 
    {0,0}, 
    {0,0}, 
    {0,0},
    {0,0}, 
    {0,0}, 
    {0,0}, 
    {0,0}
};

typedef struct __opcode__ {
    int idx;
    int d;
    int w;
    int mod;
    int reg;
    int rm;
    int s;

    int16_t u_disp;
    int16_t data;
    
    int bytesread; //debug
} opcode; 

void simulateRegister()
{
//    printf("AH %x AL %x\nBH %x BL %x\nCH %x CL\
//        %x\nDH %x DL %x\nSP %x\nBP %x\nDI %x\nSI %x\n",
//           gr->AH, gr->AL, gr->BH, gr->BL, 
//           gr->CH, gr->CL, gr->DH, gr->DL,
//           gr->SP, gr->BP, gr->DI, gr->SI);
    printf("AX %x [ AH %x AL %x ]\nBX %x [ BH %x BL %x ]\
            \nCX %x [ CH %x CL %x ]\nDX %x [ DH %x DL %x ]\
            \nSP %x\nBP %x\nDI %x\nSI %x\n",
        GENERAL_REGISTERS[0][1], GENERAL_REGISTERS[4][0], GENERAL_REGISTERS[0][0],
        GENERAL_REGISTERS[3][1], GENERAL_REGISTERS[7][0], GENERAL_REGISTERS[3][0],
        GENERAL_REGISTERS[1][1], GENERAL_REGISTERS[5][0], GENERAL_REGISTERS[1][0],
        GENERAL_REGISTERS[2][1], GENERAL_REGISTERS[6][0], GENERAL_REGISTERS[2][0],
        GENERAL_REGISTERS[4][1], GENERAL_REGISTERS[5][1], GENERAL_REGISTERS[7][1],
        GENERAL_REGISTERS[6][1]);
    return;
}


void simulateInstruction(opcode op)
{
    // d = 1 instruction destination is specified
    // in reg field
    // d = 0 instruction source is specified in 
    // reg field
    // mov <dest> <source>
    switch (op.idx)
    {
        // mov instruction set
        case 0: // mov no displacement
            printf("mov %s,%s\n",
                   RM_TABLE[0][op.rm],
                   REG_TABLE[op.rm][op.w]);
            break;
        case 1: // mov direct addressing
            printf("mov %s,[%d]\n",
                   REG_TABLE[op.reg][op.w],
                   op.data);
            break;
        case 2: // mov no displacement d = 1
            printf("mov %s,%s\n",
                   REG_TABLE[op.rm][op.w],
                   RM_TABLE[0][op.rm]);
            break;
        case 3: // mov 8 bit displacement
        case 4: // mov 16 bit displacement
            printf("mov %s %d], %s \n",
                   RM_TABLE[1][op.rm],
                   op.u_disp,
                   REG_TABLE[op.reg][op.w]
                   );
            break;
        case 5: // mov 8 bit displacement d = 1
        case 6: // mov 16 bit displacement d = 1
            printf("mov %s, %s %d]\n",
                   REG_TABLE[op.reg][op.w],
                   RM_TABLE[1][op.rm],
                   op.u_disp
                   );
            break;
        case 7: // mov register mode d = 0
            printf("mov %s, %s\n", 
                   REG_TABLE[op.rm][op.w],
                   REG_TABLE[op.reg][op.w]);
            GENERAL_REGISTERS[op.rm][op.w] = 
                GENERAL_REGISTERS[op.reg][op.w];
            break;
        case 9: // mov register mode d = 1
            printf("mov %s, %s\n", 
                   REG_TABLE[op.reg][op.w],
                   REG_TABLE[op.rm][op.w]);
            GENERAL_REGISTERS[op.reg][op.w] = 
                GENERAL_REGISTERS[op.rm][op.w];
            break;
        case 10: // mov 8-bit immediate to register
        case 11: // mov 16-bit immediate to register
            printf("mov %s,%d\n", 
                   REG_TABLE[op.reg][op.w],
                   op.data);
            GENERAL_REGISTERS[op.reg][op.w] = op.data;
            break;
        case 12: // mov memory to accumulator 
            printf("mov %s,[%d]\n",
                   REG_TABLE[0][op.w],
                   op.data);
            break;
        case 13: // mov memory to accumlatero
            printf("mov [%d],%s\n",
                   op.data,
                   REG_TABLE[0][op.w]);
            break;
        // add, sub instruction set 
        // immediate to register/memory 
        case 20: // add (mod = 00, d = 0)
            printf("add %s,%s\n",
                   RM_TABLE[0][op.rm],
                   REG_TABLE[op.rm][op.w]);
            break;
        case 30: // sub (mod = 00, d = 0)
            printf("sub %s,%s\n",
                   REG_TABLE[op.reg][op.w],
                   RM_TABLE[op.rm][op.w]);
            break;
        case 22: // add (mod = 00, d = 1)
            printf("add %s,%s\n",
                   REG_TABLE[op.reg][op.w],
                   RM_TABLE[op.mod][op.rm]);
            break;
        case 32: // sub (mod = 00, d = 1)
            printf("sub %s,%s\n",
                   REG_TABLE[op.reg][op.w],
                   RM_TABLE[op.mod][op.rm]);
            break;
        case 24: // add (mod = 01, d = 0)
            printf("add %s,%s\n",
                   RM_TABLE[0][op.rm],
                   REG_TABLE[op.rm][op.w]);
            break;
        case 34: // sub (mod = 01, d = 0)
            printf("sub %s + %d],%s\n",
                   RM_TABLE[op.mod][op.rm],
                   op.u_disp,
                   REG_TABLE[op.rm][op.w]);
            break;
        case 26: // add (mod = 01, d = 1)
            printf("add %s,%s + %d]\n",
                    REG_TABLE[op.reg][op.w],
                    RM_TABLE[op.mod][op.rm],
                    op.u_disp
                   ); 
            break;
        case 36: // add (mod = 01, d = 1)
            printf("sub %s,%s + %d]\n",
                    REG_TABLE[op.reg][op.w],
                    RM_TABLE[op.mod][op.rm],
                    op.u_disp
                   ); 
            break;
        // add, sub, cmp instruction set
        // register/memory to register/either
        case 40: 
            printf("add byte %s, %d\n",
                   RM_TABLE[op.mod][op.rm],
                   op.data
                   );
            break;
        case 42:
            printf("add word %s + %d],%d\n",
                   RM_TABLE[op.mod][op.rm],
                   op.u_disp,
                   op.data);
            break;
        case 43:
            printf("add %s,%d\n",
                    REG_TABLE[op.rm][op.w],
                    op.data
                   ); 
            break;
        case 45:
            printf("sub byte %s, %d\n",
                   RM_TABLE[op.mod][op.rm],
                   op.data
                   );
            break;
        case 47:
            if (op.mod == 2)
            {
                printf("cmp word %d, %d\n",
                       op.u_disp, 
                       op.data);
                break;
            }
            printf("cmp byte %s, %d\n",
                   RM_TABLE[op.mod][op.rm],
                   op.data);
            break;
   //     case 108: // immediate from register/memory
   //         printf("add %s,%d\n",
   //                 REG_TABLE[op.rm][op.w],
   //                 op.data
   //                ); 
   //         break;
   //     case 110: // cmp register mode no displacement
   //         printf("cmp %s, %d\n",
   //                REG_TABLE[op.rm][op.w],
   //                op.data
   //                );
   //         break;
        case 50: //ADD | immediate to accumulator
            printf("add ax,%d\n",
                   op.data);
            break;
   //     case 300:
   //         printf("cmp %s,%s\n",
   //                RM_TABLE[op.mod][op.rm],
   //                REG_TABLE[op.reg][op.w]);
   //         break;
   //     case 301:
   //         printf("cmp %s,%s\n",
   //                RM_TABLE[op.mod][op.rm],
   //                REG_TABLE[op.reg][op.w]);
   //         break;
   //     case 302: // register to register
   //         printf("cmp %s,%s\n",
   //                REG_TABLE[op.reg][op.w],
   //                RM_TABLE[op.mod][op.rm]);
   //         break;
   //     case 303: // register to register
   //         printf("cmp %s,%s+%d]\n", 
   //                REG_TABLE[op.reg][op.w],
   //                RM_TABLE[op.mod][op.rm],
   //                op.u_disp);
   //         break;
   //     case 313: // register mode d = 0
   //         printf("cmp %s,%s\n",
   //                REG_TABLE[op.reg][op.w],
   //                REG_TABLE[op.rm][op.w]);
   //         break;
   //     case 314: // register mode d = 1
   //         printf("cmp %s,%s\n",
   //                REG_TABLE[op.rm][op.w],
   //                REG_TABLE[op.reg][op.w]);
   //         break;
   //     case 320: // CMP: immediate to accumulator
   //         printf("cmp ax,%d\n",op.u_disp);
   //         break;
   //     case 321: 
   //         printf("cmp al,%d\n",op.u_disp);
   //         break;
   //     case 517:
   //         printf("jnz %d\n", op.data);
   //         break;
   //     case 512:
   //         printf("js %d\n", op.data);
   //         break;
   //     case 526:
   //         printf("jb %d\n", op.data);
   //         break;
   //     case 514:
   //         printf("jbe %d\n", op.data);
   //         break;
   //     case 516:
   //         printf("je %d\n", op.data);
   //         break;
   //     case 520:
   //         printf("jne %d\n", op.data);
   //         break;
   //     case 522:
   //         printf("jo %d\n", op.data);
   //         break;
   //     case 521:
   //         printf("jno %d\n", op.data);
   //         break;
   //     case 518:
   //         printf("jp %d\n", op.data);
   //         break;
   //     case 524:
   //         printf("jle %d\n", op.data);
   //         break;
        default:
            printf("CANNOTDISASSEMBLE\n");
            break;
    }
}

opcode decodeInstruction(unsigned char buffer[], int bytesread)
{
    opcode oc = {.idx=-1, .d=-1, .w=-1, 
                .mod=-1, .reg=-1, .rm=-1, 
                .bytesread=0 };

    unsigned char reg;
    unsigned char dw;

    unsigned char opcode_a = buffer[bytesread] >> 2; 

    // immediate to register/memory
    unsigned char opcode_b = buffer[bytesread] >> 1;

    // mov memory to accumlator
    // mov accumlator to memory
    unsigned char opcode_d = buffer[bytesread];
    // add immediate to accumulator
    // add with carry immediate to accumulator
    // sub immediate to accumalator
    // sub with borrow immediate to accumulator
    unsigned char opcode_g = buffer[bytesread];

    // CMP | compare immediate with accumulator
    unsigned char opcode_i = buffer[bytesread];
    // JNZ/JNE | jump on not equal / not zro
    unsigned char opcode_j = buffer[bytesread];

    switch (opcode_a)
    {
        case 0x22: // MOV | register/memory to/from register
            // MOV 00-09
            // idx| mod| w | d |
            // 00 | 00 | 0 | 0 | memory mode no displacement
            // 02 | 00 | 0 | 1 |
            // 03 | 01 | 0 | 0 | memory mode 8 bit displacement
            // 05 | 01 | 0 | 1 |
            // 04 | 10 | 1 | 0 | memory mode 16 bit displacement
            // 06 | 10 | 1 | 1 |
            // 07 | 11 | 0 | 0 | register mode no displacement
            // 09 | 11 | 0 | 1 |
            // idx = 2 **R/M special case
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
            oc.idx = 7 + oc.d; // idx set to register mod by default
            switch (oc.mod)
            {
                case 0: // memory mode no displacement
                    oc.idx = 0 + oc.d;
                    if (oc.rm == 6) // special case direct addressing
                    {
                        oc.idx = 1;
                        int16_t temp = buffer[bytesread + 3] << 8 | 
                            buffer[bytesread + 2];
                        oc.data = temp;
                        oc.bytesread = 4;
                    } 
                    break;
                case 1: // memory mode 8 bit displacement
                    oc.idx = 3 + oc.d;
                    int16_t temp = buffer[bytesread + 2] & 0b0000000011111111;
                    if (buffer[bytesread + 2] & 0b0000000010000000) 
                        temp = temp | 0b1111111100000000;
                    // sign bit extension
                    oc.u_disp = temp;
                    oc.bytesread = 3;
                    break;
                case 2: // memory mode 16 bit displacement
                    oc.idx = 4 + oc.d; 
                    oc.u_disp = buffer[bytesread + 3] << 8 | buffer[bytesread + 2];
                    oc.bytesread = 4;
                    break;
            }
            break;
        // NOTE: immediate to register/memory in 
        // addition and substraction both start
        // with 0b1000000sw and reg value only
        // changes. check opcode_d below for the
        // implementation
        case 0x00: // ADD | register/memory & register to either
        case 0xa:  // SUB | register/memory & register to either
            // ADD 20-29 
            // idx| mod| w | d |
            // 20 | 00 | 0 | 0 | memory mode no displacement
            // 22 | 00 | 0 | 1 |
            // 24 | 01 | 0 | 0 | memory mode 8 bit displacement
            // 26 | 01 | 0 | 1 |
            // 25 | 10 | 1 | 0 | memory mode 16 bit displacement
            // 27 | 10 | 1 | 1 |
            // 28 | 11 | 0 | 0 | register mode no displacement
            // 29 | 11 | 0 | 1 |
            // SUB 30-39 
            // idx| mod| w | d |
            // 30 | 00 | 0 | 0 | memory mode no displacement
            // 32 | 00 | 0 | 1 |
            // 34 | 01 | 0 | 0 | memory mode 8 bit displacement
            // 36 | 01 | 0 | 1 |
            // 35 | 10 | 1 | 0 | memory mode 16 bit displacement
            // 37 | 10 | 1 | 1 |
            // 38 | 11 | 0 | 0 | register mode no displacement
            // 39 | 11 | 0 | 1 |
            oc.d = buffer[bytesread] & 0b00000010;
            oc.w = buffer[bytesread] & 0b00000001;
            // TODO: add a mod function
            oc.mod = buffer[bytesread + 1] >> 6; 
            oc.reg = (buffer[bytesread + 1] & 0b00111000) >> 3;
            oc.rm = (buffer[bytesread + 1] & 0b00000111);
            switch (oc.mod)
            {
                case 0x00: // no displacement
                    oc.idx = 20 + oc.d + opcode_a;
                    oc.bytesread = 2;
                    break;
                case 0x01: // 8 bit displacement
                    oc.idx = 24 + oc.d + opcode_a;
                    oc.u_disp = (int8_t)buffer[bytesread + 2];
                    oc.bytesread = 3;
                    break;
                case 0x02: // 16 bit displacement
                    oc.idx = 25 + oc.d + opcode_a;
                    oc.u_disp = buffer[bytesread + 3] << 8 | buffer[bytesread + 2];
                    oc.bytesread = 4;
                    break;
                case 0x03: // register mode
                    oc.idx = 28 + oc.d + opcode_a;
                    break;
            }
            break;
        // NOTE: 
        // immediate to register/memory
        // for addition, substraction and compare the
        // opcode is 100000sw
        // add      : reg 000
        // add carry: reg 010
        // sub      : reg 101
        // sub borrw: reg 011
        // cmp      : reg 111
        case 0x20: // ADD, SUB & CMP | immediate to register/memory
            oc.s = buffer[bytesread] & 0b00000010;
            oc.w = buffer[bytesread] & 0b00000001;
            oc.mod = buffer[bytesread + 1] >> 6; 
            oc.reg = (buffer[bytesread + 1] & 0b00111000) >> 3;
            oc.rm = (buffer[bytesread + 1] & 0b00000111);
            oc.idx = 40 + oc.reg + oc.mod;
            // addition oc.idx = 100;
            // addition w/ carry oc.idx = 102;
            // substraction oc.idx = 105;
            // substraction w/ borrow oc.idx = 103;
            // compare oc.idx = 107;
            if ((oc.rm == 6) && (oc.mod == 0)) oc.mod = 2;
            switch (oc.mod)
            {
                case 0x01: // 8 bit displacement
                    oc.u_disp = (int8_t)buffer[bytesread + 2];
                    oc.bytesread = 3;
                    break;
                case 0x02: // 16 bit displacement
                    oc.u_disp = buffer[bytesread + 3] << 8 | buffer[bytesread + 2];
                    if ((oc.s == 1) && (oc.w == 1)) 
                    { 
                        oc.data = buffer[bytesread + 5] << 8 | buffer[bytesread + 4];
                        oc.bytesread = 6;
                    } else {
                        oc.data = (int8_t)buffer[bytesread + 4];
                        oc.bytesread = 5;
                    }
                    break;
                case 0x00:
                case 0x03: // register mode no displacement
                    if ((oc.s == 1) && (oc.w == 1)) 
                    { 
                        oc.data = buffer[bytesread + 3] << 8 | buffer[bytesread + 2];
                        oc.bytesread = 4;
                    } else {
                        oc.data = (int8_t)buffer[bytesread + 2];
                        oc.bytesread = 3;
                    }
                    break;
            }
            break;
        case 0xE: // CMP | register/memory and register
            oc.idx = 300;
            oc.d = buffer[bytesread] & 0b00000010;
            oc.w = buffer[bytesread] & 0b00000001;
            oc.mod = buffer[bytesread + 1] >> 6; 
            oc.reg = (buffer[bytesread + 1] & 0b00111000) >> 3;
            oc.rm = (buffer[bytesread + 1] & 0b00000111);
            switch (oc.mod)
            {
                case 0x00:
                    oc.idx += oc.d + oc.mod;
                    oc.bytesread = 2;
                    break;
                case 0x01: // 8 bit displacement
                    oc.idx += oc.d + oc.mod;
                    oc.u_disp = (int8_t)buffer[bytesread + 2];
                    oc.bytesread = 3;
                    break;
                case 0x02: // 16 bit displacement
                    oc.idx += oc.d + oc.mod;
                    oc.u_disp = buffer[bytesread + 3] << 8 | buffer[bytesread + 2];
                    oc.bytesread = 4;
                    break;
                case 0x03: // register mode
                    oc.idx += 10 + oc.d + oc.mod;
                    oc.bytesread = 2;
                    break;
            }
            break;
    }

    // MOV | immediate to register 
    unsigned char opcode_c = buffer[bytesread] >> 3;
    switch (opcode_c)
    {
        case 0x16: // immediate to register 8 bit 
            oc.idx = 10;
            oc.reg = (buffer[bytesread] & 0b00000111);
            oc.w = 0;
            oc.data = (int8_t)buffer[bytesread + 1];
            oc.bytesread = 2;
            break;
        case 0x17: // immediate to register 16 bit
            oc.idx = 11;
            oc.reg = (buffer[bytesread] & 0b00000111);
            oc.w = 1;
            oc.data = buffer[bytesread + 2] << 8 | buffer[bytesread + 1]; 
            oc.bytesread = 3;
            break;
    }

    // immediate to register/memory
    switch (opcode_b)
    {
        case 0x63: 
            oc.idx = 2;
            oc.w = buffer[bytesread] & 0b00000001;
            // second byte [data]
            oc.mod = buffer[bytesread + 1] >> 6; 
            oc.d = 1;
            oc.reg = 0;
            oc.rm = buffer[bytesread + 1] & 0b00000111;
            oc.bytesread = 2;
            switch (oc.mod)
            {
                case 0: // no displacement
                    oc.data = (int8_t) buffer[bytesread + 2];
                    oc.bytesread = 3;
                    break;
                case 2: // 16 bit displacement
                    oc.u_disp = buffer[bytesread + 3] << 8 | buffer[bytesread + 2];
                    oc.data = buffer[bytesread + 5] << 8 | buffer[bytesread + 4];
                    oc.bytesread = 6;
                    break;
            }
            break;
    }

    // mov memory to accumlator
    // mov accumlator to memory
    switch (opcode_d)
    {
        case 0xA0: // mov memory to accumlator
            oc.idx = 12;
            oc.reg = -1;
            oc.d = -1;
            oc.w = 0;
            oc.data = (int8_t) buffer[bytesread + 1];
            oc.bytesread = 2;
            break;
        case 0xA1:
            oc.idx = 12;
            oc.reg = -1;
            oc.d = -1;
            oc.w = 1;
            oc.data = buffer[bytesread + 2] << 8 | buffer[bytesread + 1]; 
            oc.bytesread = 3;
            break;
        case 0xA2:
            oc.idx = 13;
            oc.reg = -1;
            oc.w = 0;
            oc.data = (int8_t) buffer[bytesread + 1];
            oc.bytesread = 2;
            break;
        case 0XA3:
            oc.idx = 13;
            oc.reg = -1;
            oc.d = -1;
            oc.w = 1;
            oc.data = buffer[bytesread + 2] << 8 | buffer[bytesread + 1]; 
            oc.bytesread = 3;
            break;
        // sub immediate to accumalator
        // sub with borrow immediate to accumulator
        case 0x04: // add immediate to accumulator
        case 0x05: // add with carry immediate to accumulator
            oc.idx = 50;
            oc.w = buffer[bytesread] & 0b00000001;
            if (oc.w == 1)
            {
                oc.data = buffer[bytesread + 2] << 8 | buffer[bytesread + 1]; 
                oc.bytesread = 3;
            } else 
            {
                oc.data = (int8_t)buffer[bytesread + 1]; 
                oc.bytesread = 2;
            }
            break;
    }


    //// CMP | compare immediate with accumulator
    //unsigned char opcode_i = buffer[bytesread];
    //switch (opcode_i)
    //{
    //    case 0x3c:
    //        oc.w = 0;
    //        oc.idx = 320;
    //        oc.u_disp = (int8_t)buffer[bytesread + 1];
    //        oc.bytesread = 2;
    //        break;
    //    case 0x3d:
    //        oc.w = 1;
    //        oc.idx = 321;
    //        oc.u_disp = buffer[bytesread + 2] << 8 | buffer[bytesread + 1];
    //        oc.bytesread = 3;
    //        break;
    //}

    //// JNZ/JNE | jump on not equal / not zro
    //unsigned char opcode_j = buffer[bytesread];
    //switch (opcode_j)
    //{
    //    case 0x70: // JO
    //    case 0x7e: // JB
    //    case 0x72: // JBE
    //    case 0x74: // JE
    //    case 0x75: // JNZ/JNE
    //    case 0x7c: // JLE
    //    case 0x76: // JP
    //    case 0x7a: // JS
    //    case 0x78: // JNE
    //    case 0x79: // JNO
    //        oc.idx = 400 + buffer[bytesread];
    //        oc.data = (int8_t)buffer[bytesread + 1];
    //        oc.bytesread = 2;
    //        break;
    //}

    assert(oc.idx != -1);
    return oc;
}


int main(int argc, char* argv[]) 
{

    int option;
    
    FILE* fptr = fopen(argv[1], "rb");

    if (!fptr) {
        fprintf(stdout, "usage: 'sim.out <filename>'\n");
        return -1;
    }

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
    int bytesread = 0;

    printf("bits 16\n");

    while (len > bytesread)
    {
        oc = decodeInstruction(buffer, bytesread);
        simulateInstruction(oc);
        bytesread = bytesread + oc.bytesread;
    }

    fclose(fptr);
    free(buffer);

    return 0;
}
