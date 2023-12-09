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

#define MAXTOKENLEN 50;
#define MAXTOKENS 10;

struct token {
	char type;
	char string[MAXTOKENLEN];
}

struct token stack[MAXTOKENS];
struct token this;

// look at the current token and 
// return a value of "type" "qualifier" 
// or "identifier"
char classify_string(struct token t) {
	
}

// read the next token into this.string
// if its alphanumeric, classify_string
// else it must be a single character token
// this.type = the token itself; terminate this.string
// with a nul.
void gettoken(struct token t) {
	
}

// gettoken and push it onto the stack
// until the first identifier is read
// print "identifier is", this.string gettoken
void read_to_first_identifier(char* declaration) {

}

// read past closing ')' print out "function returning"
void deal_with_function_args() {

}

// while you got "[size]" print it out and read past it
void deal_with_arrays() {

}

// while you got "*" on the stack print "pointer to" and 
// pop it
void deal_with_any_pointer() {

}

// if this.type is '[' deal_with_arrays
// if this.type is '(' deal_with_function_args
// deal_with_any_pointers while there's stuff on the 
// stack if it's a '(' pop it and gettoken; it shoudl be closing ')'
// deal_with_declarator
// else pop it and print it
void deal_with_declartor() {

}




void main() {
	// read to first identifier
	// deal with declarator
}

















