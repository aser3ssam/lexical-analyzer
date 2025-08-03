/* front.c - a lexical analyzer for arithmetic expressions */

#include <stdio.h>
#include <ctype.h>
#include <string.h>


/* Global declarations */
int charClass;
char lexeme[100];     // stores the current lexeme
char nextChar;        // current character being read
int lexLen;           // length of the current lexeme
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10     // integer literal
#define IDENT 11       // identifier (like variable names)
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define END_OF_FILE -1


/* Main driver function */
int main() {
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in\n");
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    return 0;
}

/* This function matches single-character tokens like +, -, =, etc. */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;
        default:
    addChar();
    nextToken = -1; // Unknown token
    break;

    }
    return nextToken;
}

/* Adds the current character to the lexeme array */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long\n");
    }
}

/* Reads the next character and updates charClass accordingly */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar) || nextChar == '_')
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/* Skips whitespace */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/* Main lexical analyzer function */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        case EOF:
            nextToken = END_OF_FILE;
            strcpy(lexeme, "EOF");
            break;
    }
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}
