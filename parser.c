#include <stdio.h>

#include "parser.h"
#include "front.h"

static void error();
int C();
int E();
int T();
int F();
int V();
int N();

/**
 * This is the example Recursive-Descent Parser in pp. 181 - 185 in the 
 * textbook 
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
 *
 * */
int P() {
    int s;
    do {
        s = S();
    } while(s==0 && nextToken!=EOF);
    return s;
}



/**
 * This is the statement method syntax rule:
 * S ::= V = E; | input(V); | output(E); | if ( C ) { S } | if ( C ) { S } else { S } | S S 
 * this is basically saying its either going to be an expression or a thing that ends with brackets,
 * and if there is more to execute then you are going to call yourself again.
 */
int S() {
    // printf("Enter <S>\n");
    if(nextToken==IDENT) {
        if(V()==0) {
            if(nextToken==ASSIGN_OP) {
                lex();
                if(E()==0) {
                    if(nextToken==SEMICOLON) {
                        lex();
                        return 0;
                    } else {
                        error();
                        return 1;
                    }
                } else {
                    return 1;
                }
            } else {
                error();
                return 1;
            }
        } else {
            return 1;
        }
    } else if(nextToken==KEY_IN) {
        lex();
        if(nextToken==OPEN_PAREN) {
            lex();
            if(V()==0) {
                if(nextToken==CLOSE_PAREN) {
                    lex();
                    if(nextToken==SEMICOLON) {
                        lex();
                        return 0;
                    } else {
                        error();
                        return 1;
                    }
                } else {
                    error();
                    return 1;
                }
            } else {
                return 1;
            }
        } else {
            error();
            return 1;
        }
    } else if(nextToken==KEY_OUT) {
        lex();
        if(nextToken==OPEN_PAREN) {
            lex();
            if(E()==0) {
                if(nextToken==CLOSE_PAREN) {
                    lex();
                    if(nextToken==SEMICOLON) {
                        lex();
                        return 0;
                    } else {
                        error();
                        return 1;
                    }
                } else {
                    error();
                    return 1;
                }
            } else {
                return 1;
            }
        } else {
            error();
            return 1;
        }
    } else if(nextToken==KEY_IF) {
        lex();
        if(nextToken==OPEN_PAREN) {
            lex();
            if(C()==0) {
                if(nextToken==CLOSE_PAREN) {
                    lex();
                    if(nextToken==OPEN_CURL) {
                        lex();
                        do {
                            if(S() == 1){
                                return 1;
                            }
                        } while(nextToken!=CLOSE_CURL);
                            if(nextToken==CLOSE_CURL) {
                                lex();
                                if(nextToken==KEY_ELSE) {
                                    lex();
                                    if(nextToken==OPEN_CURL) {
                                        lex();
                                        do {
                                            if(S() == 1){
                                                return 1;
                                            }
                                        } while(nextToken!=CLOSE_CURL);
                                            if(nextToken==CLOSE_CURL){
                                                lex();
                                                return 0;
                                            } else {
                                                return 1;
                                            }
                                    } else {
                                        error();
                                        return 1;
                                    }
                                } else {
                                    return 0;
                                }
                            } else {
                                error();
                                return 1;
                            }
                    } else {
                        error();
                        return 1;
                    }
                } else {
                    error();
                    return 1;
                }
            } else {
                return 1;
            }
        } else {
            error();
            return 1;
        }
    } else {
        error();
        return 1;
    }
}

/**
 * This is the C rule, the syntax rule:
 * C ::= E < E | E > E | E == E | E != E | E <= E | E >= E | C && C | C || C | !C
 * basically, first look for if it is !infront, if it doesn't, then evaluate the E();
 * then after everything you check if && exists after or || which you then recall the C() mehtod
 */
int C() 
{
    // printf("Enter <C>\n");
    if(nextToken != BOOL_NOT) {
        if(E()==0) {
            if(nextToken==GREATER_OP || nextToken==LESSER_OP || nextToken==EQUAL_OP || nextToken==NEQUAL_OP || nextToken==GEQUAL_OP || nextToken==LEQUAL_OP) {
                lex();
                if(E() == 1)
                    return 1;
                if(nextToken==BOOL_AND || nextToken==BOOL_OR) {
                    lex();
                    return C();
                } else {
                    return 0;
                }
            } else {
                error();
                return 1;
            }
        } else {
            return 1;
        }
    } 
    
    lex();
    return C();
}

/* expr PRIME
 * Parses strings in the language generated by the rule:
 * E ::= T | E + T | E - T 
 * to avoid leftwise recursion we make it:
 * E::= TE'
 * 
 * E'::= +TE' | -TE' | end
 */
int EP() {
    // printf("Enter <EP>\n");
    if(nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        if(T() == 0) {
            // printf("Exiting <EP> \n");
            return EP();
        } else {
            error();
            return 1;
        }
    } else { 
        // printf("Exiting <EP>\n");
        return 0;
    }
}

/* expr
 * Parses strings in the language generated by the rule:
 * E ::= T | E + T | E - T 
 * to avoid leftwise recursion we make it:
 * E::= TE'
 * 
 * E'::= +TE' | -TE' | end
 */
int E() 
{
    // printf("Enter <E>\n");
    if(T()==0) {
        // printf("Exiting <E>\n");
        return EP();
    }
    return 1;

} 

/* term PRIME
 * Parses strings in the language generated by the rule:
 * T ::= F | T * F | T / F | T % F 
 * But to avoid left Recursion we implement the following:
 * T ::= FT'
 * T' ::= *FT' | /FT' | %FT' | end
 */
int TP() 
{
    // printf("Enter <TP>\n");
    if(nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP){ 
        lex();
        if(F()==0){
            return TP();
        } else{
            // printf("Exiting <TP> with an error\n");
            return 1;
        }
    } else {
        // printf("Exiting <TP>\n");
        return 0;
    }
}

/* term
 * Parses strings in the language generated by the rule:
 * T ::= F | T * F | T / F | T % F 
 * But to avoid left Recursion we implement the following:
 * T ::= FT'
 * T' ::= *FT' | /FT' | %FT' | end
 */
int T() 
{
    // printf("Enter <T>\n");
    if(F() == 0) {
        if(TP()==0) {
            // printf("Exiting <T>\n");
            return 0;
        } else {
            error();
            return 1;
        }
        
    } else {
        // printf("Exiting <T> with an error\n");
        return 1;
    }
}


/* factor
 * Parses strings in the language generated by the rule:
 * F ::= (E) | N | V 
 * */
int F() 
{
    // printf("Enter <F>\n");
    if (nextToken == OPEN_PAREN) {
        lex();
        if (E() == 0) {
            if(nextToken == CLOSE_PAREN) {
                // printf("Exiting <F>\n");
                lex();
                return 0;
            } else {
                // printf("Exiting <F> with an error\n");
                error();
                return 1;
            }
        } else {
            // printf("Exiting <F>\n");
            return 1;
         }
    } else  if(nextToken == INT_LIT){
        // printf("Exiting <F>\n");
        return N();
    } else if (nextToken == IDENT) { 
        // printf("Exiting <F>\n");
        return V();
    } else {
        // printf("Exiting <F> with an error\n");
        error();
        return 1;
    }
} 


/**
 * This is the rule of idents:
 * V ::= a | b | … | y | z | aV | bV | … | yV | zV 
 */
int V() {
    // printf("Enter <V>\n");
    if(nextToken == IDENT) {
        // printf("Exiting <v>\n");
        lex();
        return 0;
    } else {
        // printf("Exiting <v> with an error\n");
        error();
        return 1;
    }
}


/**
 * This is the rule for integer literals:
 * N ::= 0 | 1 | … | 8 | 9 | 0N | 1N | … | 8N | 9N 
 */
int N() {
    // printf("Enter <N>\n");
    if(nextToken == INT_LIT) {
        lex();
        // printf("Exiting <N>\n");
        return 0;
    }
    else {
        // printf("Exiting <N> with an error\n");
        error();
        return 1;
    }
}

static void error() {
    printf("Error encounter on line %d:  The next lexeme was %s and the next token was %s\n", getLine(), getLex(), nextToken!=EOF? convert[nextToken]: "EOF");
}
