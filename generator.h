/**
 * Module for generating final code in IFJcode22 language. Is used by parser (so called 'Syntax-directed translation')
 * to generate smaller logical segments.
 *
 * Functions are in large using Token structure defined in scanner. Notation in these functions in general is following:
 *   Variable and function token is one with type T_ID.
 *   Symbol token is one with T_ID | T_INT_VAL | T_FLOAT_VAL | T_FLOAT_EXP_VAL | T_STRING_VAL | T_NULL
 *
 * Documentation was omitted at some places, where code is self-documented well enough.
 *
 * @Author Michal Frič <xfricm02@vutbr.cz>
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"
#include "linked_list.h"

/**
 * Initialize code generation. Should be called once before any of other code segments are generated.
 */
void gen_init();

/**
 * Variable definition.
 *
 * @param variable_token Variable token.
 */
void gen_variable_definition(Token *const variable_token);

/**
 * @param variable_token Destination variable token.
 * @param symbol_token Source symbol token.
 */
void gen_variable_assignment_of_symbol(Token *const variable_token, Token *const symbol_token);

/**
 * Is called after function execution. Function return value is assigned to the variable token.
 *
 * @param variable_token Variable token.
 */
void gen_variable_assignment_of_function(Token *const variable_token);

/**
 * Generate while initialization before conditional expression calculation is generated.
 */
void gen_while_head();

/**
 * Generate while condition to control program flow after conditional expression calculation is generated.
 */
void gen_while_cond();

/**
 * Generate while ending after end of while block is hit.
 */
void gen_while_tail();

/**
 * Generate if initialization before conditional expression calculation is generated.
 */
void gen_if_head();

/**
 * Generate if ending after end of if block is hit.
 */
void gen_if_tail();

/**
 * Generate else initialization immediately after end of if block is hit.
 */
void gen_else_head();

/**
 * Generate else ending after end of else block is hit.
 */
void gen_else_tail();

/**
 * Generate function call. Both passed lists should contain same number of elements.
 *
 * @param function_token Function token containing it's name as attribute.
 * @param variable_token_list List of arguments of called function.
 * @param symbol_token_list List of symbols to passed as arguments by callee.
 */
void gen_function_call(Token *const function_token, LList *variable_token_list, LList *symbol_token_list);

/**
 * Is called when start of function definition block is hit (before function body).
 *
 * @param function_token Function token containing it's name as attribute.
 */
void gen_function_definition(Token *const function_token);

/**
 * Generate return from function when return keyword is hit.
 */
void gen_function_return();

#endif