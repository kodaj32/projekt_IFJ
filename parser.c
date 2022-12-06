#include "parser.h"
#include "scanner.c"
#include "scanner.h"
#include "generator.c"
#include "symtable.h"

#include <stdio.h>
#include <stdbool.h>


bool prog(FILE *file, Token *token) {
    
    if (token->type == T_HEAD) {
        getToken(file, token);
        return (stat_seq(file, token) && epilogue(file, token));
    }
    else {
        return false;
    }
}

bool stat_seq(FILE *file, Token *token) {

    if ((token->type == T_ID) || (token->type == T_RETURN) || (token->type == T_IF) || (token->type == T_WHILE) || (token->type == T_FUNCTION) || (token->type == T_VAR_ID)) {
        return (stat(file, token) && next_stat(file, token));
    }
    else {
        return false;
    }
}

bool epilogue(FILE *file, Token *token) {
    if (token->type == T_EPILOGUE) {
        getToken(file, token);
        if (token->type == T_EOF){
            return true;
        }
        else {
            return false;
        }
    }
    else if (token->type == T_EOF) {
        return true;
    }
    else {
        return false;
    }
}

bool stat(FILE *file, Token *token) {

    if (token->type == T_RETURN) {
        getToken(file, token);
        return expr(file, token);
    }
    else if (token->type == T_ID) {
        return fun_call(file, token);
    }
    else if (token->type == T_IF) {
        return cond_stat(file, token);
    }
    else if (token->type == T_WHILE) {
        return while_cycle(file, token);
    }
    else if (token->type == T_FUNCTION) {
        return fun_def(file, token);
    }
    else if (token->type == T_VAR_ID) {
        return (var(file, token) && assign(file,token));
    }
    else if ((token->type == T_EPILOGUE) || (token->type == T_EOF) || (token->type == T_R_CUR_BRACKET)) {
        return true;
    }
    else {
        return false;
    }
}

bool next_stat(FILE *file, Token *token) {

    if ((token->type == T_EPILOGUE) || (token->type == T_R_CUR_BRACKET)) {
        getToken(file, token);
        return true;
    }
    else if (token->type == T_EOF) {
        return true;
    }
    else if (token->type == T_SEMICOLON) {
        return (terminator(file, token) && stat(file, token) && next_stat(file, token));
    }
    else {
        return false;
    }    
}

bool terminator(FILE *file, Token *token) {

    if (token->type == T_SEMICOLON) {
        getToken(file, token);
        return true;
    }
    else if ((token->type == T_ID) || (token->type == T_RETURN) || (token->type == T_IF) || (token->type == T_WHILE) || (token->type == T_FUNCTION) || (token->type == T_VAR_ID)) {
        return true;
    }
    else {
        return false;
    }    
}

bool assign(FILE *file, Token *token){

    if (token->type == T_ASSIGN) {
        getToken(file, token);
        return expr(file, token);
    }
    else {
        return false;
    }    
}

bool expr(FILE *file, Token *token) {

    // tu niekde bude prepnutie do precedencnej analyzy, zatial neviem este na zaklade coho posudit
    
    if ((token->type == T_INT_VAL) || (token->type == T_FLOAT_VAL) || (token->type == T_FLOAT_EXP_VAL) || (token->type == T_STRING_VAL) || (token->type == T_NULL)) {
        return val(file, token);
    }
    else if(token->type == T_ID) {
        return fun_call(file, token);
    }
    else if (token->type == T_VAR_ID) {
        return var(file, token);
    }
    else {
        return false;
    }    
}

bool val(FILE *file, Token *token) {

    getToken(file, token);
    return true;
}

bool fun_call(FILE *file, Token *token) {

    getToken(file, token);
    if (token->type == T_L_BRACKET) {
        getToken(file, token);
        if (args(file, token)) {
            if(token->type == T_R_BRACKET) {
                getToken(file, token);
                return true;
            }
        }
    }

    return false;
}

bool args(FILE *file, Token *token) {

    if ((token->type == T_INT_VAL) || (token->type == T_FLOAT_VAL) || (token->type == T_FLOAT_EXP_VAL) || (token->type == T_STRING_VAL) || (token->type == T_NULL) || (token->type == T_VAR_ID)) {
        return (arg(file, token) && args_n(file, token));
    }
    else if (token->type == T_R_BRACKET) {
        return true;
    }
    else {
        return false;
    }
}

bool arg(FILE *file, Token *token) {

    if ((token->type == T_INT_VAL) || (token->type == T_FLOAT_VAL) || (token->type == T_FLOAT_EXP_VAL) || (token->type == T_STRING_VAL) || (token->type == T_NULL)) {
        return val(file, token);
    }
    else if (token->type == T_VAR_ID) {
        return var(file, token);
    }
    else {
        return false;
    }    
}

bool args_n(FILE *file, Token *token) {

    if (token->type == T_R_BRACKET) {
        return true;
    }
    else if (token->type == T_COMMA) {
        return (arg(file, token) && args_n(file, token));
    }
    else {
        return false;
    }    
}

bool cond_stat(FILE *file, Token *token) {

    getToken(file, token);
    if (token->type == T_L_BRACKET) {
        getToken(file, token);
        if (expr(file, token)) {
            if (token->type == T_R_BRACKET) {
                getToken(file, token);
                if (token->type == T_L_CUR_BRACKET) {
                    getToken(file, token);
                    if (stat_seq(file, token)) {
                        if (token->type == T_R_CUR_BRACKET) {
                            getToken(file, token);
                            if (token->type == T_ELSE) {
                                getToken(file, token);
                                if (token->type == T_L_CUR_BRACKET) {
                                    getToken(file, token);
                                    if (stat_seq(file, token)) {
                                        if (token->type == T_R_CUR_BRACKET) {
                                            getToken(file, token);
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }                
            }
        }
    }

    return false;
}

bool while_cycle(FILE *file, Token *token) {

    getToken(file, token);
    if (token->type == T_L_BRACKET) {
        getToken(file, token);
        if (expr(file, token)) {
            if (token->type == T_R_BRACKET) {
                getToken(file, token);
                if (token->type == T_L_CUR_BRACKET) {
                    getToken(file, token);
                    if (stat_seq(file, token)) {
                        if (token->type == T_R_CUR_BRACKET) {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool fun_def(FILE *file, Token *token) {

    getToken(file, token);
    if (token->type == T_ID) {
        getToken(file, token);
        if (token->type == T_L_BRACKET) {
            getToken(file, token);
            if (params(file, token)) {
                if (token->type == T_DOUBLE_DOT) {
                    getToken(file, token);
                    if (type(file, token)) {
                        if (token->type == T_L_CUR_BRACKET) {
                            getToken(file, token);
                            if (stat_seq(file, token)) {
                                if (token->type == T_R_CUR_BRACKET) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool type(FILE *file, Token *token) {

    if (type_prefix(file, token) && type2(file, token)) {
        return true;
    }
    else {
        return false;
    }    
}

bool type2(FILE *file, Token *token) {

    if ((token->type == T_INT) || (token->type == T_FLOAT) || (token->type == T_VOID)) {
        getToken(file, token);
        return true;
    }
    else {
        return false;
    }    
}

bool type_prefix(FILE *file, Token *token) {

    if (token->type == T_TYPE_ID) {
        getToken(file, token);
        return true;
    }
    else if ((token->type == T_INT) || (token->type == T_FLOAT) || (token->type == T_VOID)) {
        return true;
    }
    else {
        return false;
    }
}

bool params(FILE *file, Token *token) {
    
    if (token->type == T_R_BRACKET) {
        getToken(file, token);
        return true;
    }
    else if ((token->type == T_TYPE_ID) || (token->type == T_INT) || (token->type == T_FLOAT) || (token->type == T_VOID)) {
        return (type(file, token) && var(file, token) && params_n(file, token));
    }
    else {
        return false;
    }    
}

bool params_n(FILE *file, Token *token) {

    if (token->type == T_R_BRACKET) {
        getToken(file, token);
        return true;
    }
    else if (token->type == T_COMMA) {
        getToken(file, token);
        return (type(file, token) && var(file, token) && params_n(file, token));
    }
    else {
        return false;
    }    
}

bool var(FILE *file, Token *token) {

    if (token->type == T_VAR_ID) {
        getToken(file, token);
        return true;
    }
    else {
        return false;
    }    
}


// TODO precedencna analyza
int operatorPrecedence() {

    return 0;
}

int main(int argc, char *argv[]) {

    // open file
    FILE * fp = fopen("file.txt", "r");

    if (fp == NULL) {
        return -1;
    }

    Token token;
    getToken(fp, &token);

    if (prog(fp, &token)) {
        return 0;
    }
}