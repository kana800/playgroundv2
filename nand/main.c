#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

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
	char labelname[50];
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



int16_t createAInstruction(char buffer[], int startpoint, int endpoint)
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
		return ri;
	} else 
	{
		// @<number>
		char temp[98];
		memcpy(temp, &dest[1], endpoint);
		temp[endpoint] = '\0';

		int int_conv = atoi(temp);
		// @<LABEL>
		if ((int_conv == 0) && (strcmp(temp, "0") != 0))
		{
		} else 
		{
			return int_conv;
		}
	}
	return -1;
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
	fseek(fptr, 0L, SEEK_SET);

	char buffer[fsize];
	size_t bytesread_f = fread(buffer, sizeof(char), fsize, fptr);

	if (bytesread_f != fsize)
	{
		fprintf(stderr, 
			"short read of '%s': expected %d bytes\n", 
			argv[1], fsize);
		return -1;
	}

	struct label* bufferarray = malloc(sizeof(struct label) * 256);

	int instructiontype = -1;
	int instructionindex = -1;
	int endpoint = -1;

	int labelidx = -1;
	int commentidx =  -1;
	int atidx = -1; //@

	int line = 0;
	int labelcnt = 0;

	char temp[256];

	for (int i = 0; i < fsize; i++)
	{
		switch (buffer[i])
		{
			case '\n':
				if (commentidx != -1)
					commentidx = -1;
				line++;
				break;
			case '/':
				if (buffer[i + 1] == '/')
				{
					commentidx = i;
				}
				break;
			case ' ':
				if (commentidx != -1)
				{
					break;
				}
			case '(':
				if (commentidx == -1)
					labelidx = i;
				break;
			case ')':
				if ( labelidx != -1)
				{
					char* labeladdr = 
						bufferarray[labelcnt].labelname;
					int labelsize = i - labelidx;
					memcpy(labeladdr, &buffer[labelidx + 1], 
						labelsize - 1);
					labeladdr[labelsize] = '\0';
					bufferarray[labelcnt].line = line;
					bufferarray[labelcnt].pointer = labelidx;
					labelcnt += 1;
				}
				labelidx = -1;
				break;
			case '@':
				if (commentidx != -1) break;
				int j = i;
				for (;;)
				{
					if ((buffer[j] == ' ') ||
						(buffer[j] == '\n'))
					{
						break;
					}
					j++;
				}
				memcpy(temp, &buffer[i], j - i);
				temp[j - i] = '\0';
				i = j - 1;

				// @R<num>
				// @<num>
				// @label
				if (temp[1] == 'R' && isdigit(temp[2]))
				{
					printf("@R->%s\n",temp);
				} else if (isdigit(temp[1])) 
				{
					printf("@num->%s\n",temp);
				} else 
				{
					printf("@label->%s\n",temp);
				}
				break;
		}
	}

	fclose(fptr);

	return 0;
}
