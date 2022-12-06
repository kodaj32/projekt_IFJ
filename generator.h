/**
 * Module for generating final code in IFJcode22 language. Is used by parser (so called 'Syntax-directed translation')
 * to generate smaller logical segments.
 *
 * Functions are in large using Token structure defined in scanner. Notation in these functions in general is following:
 *   Variable token is one with type   T_ID
 *   Function token is one with type   T_ID
 *   Data type token is one with type  T_INT_VAL | T_FLOAT_VAL | T_FLOAT_EXP_VAL | T_STRING_VAL | T_NULL
 *   Symbol token is one with type     T_INT_VAL | T_FLOAT_VAL | T_FLOAT_EXP_VAL | T_STRING_VAL | T_NULL | T_ID
 *   Operation token is one with type  T_PLUS | T_MINUS | T_DIV | T_MUL | T_CONCAT | T_EQUAL | T_NOT_EQUAL | T_GREATER |
 *                                     T_LESS | T_GREATER_EQUAL | T_LESS_EQUAL
 * It is callee responsibility to only pass tokens with correct types of what individual functions are expecting. What
 * types they expect differs.
 * Token data type might not be best suite for this, but was used, because in time of code generation creation it was
 * most suitable. This might be changed in the future.
 *
 * Documentation was omitted at some places, where code is self-documented well enough.
 *
 * @Author Michal Frič <xfricm02@vutbr.cz>
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"
#include "linked_list.h"

/// Flags for dynamic type checks by which exit code is determined
typedef enum place {
    expression,
    function
} Place;

/**
 * Initialize code generation. Must be called once and only once, before any of other code segments are generated.
 */
void gen_init();

/**
 * Variable definition.
 *
 * @param variable_token Variable token.
 */
void gen_variable_definition(Token *const variable_token);

/**
 * Push one operand to the top of data stack.
 *
 * Is intended for expressions generation.
 *
 * @param symbol_token Expression operand.
 */
void gen_expression_operand(Token *const symbol_token);

/**
 * Execute operation on two (or one if operation takes one argument) top operands on stack.
 *
 * Is intended for expressions generation.
 *
 * @param operation_token Expression operator.
 */
void gen_expression_operator(Token *const operation_token);

/**
 * @param variable_token Destination variable token.
 * @param symbol_token Source symbol token.
 */
void gen_variable_assignment_of_symbol(Token *const variable_token, Token *const symbol_token);

/**
 * Assign value from top of the data stack to variable. That might be:
 *   expression result
 *   function return value (return value expression)
 *
 * @param variable_token Variable token.
 */
void gen_variable_assignment_of_expression(Token *const variable_token);

/**
 * Dynamically checks data type of variable. This is needed since ifj22 is dynamically typed language.
 *
 * @param variable_token Variable token.
 * @param data_type_token Made-up token with type set to needed data type (attribute can be left uninitialized).
 * @param place Place where checked variable is in program, hence correct return error code can be chose.
 */
void gen_variable_dynamic_type_check(Token *const variable_token, Token *const data_type_token, Place place);

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
 * @param symbol_token_list List of symbols passed as arguments by callee.
 */
void gen_function_call(Token *const function_token, LList *variable_token_list, LList *symbol_token_list);

/**
 * Is called when start of function definition block is hit (before function body).
 *
 * This function can be called in any point of code generation and integrity of statements at the main level will not be
 * violated (In other words, this function head definition can be called in any point of main body program code
 * generation). This is ensured by generating JUMP statement from top of function definition to it's bottom, so only way
 * to jump into function body is by it's call.
 *
 * @param function_token Function token containing it's name as attribute.
 */
void gen_function_definition_head(Token *const function_token);

/**
 * Is called on end of function definition block.
 *
 * @param function_token Function token containing it's name as attribute.
 */
void gen_function_definition_tail(Token *const function_token);

/**
 * Generate return from function when return keyword is hit.
 */
void gen_function_return();

#endif