/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages.
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme[100];
static char nextChar;
static int lexLen;
static FILE *in_fp;
static int lineCounter = 1;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

// f =  15 ;  i  =  11  ;  input  (  n  )  ;  if  (  n  !=  i  )  {  output  (  f  )  ;  }

/******************************************************/
/* main driver */
int main(int argc, char *argv[])
{
    printf("Cooke Analyzer:: R11833748\n\n");
    /* Open the input data file and process its contents */
    if (argc == 1)
    {
        printf("ERROR - there is no input\n");
        return 2;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if ((in_fp = fopen(argv[i], "r")) == NULL)
            {
                printf("ERROR - input file was not found\n");
                return 3;
            }
            else
            {
                /// THIS IS FOR THE PARSER
                // getChar();
                // do {
                //     lex();
                // } while (nextToken != EOF);

                // This is for the syntax analyzer
                getChar(); // must call this once in the beginning
                lex();
                if (P() == 0)
                {
                    // printf("There were no syntax errors found\n");
                    // printf("\nThe line ended on is: %d" , lineCounter); // testing
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
 * token */
static int lookup(char ch)
{
    switch (ch)
    {
    case '(':
        addChar();
        nextToken = OPEN_PAREN;
        break;
    case ')':
        addChar();
        nextToken = CLOSE_PAREN;
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
    case '%':
        addChar();
        nextToken = MOD_OP;
        break;
    case '>':
        addChar();
        getChar();
        if (nextChar == '=')
        {
            nextToken = GEQUAL_OP;
            addChar();
        }
        else
        {
            nextToken = GREATER_OP;
            ungetc(nextChar, in_fp);
        }
        break;
    case '<':
        addChar();
        getChar();
        if (nextChar == '=')
        {
            addChar();
            nextToken = LEQUAL_OP;
        }
        else
        {
            ungetc(nextChar, in_fp);
            nextToken = LESSER_OP;
        }
        break;
    case '=':
        addChar();
        getChar();
        if (nextChar == '=')
        {
            addChar();
            nextToken = EQUAL_OP;
        }
        else
        {
            ungetc(nextChar, in_fp);
            nextToken = ASSIGN_OP;
        }
        break;
    case '!':
        addChar();
        getChar();
        if (nextChar == '=')
        {
            addChar();
            nextToken = NEQUAL_OP;
        }
        else
        {
            ungetc(nextChar, in_fp);
            nextToken = BOOL_NOT;
        }
        break;
    case '&':
        addChar();
        getChar();
        if (nextChar == '&')
        {
            addChar();
            nextToken = BOOL_AND;
        }
        else
        {
            ungetc(nextChar, in_fp);
            nextToken = UNKNOWN;
        }
        break;
    case '|':
        addChar();
        getChar();
        if (nextChar == '|')
        {
            addChar();
            nextToken = BOOL_OR;
        }
        else
        {
            ungetc(nextChar, in_fp);
            nextToken = UNKNOWN;
        }
        break;
    case ';':
        addChar();
        nextToken = SEMICOLON;
        break;
    case '{':
        addChar();
        nextToken = OPEN_CURL;
        break;
    case '}':
        addChar();
        nextToken = CLOSE_CURL;
        break;
    default:
        addChar();
        nextToken = UNKNOWN;
        break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar()
{
    if (lexLen <= 98)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
    {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */
static void getChar()
{
    if ((nextChar = getc(in_fp)) != EOF)
    {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
        {
            charClass = UNKNOWN;
        }
    }
    else
    {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */
static void getNonBlank()
{
    while (isspace(nextChar) && charClass != EOF)
    {
        if (nextChar == '\n')
        {
            lineCounter++;
        }
        getChar();
    }
}

/*****************************************************/
/* isInput - a function to get if the letter input is actually unkown */
static bool isInput()
{
    return strcmp(lexeme, "input") == 0;
}

static bool isOutput()
{
    return strcmp(lexeme, "output") == 0;
}

static bool isIf()
{
    return strcmp(lexeme, "if") == 0;
}

static bool isElse()
{
    return strcmp(lexeme, "else") == 0;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex()
{
    lexLen = 0;
    getNonBlank();

    switch (charClass)
    {
    /* Parse integer literals */
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT)
        {
            addChar();
            getChar();
        }

        nextToken = INT_LIT;
        break;

    /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        if (isInput())
        {
            nextToken = KEY_IN;
        }
        else if (isOutput())
        {
            nextToken = KEY_OUT;
        }
        else if (isIf())
        {
            nextToken = KEY_IF;
        }
        else if (isElse())
        {
            nextToken = KEY_ELSE;
        }
        else
        {
            nextToken = IDENT;
        }
        break;

    /* parser for UNKNOWNS */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

    /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        return nextToken;
    } /* End of switch */
    // printf("%-8s%s\n",lexeme,  convert[nextToken]);
    return nextToken;
} /* End of function lex */

/**
 * This is a method for the parser to get the value of the current line for the error message
 */
int getLine()
{
    return lineCounter;
}

/**
 * This is a method for the parser to return the current lexeme for the error message
 */
char *getLex()
{
    return lexeme;
}