#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

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

char* comp_bit_table[18] = {
	"101010",
	"111111",
	"111010",
	"001100",
	"110000",
	"001101",
	"110001",
	"011111",
	"110011",
	"011111",
	"110111",
	"001110",
	"110010",
	"000010",
	"010011",
	"000111",
	"000000",
	"010101"
};

char* comp_table[36] = {
	"0","1","-1","D",
	"A","!D","!A","-D",
	"-A","D+1","A+1","D-1",
	"A-1","D+A","D-A","A-D",
	"D&A","D|A","","","","",
	"M","","!M","","-M","",
	"M+1","","M-1","D+M",
	"D-M","M-D","D&M","D|M"
};


char* dest_jmp_table[8] = {
	"000", // null | the value is not stored
	"001", // M    | RAM[A]
	"010", // D    | D register
	"011", // MD   | RAM[A] and D register
	"100", // A    | A register
	"101", // AM   | A register and RAM[A]
	"110", // AD   | A register and D register
	"111", // AMD  | A register, RAM[A] and D register
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
				if (li != 0)
				{
					linebuffer[li] = '\0';
					tokenarray[tokencount].comp = 1;
					tokenarray[tokencount].type = 1;
					char* t = tokenarray[tokencount].binary;
					memcpy(t, linebuffer, li);
					t[li] = '\0';
					tokenarray[tokencount].line = line;
					tokenarray[tokencount].spoint = i;
					tokenarray[tokencount].len = li;
					tokencount += 1;
				}
				// resetting values;
				li = 0;
				atidx = -1;
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
				if (commentidx != -1) break;
				if (li == 0) break;
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
				atidx = i;
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
				if (atidx != -1) break;
				if (labelidx != -1) break;
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
		switch (t.type)
		{
			case 1:
				char cins[16];

				char* etp = strtok(t.binary, "=");
				// DESTINATION
				int res = strcmp(etp, t.binary);
				if (res != 0)
				{
					for ( int j = 0; j < 7; j++ )
					{
						if (strcmp(etp, dest_table[j]) == 0)
						{
							break;
						}
					}
					etp = strtok(NULL, "=");
				} else
				{
					// DESTINATION IS NULL
					etp = strtok(t.binary, ";");
				}
				// COMP
				for (int k = 0; k < 36; k++)
				{
					if (strcmp(etp, comp_table[k]) == 0)
					{
						if (k > 16) 
						{
							printf("\t%s\n",comp_bit_table[k - 16]); 
						} else 
						{
							printf("\t%s\n",comp_bit_table[k]); 
						}
						break;
					}
				}
				// JUMP
				char* jtp = strtok(t.binary, ";");
				printf("k %s\n",jtp);
				if (strcmp(jtp, t.binary) != 0)
				{
					jtp = strtok(NULL, ";");
					printf("k %s\n",jtp);
					for (int l = 0; l < 8; l++)
					{
						if (strcmp(jtp, jump_table[l]) == 0)
						{
							printf("\t%s\n",jump_table[l]);
						}
					}
				}

				break;
		}

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
