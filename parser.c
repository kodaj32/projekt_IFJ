#include "parser.h"
#include "scanner.h"
#include "generator.h"
#include "symtable.h"
#include "preced_list.h"

#include <stdio.h>
#include <stdbool.h>

/** 
 * The start of syntax analysis through recursive descent.
 * Each bool function is an implemented rule from LL-grammar.
 * We check the token type from input and based on LL-table choose the designated rule to call.
*/
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

    if ((token->type == T_ID) || (token->type == T_RETURN) || 
        (token->type == T_IF) || (token->type == T_WHILE) || 
        (token->type == T_FUNCTION) || (token->type == T_VAR_ID)) {

        return (stat(file, token) && next_stat(file, token));
    }
    else {
        return false;
    }
}

bool local_stat_seq(FILE *file, Token *token) {

    if ((token->type == T_ID) || (token->type == T_RETURN) || 
        (token->type == T_IF) || (token->type == T_WHILE) || 
        (token->type == T_FUNCTION) || (token->type == T_VAR_ID)) {

        return (stat(file, token) && local_next_stat(file, token));
    }
    else {
        return false;
    }
}

bool local_next_stat(FILE *file, Token *token) {

    if ((token->type == T_R_CUR_BRACKET)) {
        return true;
    }
    else if ((token->type == T_SEMICOLON) || (token->type == T_ID) || 
             (token->type == T_RETURN) || (token->type == T_IF) || 
             (token->type == T_WHILE) || (token->type == T_FUNCTION) || 
             (token->type == T_VAR_ID)) {

        return (terminator(file, token) && stat(file, token) && local_next_stat(file, token));
    }
    else {
        return false;
    }    
}

bool epilogue(FILE *file, Token *token) {

    if (token->type == T_R_CUR_BRACKET) {
        getToken(file, token);
    }
    
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

    if ((token->type == T_EPILOGUE) || (token->type == T_EOF)) {
        return true;
    }
    else if ((token->type == T_SEMICOLON) || (token->type == T_ID) || 
             (token->type == T_RETURN) || (token->type == T_IF) || 
             (token->type == T_WHILE) || (token->type == T_FUNCTION) || 
             (token->type == T_VAR_ID)) {

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
    else if ((token->type == T_ID) || (token->type == T_RETURN) || 
             (token->type == T_IF) || (token->type == T_WHILE) || 
             (token->type == T_FUNCTION) || (token->type == T_VAR_ID)) {

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

    if ((token->type == T_VAR_ID) || (token->type == T_INT_VAL) || 
        (token->type == T_FLOAT_VAL) || (token->type == T_FLOAT_EXP_VAL) || 
        (token->type == T_STRING_VAL) || (token->type == T_NULL)) {

        /** Switch into operator precedence analysis */
        return operatorPrecedence(file, token); 
    }
    else if(token->type == T_ID) {
        return fun_call(file, token);
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

    if ((token->type == T_INT_VAL) || (token->type == T_FLOAT_VAL) || 
        (token->type == T_FLOAT_EXP_VAL) || (token->type == T_STRING_VAL) || 
        (token->type == T_NULL) || (token->type == T_VAR_ID)) {

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

    if ((token->type == T_INT_VAL) || (token->type == T_FLOAT_VAL) || 
        (token->type == T_FLOAT_EXP_VAL) || (token->type == T_STRING_VAL) || (token->type == T_NULL)) {

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
        getToken(file, token);
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
                    if (local_stat_seq(file, token)) {
                        if (token->type == T_R_CUR_BRACKET) {
                            getToken(file, token);
                            if (token->type == T_ELSE) {
                                getToken(file, token);
                                if (token->type == T_L_CUR_BRACKET) {
                                    getToken(file, token);
                                    if (local_stat_seq(file, token)) {
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
                    if (local_stat_seq(file, token)) {
                        if (token->type == T_R_CUR_BRACKET) {
                            getToken(file, token);
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
                            if (local_stat_seq(file, token)) {
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
    else if ((token->type == T_TYPE_ID) || (token->type == T_INT) || 
             (token->type == T_FLOAT) || (token->type == T_VOID)) {

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

/** Sets input variable based on actual token from input*/
void setInput(Prec_type *dataPtr, Token *token, int *lBracketFlag) {

    if (token->type == T_PLUS) {

        *dataPtr = PLUS;   
    }
    else if (token->type == T_MINUS) {

        *dataPtr = MIN;         
    }
    else if (token->type == T_CONCAT) {

        *dataPtr = DOT;       
    }
    else if (token->type == T_DIV) {
        
        *dataPtr = DIV;   
    }
    else if (token->type == T_MUL) {
        
        *dataPtr = MUL;        
    }
    else if (token->type == T_EQUAL) {
        
        *dataPtr = EQ;        
    }
    else if (token->type == T_NOT_EQUAL) {
        
        *dataPtr = NOT_EQ;         
    }
    else if (token->type == T_LESS) {

        *dataPtr = LESS;        
    }
    else if (token->type == T_GREATER) {

        *dataPtr = GREATER;
      
    }
    else if (token->type == T_LESS_EQUAL) {

        *dataPtr = LESS_E;         
    }
    else if (token->type == T_GREATER_EQUAL) {

        *dataPtr = GREATER_E;         
    }
    else if (token->type == T_L_BRACKET) {
        
        *dataPtr = LEFT_BR;
        *lBracketFlag = 1;     
    }
    else if (token->type == T_R_BRACKET) {

        if (*lBracketFlag == 1) {
            *dataPtr = RIGHT_BR;
            *lBracketFlag = 0;
        }
        else {
            *dataPtr = END_MARKER;
        }        
    }
    else if ((token->type == T_VAR_ID) || (token->type == T_INT_VAL) || 
             (token->type == T_FLOAT_VAL) || (token->type == T_FLOAT_EXP_VAL) || 
             (token->type == T_STRING_VAL) || (token->type == T_NULL)) {

        *dataPtr = ID;
    }
    else if (token->type == T_SEMICOLON) {

        *dataPtr = END_MARKER;
    }
    else {
        *dataPtr = ERR;
    }
}

/** The start of operator precedence analysis */
bool operatorPrecedence(FILE *file, Token *token) {

    bool repeat = true;

    /** Initialization of a list*/
    PrecLList list;
    Prec_LL_Init(&list);
    Prec_LL_InsertFirst(&list, END_MARKER);

    /**
     * 0 --> no L bracket yet
     * 1 --> L bracket on input/on stack
    */
    int lBracketFlag = 1;

    /** Allocation of input and top variable */
    PrecElementPtr input = malloc(sizeof(struct PrecLLElement));
    if (input == NULL) {
        Prec_LL_Error();
    }
    PrecElementPtr top = malloc(sizeof(struct PrecLLElement));
    if (top == NULL) {
        Prec_LL_Error();
    }

    while (repeat) {

        setInput(&input->data, token, &lBracketFlag);
        if (input->data == ERR) {
            return false;
        }

        Prec_LL_GetFirstTerminal(&list, &top->data);

        char op = precedenceTable[top->data][input->data];

        if (op == '=') {
            Prec_LL_InsertFirst(&list, input->data);

            getToken(file, token);
        }
        else if (op == '<') {
            Prec_LL_InsertBeforeFirstTerminal(&list, HANDLE);
            Prec_LL_InsertFirst(&list, input->data);

            getToken(file, token);
        }
        else if (op == '>') {
            Prec_LL_RuleReduction(&list);
        }
        else if (op == 'w') {
            repeat = false;
        }
        else {
            return false;
        }          
    }

    /** Free allocated memory */
    Prec_LL_Dispose(&list);
    free(input);
    free(top);

    return true;
}

int main(int argc, char *argv[]) {

    
    /** Open file */
    FILE * fp;

    if (argc == 2) {
        fp = fopen(argv[1], "r");
    }    

    if (fp == NULL) {
        return -1;
    }

    bst_node_t *sym_table;
    bst_init(sym_table);

    /** Initialize token structure and get first token */
    Token token;
    getToken(fp, &token);

    /** Start the syntax analysis */
    if (prog(fp, &token)) {

        
        return 0; // syntax analysis finished without any error
    }
    else {

        
        return 2; // syntax error
    }
}