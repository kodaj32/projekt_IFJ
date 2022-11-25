//*************************************
//File: scanner.c
//
//Author: Radoslav Kodaj (xkodaj00)
//Date: 15.11.2022 
//*************************************


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "scanner.h"

bool head_detected = false;

int openFile(char *filename, FILE **file){

    if(filename == NULL){
        *file = stdin;
    }else{
        *file = fopen(filename,"r");
        if(*file == NULL){
            return -1;
        }
    }
    return 0;
}

int octalToDecimal(int octal){
    int decimal = 0;
    int i = 0;

    while(octal != 0){
        decimal = decimal + (octal % 10) * pow(8, i++);
        octal = octal / 10;
    }

    return decimal;
}

int hexaToDecimal(char *hexa){
    int decimal = 0;
    int i = 0;

    int val;
    int len = strlen(hexa);
    len--;

    while(hexa[i] != '\0'){
        if(hexa[i] >= '0' && hexa[i] <= '9'){
            val = hexa[i] - 48;

        }else if(hexa[i] >= 'a' && hexa[i] <= 'f'){
            val = hexa[i] - 97 + 10;

        }else if(hexa[i] >= 'A' && hexa[i] <= 'F'){
            val = hexa[i] - 65 + 10;
        }

        decimal += val * pow(16,len);
        len--;
        i++; 
    }

    return decimal;
}

int getToken(FILE *file, Token *token){

    char curr_char;
    State curr_state = STATE_START;
    char str[1000];
    char oct[10];
    char hex[10];
    strcpy(str,"");

    while(true){
        curr_char = getc(file);

        switch (curr_state)
        {
        case STATE_START:
            if (curr_char == '+')
            {
                token->type = T_PLUS;
                curr_state = STATE_FINAL;

            }else if(curr_char == '-')
            {
                token->type = T_MINUS;
                curr_state = STATE_FINAL;

            }else if(curr_char == '*')
            {
                token->type = T_MUL;
                curr_state = STATE_FINAL;

            }else if(curr_char == '/')
            {   
                curr_char = getc(file);
                if(curr_char == '/'){
                    curr_state = STATE_LINE_COMMENT;
                }else if(curr_char == '*'){
                    curr_state = STATE_BLOCK_COMMENT;
                }
                else{
                    token->type = T_DIV;
                    curr_state = STATE_FINAL;
                }
                
            }else if(curr_char == '.'){
                token->type = T_CONCAT;
                curr_state = STATE_FINAL;

            }else if(curr_char == ','){
                token->type = T_COMMA;
                curr_state = STATE_FINAL;
                
            }else if(curr_char == '('){
                token->type = T_L_BRACKET;
                curr_state = STATE_FINAL;

            }else if (curr_char == ')'){
                token->type = T_R_BRACKET;
                curr_state = STATE_FINAL;

            }else if(curr_char == '{'){
                token->type = T_L_CUR_BRACKET;
                curr_state = STATE_FINAL;

            }else if(curr_char == '}'){
                token->type = T_R_CUR_BRACKET;
                curr_state = STATE_FINAL;

            }else if(curr_char == '='){
                curr_state = STATE_ASSIGN;

            }else if(curr_char == '!'){
                curr_state = STATE_NOT;

            }else if(curr_char == '?'){
                curr_state = STATE_Q_MARK;

            }else if(curr_char == ':'){
                token->type = T_DOUBLE_DOT;
                curr_state = STATE_FINAL;

            }else if(curr_char == '$'){
                token->type = T_VAR_ID;
                curr_state = STATE_FINAL;

            }else if(curr_char == '>'){
                curr_state = STATE_GREATER;

            }else if(curr_char == '<'){
                curr_state = STATE_LESS;

            }else if(curr_char == ';'){
                token->type = T_SEMICOLON;
                curr_state = STATE_FINAL;
            }
            else if(isspace(curr_char)){
                curr_state = STATE_START;

            }else if(curr_char == '"'){
                curr_state = STATE_STRING_START;

            }else if(curr_char == '_' || isalpha(curr_char)){
                strncat(str, &curr_char, 1);
                curr_state = STATE_ID;

            }else if(isdigit(curr_char)){
                strncat(str, &curr_char, 1);
                curr_state = STATE_INTEGER_VAL;

            }else if(curr_char == EOF){

                if(!head_detected){
                    return 1;
                }else{
                    token->type = T_EOF;
                    curr_state = STATE_FINAL;
                }
            }else{
                //Chyba
                return 1;
            }
            break;

        case STATE_LINE_COMMENT:
            if(curr_char == '\n'){
                curr_state = STATE_START;
            }else if(curr_char == EOF){
                token->type = T_EOF;
                curr_state = STATE_FINAL;
            }
            break;

        case STATE_BLOCK_COMMENT:
            if(curr_char == '*'){
                curr_char = getc(file);
                if(curr_char == '/'){
                    curr_state = STATE_START;
                }else{
                    ungetc(curr_char,file);
                }
            }else if(curr_char == EOF){

                //Chyba
                return 1;
            }
            break;

        case STATE_LESS:
            if(curr_char == '='){
                token->type = T_LESS_EQUAL;
                curr_state = STATE_FINAL;
                
            }else if(curr_char == '?'){  
                curr_state = STATE_HEAD_1;

            }else{
                ungetc(curr_char, file);
                token->type = T_LESS;
                curr_state = STATE_FINAL;
            }
            break;
            
        case STATE_Q_MARK:
            if(curr_char == '>'){
                token->type = T_PROLOG;
                
            }else{
                token->type = T_TYPE_ID;
            }
            curr_state = STATE_FINAL;
            break;

        case STATE_HEAD_1:
            if(curr_char == 'p'){
                curr_state = STATE_HEAD_2;
            }else{
                //Chyba
                return 1;
            }
            break;

        case STATE_HEAD_2:
            if(curr_char == 'h'){
                curr_state = STATE_HEAD_3;
            }else{
                //Chyba
                return 1;
            }
            break;

        case STATE_HEAD_3:
            if(curr_char == 'p'){
                curr_state = STATE_FINAL;
                token->type = T_HEAD;
                strcpy(str, "<?php");
                head_detected = true;
            }else{
                //Chyba
                return 1;
            }
            break;

        case STATE_GREATER:
            if(curr_char == '='){
                token->type = T_GREATER_EQUAL;
            }else{
                ungetc(curr_char, file);
                token->type = T_GREATER;
            }
            curr_state = STATE_FINAL;
            break;

        case STATE_ASSIGN:
            if(curr_char == '='){
                curr_state = STATE_NEAR_ASSIGN;
            }else{
                ungetc(curr_char,file);
                token->type = T_ASSIGN;
                curr_state = STATE_FINAL;
            }

            break;

        case STATE_NEAR_ASSIGN:
            if(curr_char == '='){
                token->type = T_EQUAL;
                curr_state = STATE_FINAL;
            }else{

                //Chyba
                return 1;
            }
            break;
        case STATE_NOT:
            if(curr_char == '='){
                curr_state = STATE_NEAR_NOT_EQUAL;
            }else{
                return 1;   
            }
            break;
        case  STATE_NEAR_NOT_EQUAL:
            if(curr_char == '='){
                token->type = T_NOT_EQUAL;
                curr_state = STATE_FINAL;
            }else{
                return 1;
            }
            break;
        case STATE_ID:
            if(curr_char == '_' || isalpha(curr_char) || isdigit(curr_char)){
                strncat(str, &curr_char, 1);

            }else{
                ungetc(curr_char, file);
                curr_state = STATE_FINAL;

                if(strcmp(str, "else") == 0) 
                    token->type = T_ELSE;
                else if(strcmp(str, "if") == 0) 
                    token->type = T_IF;
                else if(strcmp(str,"return") == 0) 
                    token->type = T_RETURN;
                else if(strcmp(str,"while") == 0) 
                    token->type = T_WHILE;
                else if(strcmp(str,"function") == 0)    
                    token->type = T_FUNCTION;
                else if(strcmp(str,"int") == 0) 
                    token->type = T_INT;
                else if(strcmp(str,"float") == 0) 
                    token->type = T_FLOAT;
                else if(strcmp(str,"string") == 0) 
                    token->type = T_STRING;
                else if(strcmp(str,"void") == 0) 
                    token->type = T_VOID;
                else if(strcmp(str, "null") == 0)
                    token->type = T_NULL;
                else{
                    token->type = T_ID;
                }
            }
            break;
        case STATE_INTEGER_VAL:
            if(isdigit(curr_char)){
                strncat(str,&curr_char,1);

            }else if(curr_char == '.'){
                strncat(str, &curr_char,1);
                curr_state = STATE_FLOAT_DOT;

            }else if(curr_char == 'e' || curr_char == 'E'){
                strncat(str, &curr_char, 1);
                curr_state = STATE_FLOAT_EXP;

            }else{
                ungetc(curr_char, file);
                token->type = T_INT_VAL;
                curr_state = STATE_FINAL;
            }
            break;

        case STATE_FLOAT_DOT:
            if(isdigit(curr_char)){
                strncat(str, &curr_char,1);
                curr_state = STATE_FLOAT_VAL;
            }else{
                //Chyba
                return 1;
            }
            break;

        case STATE_FLOAT_VAL:
            if(isdigit(curr_char)){
                strncat(str, &curr_char, 1);
            }else if(curr_char == 'e' || curr_char == 'E'){
                strncat(str,&curr_char,1);
                curr_state = STATE_FLOAT_EXP;
            }else{
                ungetc(curr_char, file);
                token->type = T_FLOAT_VAL;
                curr_state = STATE_FINAL;
            }
            break;

        case STATE_FLOAT_EXP:
            if(curr_char == '+' || curr_char == '-' || isdigit(curr_char)){
                strncat(str, &curr_char, 1);
                curr_state = STATE_FLOAT_EXP_VAL;
            }else{
                //Chyba
                return 1;
            }
            break;

        case STATE_FLOAT_EXP_VAL:
            if(isdigit(curr_char)){
                strncat(str, &curr_char, 1);

            }else{
                ungetc(curr_char, file);    
                token->type = T_FLOAT_EXP_VAL;
                curr_state = STATE_FINAL;
            }
            break;

        case STATE_STRING_START:
            if(curr_char == '"'){
                token->type = T_STRING_VAL;
                curr_state = STATE_FINAL;

            }else if(curr_char == '\\'){
                
                curr_state = STATE_ESC_SEQ;

            }else if(curr_char == EOF){
                //Chyba
                return 1;
            }else{
                strncat(str, &curr_char, 1);
            }
            break;
        case STATE_ESC_SEQ:
            if(curr_char == 'x'){
        
                curr_state = STATE_ESC_SEQ_HEX_1;

            }else if(curr_char == 'n' || curr_char == '"' || curr_char == 't' || curr_char == '\\' || curr_char == '$'){
                
                if(curr_char == 'n'){
                    strcat(str,"\n");
                }else if(curr_char == '"'){    
                    strncat(str, &curr_char, 1);
                }else if(curr_char == 't'){
                    strcat(str,"\t");
                }else if(curr_char == '\\'){
                    strcat(str, "\\");
                }else if(curr_char == '$'){
                    strncat(str, &curr_char, 1);
                }
                curr_state = STATE_STRING_START;
            }else if(curr_char >= '0' && curr_char <= '3'){
                strncat(oct, &curr_char,1);
                curr_state = STATE_ESC_SEQ_OCT_1;
            }else{
                strcat(str,"\\");
                ungetc(curr_char, file);
                curr_state = STATE_STRING_START;
            }
            break;
        case STATE_ESC_SEQ_OCT_1:
            if(curr_char >= '0' && curr_char < '8'){
                strncat(oct, &curr_char,1);
                curr_state = STATE_ESC_SEQ_OCT_2;
            }else{
                strcat(str,"\\");
                ungetc(curr_char,file);
                
                strcat(str, oct);
                strcpy(oct, "");
                curr_state = STATE_STRING_START;
            }
            break;
        case STATE_ESC_SEQ_OCT_2:
            if(curr_char >= '0' && curr_char < '8'){
                strncat(oct, &curr_char,1);

                char c = (char)octalToDecimal(atoi(oct));
                strncat(str, &c, 1);

                strcpy(oct, "");
                curr_state = STATE_STRING_START;
            }else{
                strcat(str,"\\");
                strcat(str, oct);
                strcpy(oct, "");
                ungetc(curr_char,file);
                curr_state = STATE_STRING_START;
            }
            break;
        case STATE_ESC_SEQ_HEX_1:
            if(isdigit(curr_char) || (curr_char >= 'a' && curr_char <= 'f') || (curr_char >= 'A' && curr_char <= 'F')){
                strncat(hex, &curr_char,1);
                curr_state = STATE_ESC_SEQ_HEX_2;
            }else{
                strcat(str,"\\x");
                ungetc(curr_char,file);
                strcpy(hex,"");
                curr_state = STATE_STRING_START;
            }
            break;
        case STATE_ESC_SEQ_HEX_2:
            if (isdigit(curr_char) || (curr_char >= 'a' && curr_char <= 'f') || (curr_char >= 'A' && curr_char <= 'F')){
                strncat(hex, &curr_char,1);

                char c = (char)hexaToDecimal(hex);
                strncat(str, &c,1);

                strcpy(hex,"");
                curr_state = STATE_STRING_START;
            }else{
                strcat(str, "\\x");
                strcat(str, hex);
                ungetc(curr_char,file);
                strcpy(hex, "");
                curr_state = STATE_STRING_START;
            }
            break;
        default:
            //Chyba
            return 1;
            break;
        }
        
        if(curr_state == STATE_FINAL){
            strcpy(token->attribute,str);
            break;
        }
    }
    return 0;
}

const char* getTokenSymbol(Type_token type){

    switch (type){
        case T_ASSIGN: return "=";
        case T_COMMA: return ",";
        case T_CONCAT: return ".";
        case T_DIV: return "/";
        case T_DOUBLE_DOT: return ":";
        case T_EQUAL: return "===";
        case T_GREATER: return ">";
        case T_GREATER_EQUAL: return ">=";
        case T_L_BRACKET: return "(";
        case T_R_BRACKET: return ")";
        case T_L_CUR_BRACKET: return "{";
        case T_R_CUR_BRACKET: return "}";
        case T_VAR_ID: return "$";
        case T_TYPE_ID: return "?";
        case T_MINUS: return "-";
        case T_PLUS: return "+";
        case T_NOT_EQUAL: return "!==";
        case T_STRING_VAL: return "string";
        case T_SEMICOLON: return ";";
        case T_MUL: return "*";
        case T_INT_VAL: return "integer";
        case T_FLOAT_VAL: return "float";
        case T_FLOAT_EXP_VAL: return "float_exp";
        case T_LESS: return "<";
        case T_LESS_EQUAL: return "<=";
        case T_EOF: return "End of file";
        case T_ID: return "identificator";
        case T_HEAD: return "head (<?php)";
        case T_PROLOG: return "prolog";
        default: return "";
    }

}
