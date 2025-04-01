#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct bit3
{
	int d1:1;
	int d2:1;
	int d3:1;
};

struct bit6
{
	int c1:1;
	int c2:1;
	int c3:1;
	int c4:1;
	int c5:1;
	int c6:1;
};

char* tokens[] = {
	"AINSTRUCTION",
	"CINSTRUCTION",
	"LINSTRUCTION",
	"LOOP",
	"JUMP",
	"NEWLINE",
	"COMMENT"
};

struct label
{
	char* labelname[50];
	int line;
	int pointer;
};

char* ram_reg[] = {
        "0000000000000000",
        "0000000000000001",
        "0000000000000010",
        "0000000000000011",
        "0000000000000100",
        "0000000000000101",
        "0000000000000110",
        "0000000000000111",
        "0000000000001000",
        "0000000000001001",
        "0000000000001010",
        "0000000000001011",
        "0000000000001100",
        "0000000000001101",
        "0000000000001110",
        "0000000000001111"
};

// null | the value is not stored
// M    | RAM[A]
// D    | D register
// MD   | RAM[A] and D register
// A    | A register
// AM   | A register and RAM[A]
// AD   | A register and D register
// AMD  | A register, RAM[A] and D register
char* dest_table[8] = { 
	"null", 
	"M", 
	"D", 
	"MD", 
	"A", 
	"AM", 
	"AD", 
	"AMD" 
};

// null | no jump
// JGT  | if out > 0 jump
// JEQ  | if out = 0 jump
// JGE  | if out >= 0 jump
// JLT  | if out < 0 jump
// JNE  | if out != 0 jump
// JLE  | if out <= 0 jump
// JMP  | unconditional jump
char* jump_table[8] = { 
	"null", 
	"JGT", 
	"JEQ", 
	"JGE", 
	"JLT", 
	"JNE", 
	"JLE"
	"JMP"
};

struct bit6 comp_bit_table[18] = {
	{1,0,1,0,1,0},
	{1,1,1,1,1,1},
	{1,1,1,0,1,0},
	{0,0,1,1,0,0},
	{1,1,0,0,0,0},
	{0,0,1,1,0,1},
	{1,1,0,0,0,1},
	{0,1,1,1,1,1},
	{1,1,0,0,1,1},
	{0,1,1,1,1,1},
	{1,1,0,1,1,1},
	{0,0,1,1,1,0},
	{1,1,0,0,1,0},
	{0,0,0,0,1,0},
	{0,1,0,0,1,1},
	{0,0,0,1,1,1},
	{0,0,0,0,0,0},
	{0,1,0,1,0,1}
};

char* comp_a_table[18][2] = {
	{"0",""},
	{"1",""},
	{"-1",""},
	{"D",""},
	{"A","M"},
	{"!D",""},
	{"!A", "!M"},
	{"-D",""},
	{"-A","-M"},
	{"D+1",""},
	{"A+1","M+1"},
	{"D-1",""},
	{"A-1","M-1"},
	{"D+A","D+M"},
	{"D-A","D-M"},
	{"A-D","M-D"},
	{"D&A","D&M"},
	{"D|A","D|M"}
};


struct bit3 dest_jmp_table[8] = {
	{0, 0, 0}, // null | the value is not stored
	{0, 0, 1}, // M    | RAM[A]
	{0, 1, 0}, // D    | D register
	{0, 1, 1}, // MD   | RAM[A] and D register
	{1, 0, 0}, // A    | A register
	{1, 0, 1}, // AM   | A register and RAM[A]
	{1, 1, 0}, // AD   | A register and D register
	{1, 1, 1}, // AMD  | A register, RAM[A] and D register
};

int getDestIndex(char* d)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		if (strcmp(dest_table[i], d) == 0)
			break;
	}
	return i;
}

int getJumpIndex(char* d)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		if (strcmp(jump_table[i], d) == 0)
		{
			break;
		}
	}
	return i;
}

void createAInstruction(char buffer[], int startpoint, int endpoint)
{
	// sanitize the lines ie remove comments
	// disassemble a instruction
	char dest[100];
	int len = endpoint;
	memcpy(dest, &buffer[startpoint], endpoint);
	dest[len] = '\0';

	assert(dest[0] == '@');

	// @R
	char tempreg[2];
	memcpy(tempreg, dest, 2);
	tempreg[2] = '\0';

	// @R0 - @R15
	if ((strcmp(tempreg,"@R") == 0) && (len <= 5))
	{
		char intreg[2];
		memcpy(intreg, &dest[2], len);
		intreg[len] = '\0';
		int16_t ri = (int16_t)atol(intreg);
	} else 
	{
		// @<number>
		char tempnum[6];
		memcpy(tempnum, &dest[1], endpoint);
		tempnum[endpoint] = '\0';
		printf("number %s", tempnum);

		// @<LABEL>
	}
	return;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "usage: assembler.out <filename.asm>\n");
		return -1;
	}

	FILE* fptr = fopen(argv[1], "r");
	

	if (!fptr)
	{
		fprintf(stderr, "error reading the file '%s'\n",argv[1]);
		return -1;
	}

	fseek(fptr, 0L, SEEK_END);
	int fsize = ftell(fptr);
	unsigned char* symboltable = 
		malloc(sizeof(unsigned char) * fsize);
	fseek(fptr, 0L, SEEK_SET);

	const unsigned int len = 256;
	int linenumber = 0;
	char buffer[len];
	
	int instructiontype = -1;
	int instructionindex = -1;
	int endpoint = -1;

	while (fgets(buffer, len, fptr))
	{
		int i; // bufferindex
		for (i = 0; i < len; i++)
		{
			switch (buffer[i])
			{
				case '/': // COMMENT
					if (buffer[i + 1] == '/')
					{
						endpoint = i;
						i = len;
					} 
					else 
					{
						fprintf(stderr, 
							"%d:%d /< incomplete comment", 
							linenumber, i);
					}
					break;
				case '@':
					instructiontype = 0;
					instructionindex = i;
					break;
				case ' ':
					if (instructiontype > 0)
					{
						endpoint = i;
						i = len;
					}
					break;
				case '\n':
					endpoint = i;
					i = len;
					break;
			}
		}
		fprintf(stdout, "%d %s %d\n", linenumber, buffer,instructiontype);
		switch (instructiontype)
		{
			case 0: // A INSTRUCTION
				createAInstruction(buffer, instructionindex, endpoint);
				break;
		}
		instructiontype = -1;
		endpoint = -1;

		linenumber += 1;
	}

	fclose(fptr);

	return 0;
}
