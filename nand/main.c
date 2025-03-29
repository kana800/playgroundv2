#include <stdio.h>
#include <string.h>

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
			fprintf(stdout, "%s == %s\n", d, jump_table[i]);
			break;
		}
	}
	return i;
}

int main(int argc, char* argv[])
{
	int i  = getJumpIndex("null");
	printf("%d\n", i);
	return 0;
}
