#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdbool.h>
#include <stdio.h>

#include "scanner.h"
#include "preced_list.h"
#include "symtable.h"

/**
 * Following functions each represent LL-grammar rule
*/
bool prog(FILE *file, Token *token, bst_node_t *table);
bool epilogue(FILE *file, Token *token);
bool stat_seq(FILE *file, Token *token, bst_node_t *table);
bool next_stat(FILE *file, Token *token, bst_node_t* table);
bool terminator(FILE *file, Token *token);
bool stat(FILE *file, Token *token, bst_node_t *table);
bool assign(FILE *file, Token *token,Token *var_id);
bool expr(FILE *file, Token *token);
bool val(FILE *file, Token *token);
bool fun_call(FILE *file, Token *token);
bool args(FILE *file, Token *token);
bool arg(FILE *file, Token *token);
bool args_n(FILE *file, Token *token);
bool cond_stat(FILE *file, Token *token, bst_node_t* table);
bool while_cycle(FILE *file, Token *token, bst_node_t* table);
bool fun_def(FILE *file, Token *token, bst_node_t* table);
bool type(FILE *file, Token *token);
bool type2(FILE *file, Token *token);
bool type_prefix(FILE *file, Token *token);
bool params(FILE *file, Token *token);
bool params_n(FILE *file, Token *token);
bool var(FILE *file, Token *token);
bool operatorPrecedence(FILE *file, Token *token);
void setInput(Prec_type *dataPtr, Token *token, int *lBracketFlag);
bool local_stat_seq(FILE *file, Token *token, bst_node_t *table);
bool local_next_stat(FILE *file, Token *token, bst_node_t *table);

char precedenceTable[15][15] = {
    {'>','>','>','<','<','>','>','>','>','>','>','<','>','<','>'},
    {'>','>','>','<','<','>','>','>','>','>','>','<','>','<','>'},
    {'>','>','>','<','<','>','>','>','>','>','>','<','>','<','>'},
    {'>','>','>','>','>','>','>','>','>','>','>','<','>','<','>'},
    {'>','>','>','>','>','>','>','>','>','>','>','<','>','<','>'},
    {'<','<','<','<','<','>','>','<','<','<','<','<','>','<','>'},
    {'<','<','<','<','<','>','>','<','<','<','<','<','>','<','>'},
    {'<','<','<','<','<','>','>','>','>','>','>','<','>','<','>'},
    {'<','<','<','<','<','>','>','>','>','>','>','<','>','<','>'},
    {'<','<','<','<','<','>','>','>','>','>','>','<','>','<','>'},
    {'<','<','<','<','<','>','>','>','>','>','>','<','>','<','>'},
    {'<','<','<','<','<','<','<','<','<','<','<','<','=','<','n'},
    {'>','>','>','>','>','>','>','>','>','>','>','n','>','n','>'},
    {'>','>','>','>','>','>','>','>','>','>','>','n','>','n','>'},
    {'<','<','<','<','<','<','<','<','<','<','<','<','n','<','w'}
};

#endif