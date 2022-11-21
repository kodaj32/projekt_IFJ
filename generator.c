/**
* @Author Michal Frič <xfricm02@vutbr.cz>
*/

#include <stdio.h>
#include <string.h>

#include "generator.h"
#include "scanner.h"
#include "linked_list.h"

/* Outputting macros for code generation */
#define print(str)              printf("%s", str)           // prints string
#define println(str)            printf("%s\n", str)         // prints string & EOL
#define printeol()              printf("\n")                // prints EOL
#define printcnt(str, cnt)      printf("%s%d\n", str, cnt)  // prints string & counter
#define printd(dec)             printf("%d", dec)           // prints decimal (integer)

/* Macros for flag bitfield */
#define FLAG_NO_AT  0b10000000
#define FLAG_TF     0b01000000

/* Static repetitive components of final generated code */
#define RESULT_VAR "GF@result" // Global variable used to store expression result popped from stack or function return value

static void gen_built_in();
static char* get_type_string_of_symbol(Type_token const symbol_type, char const flags);

struct counters {
    unsigned while_cnt;
    unsigned if_cnt;
    unsigned else_cnt;
};

static struct counters counters = {};

void gen_init() {
    println(".IFJcode22");
    print("DEFVAR ");
    println(RESULT_VAR);
    println("CREATEFRAME");
    println("PUSHFRAME");
    println("JUMP $$main");
    printeol();
    gen_built_in();
    println("LABEL $$main"); // todo Premyslieť, ako situovať $$main. To by mohlo byť problematické, pretože sa mi zdá, že funckie môžu ľubovolne prekrývať hlavné telo programu.
}

void gen_variable_definition(Token *const variable_token) {
    print("DEFVAR ");
    print(get_type_string_of_symbol(variable_token->type, 0));
    println(variable_token->attribute);
}

void gen_variable_assignment_of_symbol(Token *const variable_token, Token *const symbol_token) {
    print("MOVE ");
    print(get_type_string_of_symbol(variable_token->type, 0));
    print(variable_token->attribute);
    print(" ");
    print(get_type_string_of_symbol(symbol_token->type, 0));
    println(symbol_token->attribute);
}

void gen_variable_assignment_of_function(Token *const variable_token) {
    print("MOVE ");
    print(get_type_string_of_symbol(variable_token->type, 0));
    print(variable_token->attribute);
    print(" ");
    println(RESULT_VAR);
}

void gen_while_head() {
    printcnt("LABEL while_head", counters.while_cnt);
    printeol();
}

void gen_while_cond() {
    println("PUSHS bool@false"); // todo bude tam vzdy bool, alebo môže byť aj int? treba konverziu int2bool? toto záleží na tom ako sa budú spracovávať výrazi
    // todo "Pokud výsledná hodnota výrazu není pravdivostní (tj. pravda či nepravda - v základním zadání pouze jako výsledek aplikace relačních operátorů dle sekce 5.1), tak se hodnota null, prázdný řetězec, 0 a "0" považují za nepravdu a ostatní hodnoty jako pravda."
    printcnt("JUMPIFEQS while_tail", counters.while_cnt);
    printeol();
}

void gen_while_tail() {
    printcnt("JUMP while_head", counters.while_cnt);
    printeol();
    printcnt("LABEL while_tail", counters.while_cnt++);
    printeol();
}

void gen_if_head() { // todo rovnaký problém ako v gen_while_cond()
    print("POPS ");
    println(RESULT_VAR);
    printcnt("JUMPIFEQ if_tail GF@result", counters.if_cnt);
    println(" bool@false");
}

void gen_if_tail() {
    printcnt("LABEL if_tail", counters.if_cnt++);
    printeol();
}

void gen_else_head() {
    printcnt("JUMPIFEQ else_tail", counters.else_cnt);
    print(" ");
    print(RESULT_VAR);
    println(" bool@true");
}

void gen_else_tail() {
    printcnt("LABEL else_tail", counters.else_cnt++);
    printeol();
}

void gen_function_call(Token *const function_token, LList *variable_token_list, LList *symbol_token_list) {
    Token *variable_token, *symbol_token;

    println("CREATEFRAME");

    LL_First(variable_token_list);
    LL_First(symbol_token_list);

    // Iterate through lists, define and initialize arguments
    while(LL_IsActive(variable_token_list) && LL_IsActive(symbol_token_list)) {
        LL_GetValue(variable_token_list, variable_token);
        LL_GetValue(symbol_token_list, symbol_token);

        print("DEFVAR ");
        print(get_type_string_of_symbol(variable_token->type, FLAG_TF));
        println(variable_token->attribute);
        print("MOVE ");
        print(get_type_string_of_symbol(variable_token->type, FLAG_TF));
        print(variable_token->attribute);
        print(" ");
        print(get_type_string_of_symbol(symbol_token->type, 0));
        println(symbol_token->attribute);

        LL_Next(variable_token_list);
        LL_Next(symbol_token_list);
    }

    print("CALL $");
    println(function_token->attribute);
}

void gen_function_definition(Token *const function_token) {
    print("LABEL $");
    println(function_token->attribute);
    println("PUSHFRAME");
}

void gen_function_return() { // todo return value bude asi na vrchole zásobníku (záleží ako sa implementujú výrazi)
    println("POPFRAME");
    println("RETURN");
}

void gen_built_in() {
    Token function_token;

    /* reads() */
    strcpy(function_token.attribute, "reads");
    gen_function_definition(&function_token);
    print("READ ");
    print(RESULT_VAR);
    println(" string");
    gen_function_return();

    /* readi() */
    strcpy(function_token.attribute, "readi");
    gen_function_definition(&function_token);
    print("READ ");
    print(RESULT_VAR);
    println(" int");
    gen_function_return();

    /* readf() */
    strcpy(function_token.attribute, "readf");
    gen_function_definition(&function_token);
    print("READ ");
    print(RESULT_VAR);
    println(" float");
    gen_function_return();

}

static char* get_type_string_of_symbol(Type_token const symbol_type, char const flags) {
    char const _FLAG_NO_AT = 0b1000000 & flags;
    char const _FLAG_TF = 0b0100000 & flags;

    switch (symbol_type) {
        case T_ID:
            if (_FLAG_TF) {
                return _FLAG_NO_AT ? "@TF" : "TF";
            }
            return _FLAG_NO_AT ? "@LF" : "LF";
        case T_INT_VAL:
            return _FLAG_NO_AT ? "@int" : "int";
        case T_FLOAT_VAL:
        case T_FLOAT_EXP_VAL:
            return _FLAG_NO_AT ? "@float" : "float";
        case T_STRING_VAL:
            return _FLAG_NO_AT ? "@string" : "string";
        case T_NULL:
            return _FLAG_NO_AT ? "@nil" : "nil";
        default:
            // todo err exit (99 - internal compiler error) (zatiaľ neviem ako sa bude riešiť error handling)
    }
}

int main() {
    gen_init();
//    gen_function_call("reads", NULL, NULL);
    return 0;
}

// todo ?maybe? čistiť stack