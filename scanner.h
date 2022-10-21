/**
 * File: scanner.h
 * Author: Radoslav Kodaj (xkdoaj00)
 * 
 * Date: 2022-10-19
 */


#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>

//States
typedef enum state{

    STATE_START,

    STATE_CONCAT,
    STATE_MINUS,
    STATE_PLUS,
    STATE_MUL,
    STATE_DIV,
    STATE_L_BRACKET,
    STATE_R_BRACKET,
    STATE_L_CUR_BRACKET,
    STATE_R_CUR_BARCKET,
    
    STATE_COMMA,
    STATE_UNDER,
    STATE_ASSIGN,
    STATE_NEAR_ASSIGN,
    STATE_EQUAL,

    STATE_NOT,
    STATE_NEAR_NOT_EQUAL,
    STATE_NOT_EQUAL, 

    STATE_INTEGER_VAL,
    STATE_NEAR_FLOAT_VAL,
    STATE_FLOAT_VAL,
    STATE_FLOAT_EXP,
    STATE_FLOAT_EXP_VAL,

    STATE_STRING_START,
    STATE_STRING_END,
    STATE_ESC_SEQ,
    STATE_ESC_SEQ_OCT_1,
    STATE_ESC_SEQ_OCT_2,
    STATE_ESC_SEQ_HEX_1,
    STATE_ESC_SEQ_HEX_2,

    STATE_LINE_COMMENT,
    STATE_BLOCK_COMMENT,

    STATE_LESS,
    STATE_GREATER,

    STATE_ID,

    STATE_FINAL

}State;

//Token types
typedef enum t_type{
    
    //Keywords:
    T_ELSE,             //else
    T_FUNCTION,         //function
    T_RETURN,           //return
    T_IF,               //if
    T_FLOAT,            //float
    T_INT,              //int
    T_STRING,           //string
    T_NULL,             //null
    T_WHILE,            //while
    T_VOID,             //void

    T_PLUS,             // +
    T_MINUS,            // -
    T_DIV,              // /
    T_MUL,              // *
    T_CONCAT,           // .
    T_COMMA,            // ,
    T_ASSIGN,           // =
    T_EQUAL,            // ===
    T_SEMICOLON,        // ;
    T_NOT_EQUAL,        // !==
    T_L_BRACKET,        // (
    T_R_BRACKET,        // )
    T_L_CUR_BRACKET,    // {
    T_R_CUR_BRACKET,    // }
    T_DOUBLE_DOT,       // :
    T_ID,               // reprezents an identificator
    T_VAR_ID,           // $
    T_TYPE_ID,          // ?

    T_INT_VAL,          // integer value
    T_FLOAT_VAL,        // float value
    T_FLOAT_EXP_VAL,    // float value in exponentional form
    T_STRING_VAL,       // string value

    T_GREATER,          // >
    T_LESS,             // <
    T_GREATER_EQUAL,    // >=
    T_LESS_EQUAL,       // <=

    T_EOF,              // End of file
    T_ERROR             // Error
    
}Type_token;

//Token structure
typedef struct token{
    Type_token type;
    char attribute[50];
}Token;

/**
 * @param filename Name of the file
 * @param file Pointer for file reading
 * @return int -- 1 if an error is indicated else 0
 */
int openFile(char *filename, FILE **file);

/**
 * 
 * @param file File where scanner reads characters 
 * @param token Token structure where are stored informations
 * @return int -- 1 if an error is indicated else 0
 */
int getToken(FILE *file, Token *token);

/**
 * 
 * @param type Type of token
 * @return const char* -- String representing token type
 */
const char* getTokenSymbol(Type_token type);

#endif