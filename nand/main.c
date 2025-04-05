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

char* tokentype[] = {
	"AINSTRUCTION",
	"CINSTRUCTION",
	"LINSTRUCTION",
	"NUMBER",
	"LABEL",
	"LOOP",
	"JUMP"
};

struct token
{
	int comp;

	int type;
	char binary[16];

	int line;
	int spoint;
	int len;
};

struct label
{
	char labelname[50];
	int line;
	int pointer;
};

struct pair
{
	int x;
	int y;
};

char* ram_reg[] = {
        "000000000000000",
        "000000000000001",
        "000000000000010",
        "000000000000011",
        "000000000000100",
        "000000000000101",
        "000000000000110",
        "000000000000111",
        "000000000001000",
        "000000000001001",
        "000000000001010",
        "000000000001011",
        "000000000001100",
        "000000000001101",
        "000000000001110",
        "000000000001111"
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

void itob(char** b, int num)
{
	char* temp = *b;
	int bit;
	for (int i = 15; i >= 0; i--) {
		bit = (num >> i) & 1;
		if (bit == 1) 
		{
			temp[15 - i] = '1';
		} else {
			temp[15 - i] = '0';
		}
	}
}

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
	int labelidx = -1; // (label) 
	int labelcnt = 0;

	int commentidx =  -1; // //
	int atidx = -1; // @

	int line = 0;
	char linebuffer[256];
	int li = 0;

	// stores the line number and
	// the index of the labels encounter
	// x -> starting point
	// y -> ending point
	struct pair labellocation[200] = {};
	int labelcall = 0;

	struct token tokenarray[200] = {};
	int tokencount = 0;

	char temp[256];

	for (int i = 0; i < fsize; i++)
	{
		switch (buffer[i])
		{
			case '\n':
				if (commentidx != -1)
					commentidx = -1;
				li = 0;
				line++;
				break;
			case '/':
				if (buffer[i + 1] == '/')
				{
					commentidx = i;
				}
				break;
			case ' ':
				if (commentidx != -1) break;
				li = 0;
				break;
			case '=':
				if (commentidx != -1) break;
				if (li == 0) break;
				linebuffer[li] = '\0';
				printf("line buffer %s\n", linebuffer);
				break;
			case '(':
				if (commentidx == -1)
				{
					labelidx = i;
				}
				break;
			case ')':
				if ( labelidx != -1)
				{
					char* labeladdr = 
						bufferarray[labelcnt].labelname;
					int labelsize = i - labelidx;
					labeladdr[0] = '@';
					memcpy(&labeladdr[1], &buffer[labelidx + 1], 
						labelsize - 1);
					labeladdr[labelsize + 1] = '\0';
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
				//@R<AAA>
				memcpy(temp, &buffer[i], j - i);
				temp[j - i] = '\0';
				// @R<num>
				// @<num>
				// @label
				if (temp[1] == 'R' && isdigit(temp[2]))
				{
					int val = atoi(&temp[2]);
					printf("@R->%s %d %s\n",
							temp, val, ram_reg[val]);
					tokenarray[tokencount].comp = 1;
					tokenarray[tokencount].type = 0;
					char* t = tokenarray[tokencount].binary;
					memcpy(t, ram_reg[val],15);
					t[16] = '\0';

					tokenarray[tokencount].line = line;
					tokenarray[tokencount].spoint = i;
					tokenarray[tokencount].len = j - i;
					tokencount += 1;

				} else if (isdigit(temp[1])) 
				{
					int val = atoi(&temp[2]);
					tokenarray[tokencount].comp = 1;
					tokenarray[tokencount].type = 3;
					char* t = tokenarray[tokencount].binary;
					itob(&t, val);
					t[16] = '\0';
					printf("%d -> %s\n", val, t);
					tokenarray[tokencount].line = line;
					tokenarray[tokencount].spoint = i;
					tokenarray[tokencount].len = j - i;
					tokencount += 1;
				} else 
				{
					labellocation[labelcall].x = i;
					labellocation[labelcall].y = j - i;
					labelcall += 1;
				}
				i = j - 1;
				break;
			default:
				if (commentidx != -1) break;
				linebuffer[li] = buffer[i];
				li += 1;
				break;
		}
	}

	for (int i = 0; i < tokencount; i++)
	{
		struct token t = tokenarray[i];
		int x = t.spoint;
		int y = t.len;
		fprintf(stdout,"%d:%d %s->%s\n",
			t.line, t.spoint, 
			tokentype[t.type], t.binary);
	}

	for ( int i = 0; i < labelcall; i++)
	{
		int x = labellocation[i].x;
		int y = labellocation[i].y;
		memcpy(temp,&buffer[x], y);
		temp[y] = '\0';
		for ( int j = 0; j < labelcnt; j++)
		{
			char* labeladdr = bufferarray[j].labelname;
			if (strcmp(temp,labeladdr) == 0)
			{
				printf("%s -> %d\n",labeladdr, 
						bufferarray[j].line);
				break;
			}
		}
	}

	fclose(fptr);

	return 0;
}
