// write a program to translate C Declarations into English
// page (78/290)
// Here's the design. The main datastructure is a stack, on
// which we store tokens that we have read, while we are reading
// forward to the identifier. Then we can look at the next token to
// the right by reading it, and the next token to the left by popping
// it off the stack. The data structure looks like:
// 
// struct token { char type; 
// 		 char string[MAXTOKENLEN]; };
// /*holds token we read before reaching first identifier*/
// struct token stack[MAXTOKENS];
// /*holds the token just read*/
// struct token this;

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKENLEN 64
#define MAXTOKENS 100

struct token {
	char type;
	char string[MAXTOKENLEN];
};

enum type_tag { IDENTIFIER, QUALIFIER, TYPE };

struct token stack[MAXTOKENS];
struct token this;
int top = -1;

#define pop stack[top--]
#define push(s) stack[++top]=s

// look at the current token and 
// return a value of "type" "qualifier" 
// or "identifier"
enum type_tag classify_string() {
	char *s = this.string;	
	if (!strcmp(s, "const")) {
		strcpy(s, "read-only");
		return QUALIFIER;
	}

	if (!strcmp(s,"volatile")) return QUALIFIER;
	if (!strcmp(s,"void")) return TYPE;
	if (!strcmp(s,"char")) return TYPE;
	if (!strcmp(s,"signed")) return TYPE;
	if (!strcmp(s,"unsigned")) return TYPE;
	if (!strcmp(s,"short")) return TYPE;
	if (!strcmp(s,"int")) return TYPE;
	if (!strcmp(s,"long")) return TYPE;
	if (!strcmp(s,"float")) return TYPE;
	if (!strcmp(s,"double")) return TYPE;
	if (!strcmp(s,"struct")) return TYPE;
	if (!strcmp(s,"union")) return TYPE;
	if (!strcmp(s,"enum")) return TYPE;
	return IDENTIFIER;
}

// read the next token into this.string
// if its alphanumeric, classify_string
// else it must be a single character token
// this.type = the token itself; terminate this.string
// with a nul.
void gettoken(void) {
	char *p = this.string;

	while ((*p = getchar()) == ' ');
	if (isalnum(*p)) {
		while (isalnum(*++p=getchar()));
		ungetc(*p, stdin);
		
		*p = '\0';
		this.type = classify_string();
		return;
	}

	if (*p=="*") {
		strcpy(this.string, "pointer to");
		this.type = "*";
		return;
	}

	this.string[1] = '\0';
	this.type = *p;
	return;
}

// gettoken and push it onto the stack
// until the first identifier is read
// print "identifier is", this.string gettoken
void read_to_first_identifier() {
	gettoken();
	while (this.type != IDENTIFIER) {
		push(this);
		gettoken();
	}
	printf("%s is ", this.string);
	gettoken();
}

// read past closing ')' print out "function returning"
void deal_with_function_args() {
	while (this.type != ')') {
		gettoken();
	}
	gettoken();
	printf("function returning ");
}

// while you got "[size]" print it out and read past it
void deal_with_arrays() {
	while (this.type=='[') {
		printf("array ");
		gettoken(); // a number of ']'
		if (isdigit(this.string[0])) {
			printf("0..%d", atoi(this.string) - 1);
			gettoken(); // read the ']'
		}
		gettoken();
		printf("of ");
	}
}

// while you got "*" on the stack print "pointer to" and 
// pop it
void deal_with_pointers() {
	while (stack[top].type == '*') {
		printf("%s ",pop.string);
	}
}

// if this.type is '[' deal_with_arrays
// if this.type is '(' deal_with_function_args
// deal_with_any_pointers while there's stuff on the 
// stack if it's a '(' pop it and gettoken; it shoudl be closing ')'
// deal_with_declarator
// else pop it and print it
void deal_with_declarator() {
	switch (this.type) {
		case '[' : deal_with_arrays(); break;
		case '(' : deal_with_function_args();
	}

	deal_with_pointers();

	while ( top >= 0 ) {
		if (stack[top].type == '(') {
			pop;
			gettoken();
			deal_with_declarator();
		} else {
			printf("%s ", pop.string);
		}
	}
}




void main() {
	// read to first identifier
	// deal with declarator
	read_to_first_identifier();
	deal_with_declarator();
	printf("\n");
	return;
}

















