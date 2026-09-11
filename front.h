#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 30

/* Token codes */
#define INT_LIT 2
#define IDENT 3
#define ASSIGN_OP 4
#define ADD_OP 5
#define SUB_OP 6
#define MULT_OP 7
#define DIV_OP 8
#define OPEN_PAREN 9
#define CLOSE_PAREN 10

// THIS IS PERSONALLY DEFINED (DOES NOT INTERFERE WITH FRONT.C)
#define EQUAL_OP 11
#define LESSER_OP 12
#define GREATER_OP 13
#define NEQUAL_OP 14
#define LEQUAL_OP 15
#define GEQUAL_OP 16
#define MOD_OP 17
#define BOOL_AND 18
#define BOOL_OR 19
#define BOOL_NOT 20
#define SEMICOLON 21
#define KEY_IN 22
#define KEY_OUT 23
#define KEY_IF 24
#define KEY_ELSE 25
#define OPEN_CURL 26
#define CLOSE_CURL 27

static char convert[][15] = {"LETTER","DIGIT","INT_LIT","IDENT","ASSIGN_OP","ADD_OP","SUB_OP","MULT_OP","DIV_OP","OPEN_PAREN","CLOSE_PAREN","EQUAL_OP","LESSER_OP","GREATER_OP","NEQUAL_OP","LEQUAL_OP","GEQUAL_OP","MOD_OP","BOOL_AND","BOOL_OR","BOOL_NOT","SEMICOLON","KEY_IN","KEY_OUT","KEY_IF","KEY_ELSE","OPEN_CURL","CLOSE_CURL","","","UNKNOWN", "EOF"};
int getLine();
char* getLex();
int lex();

#endif
