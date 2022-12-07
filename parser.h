#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdbool.h>
#include <stdio.h>

#include "scanner.h"
#include "preced_list.h"

/**
 * Following functions each represent LL-grammar rule
*/
bool prog(FILE *file, Token *token);
bool epilogue(FILE *file, Token *token);
bool stat_seq(FILE *file, Token *token);
bool next_stat(FILE *file, Token *token);
bool terminator(FILE *file, Token *token);
bool stat(FILE *file, Token *token);
bool assign(FILE *file, Token *token);
bool expr(FILE *file, Token *token);
bool val(FILE *file, Token *token);
bool fun_call(FILE *file, Token *token);
bool args(FILE *file, Token *token);
bool arg(FILE *file, Token *token);
bool args_n(FILE *file, Token *token);
bool cond_stat(FILE *file, Token *token);
bool while_cycle(FILE *file, Token *token);
bool fun_def(FILE *file, Token *token);
bool type(FILE *file, Token *token);
bool type2(FILE *file, Token *token);
bool type_prefix(FILE *file, Token *token);
bool params(FILE *file, Token *token);
bool params_n(FILE *file, Token *token);
bool var(FILE *file, Token *token);
bool operatorPrecedence(FILE *file, Token *token);
void setInput(Prec_type *dataPtr, Token *token);

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
    {'<','<','<','<','<','<','<','<','<','<','<','<','n','<','n'}
};

#endif