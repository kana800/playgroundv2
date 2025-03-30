#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void createAInstruction(char buffer[], int instructiontype,
		       int startingpoint, int commentindex)
{
	// sanitize the lines ie remove comments
	// disassemble a instruction
	char dest[100];
	int len = commentindex - 1;
	memcpy(dest, &buffer[startingpoint], commentindex);
	dest[len] = '\0';
	fprintf(stdout, "(CI)\tsantized->%s\n", dest);
	switch (instructiontype)
	{
		case 0:
			break;
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
	
	int instructiontype = 0;
	int instructionindex = -1;
	int commentindex = -1;

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
						fprintf(stdout, 
							"%d:%d %s\n\tline->%s\n",
							linenumber,i,tokens[6],buffer); 
						commentindex = i;
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
					fprintf(stdout, "%d:%d %s\n\tline->%s",
					linenumber, i, tokens[0],buffer);
					instructiontype = 0;
					instructionindex = i;
					break;
				case '\n':
					commentindex = i;
					i = len;
					break;
			}
		}
		if (instructiontype != -1)
			createInstruction(buffer, instructiontype,
			    instructionindex, commentindex);
		
		instructiontype = -1;
		commentindex = -1;

		linenumber += 1;
	}

	fclose(fptr);

	int i  = getJumpIndex("null");
	return 0;
}
