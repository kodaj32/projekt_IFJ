/**
 * @Author Michal Frič <xfricm02@vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generator.h"
#include "scanner.h"
#include "linked_list.h"

/** Outputting macros for code generation */
#define print(str)              printf("%s", str)                 // prints string
#define println(str)            printf("%s\n", str)               // prints string & EOL
#define printeol()              printf("\n")                      // prints EOL
#define printcnt(str, cnt)      printf("%s%d\n", str, cnt)        // prints string & counter
#define printch(ch)             printf("%c", ch)                  // prints character
#define printd(dec)             printf("%03d", dec)               // prints decimal (integer)
#define printflt(flt)           printf("%a", strtod(flt, NULL))   // prints float
#define printfrmt(tok, flgs)    print_formatted_token(tok, flgs)  // prints formatted token

/** Macros for flag bitfield */
#define FLAG_TYPE_ONLY  (char) 0b10000000  // Do not print at sign followed by attribute of token
#define FLAG_TF         (char) 0b01000000  // Use Temporary frame instead of Local frame
#define FLAG_NL         (char) 0b00100000  // Insert new line at the end

/** Static repetitive components of final generated code */
#define TMP1 "GF@$1" // Used for temporary intermediate values

/**
 * Generate standard built-in function definitions.
 */
static void gen_built_in();

/**
 * Prints correctly formatted symbol token. todo nie iba symbol alebo to skor presunut
 *
 * @param token Symbol token.
 * @param flags Flags adjusting print format.
 * Flags (see definitions of corresponding macros):
 *  x....... FLAG_TYPE_ONLY
 *  .x...... FLAG_TF
 *  ..x..... FLAG_NF
 *  ...xxxxx Reserved (no use)
 */
static void print_formatted_token(Token *const token, char const flags);

/// Incrementing counters for labels
struct counters {
    unsigned while_cnt;
    unsigned if_cnt;
    unsigned else_cnt;
    unsigned data_type_check_cnt;
};

static struct counters counters = {};

void gen_init() {
    println(".IFJcode22");
    print("DEFVAR ");
    println(TMP1);
    println("CREATEFRAME");
    println("PUSHFRAME");
    printeol();
    gen_built_in();
}

void gen_variable_definition(Token *const variable_token) {
    print("DEFVAR ");
    printfrmt(variable_token, FLAG_NL);
}

void gen_expression_operand(Token *const symbol_token) {
    print("PUSHS ");
    printfrmt(symbol_token, FLAG_NL);
}

void gen_expression_operator(Token *const operation_token) {
    printfrmt(operation_token, FLAG_NL);
}

void gen_variable_assignment_of_symbol(Token *const variable_token, Token *const symbol_token) {
    print("MOVE ");
    printfrmt(variable_token, 0);
    print(" ");
    printfrmt(symbol_token, FLAG_NL);
}

void gen_variable_assignment_of_expression(Token *const variable_token) {
    print("POPS ");
    printfrmt(variable_token, FLAG_NL);
}

void gen_variable_dynamic_type_check(Token *const variable_token, Token *const data_type_token, Place place) {
    print("TYPE ");
    print(TMP1);
    print(" ");
    println(variable_token->attribute);
    print("JUMPIFEQ ");
    printcnt("$dynamic_type_check", counters.data_type_check_cnt);
    print(" ");
    print(TMP1);
    print(" string@");
    printfrmt(data_type_token, FLAG_TYPE_ONLY | FLAG_NL);
    print("EXIT ");
    println(place == function ? "4" : "7");
    printcnt("LABEL $dynamic_type_check", counters.data_type_check_cnt++);
    printeol();
}

void gen_while_head() {
    printcnt("LABEL $while_head", counters.while_cnt);
    printeol();
}

void gen_while_cond() {
    println("PUSHS bool@false"); // todo bude tam vzdy bool, alebo môže byť aj int? treba konverziu int2bool? toto záleží na tom ako sa budú spracovávať výrazi
    // todo "Pokud výsledná hodnota výrazu není pravdivostní (tj. pravda či nepravda - v základním zadání pouze jako výsledek aplikace relačních operátorů dle sekce 5.1), tak se hodnota null, prázdný řetězec, 0 a "0" považují za nepravdu a ostatní hodnoty jako pravda."
    // todo also bool je v základnom zadaní možné použiť iba ako podmienku a nie je možné s nim ďalej pracovať (napríklad ho ukladať do premennej)
    printcnt("JUMPIFEQS $while_tail", counters.while_cnt);
    printeol();
}

void gen_while_tail() {
    printcnt("JUMP $while_head", counters.while_cnt);
    printeol();
    printcnt("LABEL $while_tail", counters.while_cnt++);
    printeol();
}

void gen_if_head() { // todo rovnaký problém ako v gen_while_cond()
    print("POPS ");
    println(TMP1);
    printcnt("JUMPIFEQ $if_tail", counters.if_cnt);
    print(" ");
    print(TMP1);
    println(" bool@false");
}

void gen_if_tail() {
    printcnt("LABEL $if_tail", counters.if_cnt++);
    printeol();
}

void gen_else_head() {
    printcnt("JUMPIFEQ $else_tail", counters.else_cnt);
    print(" ");
    print(TMP1);
    println(" bool@true");
}

void gen_else_tail() {
    printcnt("LABEL $else_tail", counters.else_cnt++);
    printeol();
}

void gen_function_call(Token *const function_token, LList *variable_token_list, LList *symbol_token_list) {
    Token variable_token, symbol_token;

    println("CREATEFRAME");

    LL_First(variable_token_list);
    LL_First(symbol_token_list);

    // Iterate through lists, define and initialize function's arguments
    while(LL_IsActive(variable_token_list) && LL_IsActive(symbol_token_list)) {
        LL_GetValue(variable_token_list, &variable_token);
        LL_GetValue(symbol_token_list, &symbol_token);

        print("DEFVAR ");
        printfrmt(&variable_token, FLAG_TF | FLAG_NL);
        print("MOVE ");
        printfrmt(&variable_token, FLAG_TF);
        print(" ");
        printfrmt(&symbol_token, FLAG_NL);

        LL_Next(variable_token_list);
        LL_Next(symbol_token_list);
    }

    print("CALL ");
    println(function_token->attribute);
}

void gen_function_definition_head(Token *const function_token) {
    print("JUMP $$");
    println(function_token->attribute);
    print("LABEL ");
    println(function_token->attribute);
    println("PUSHFRAME");
}

void gen_function_definition_tail(Token *const function_token) {
    print("LABEL $$");
    println(function_token->attribute);
}

void gen_function_return() { // todo return void
    println("POPFRAME");
    println("RETURN");
}

static void gen_built_in() {
    Token function_token;

    /* function reads() : ?string */
    strcpy(function_token.attribute, "reads");
    gen_function_definition_head(&function_token);
    print("READ ");
    print(TMP1);
    println(" string");
    print("PUSHS ");
    println(TMP1);
    gen_function_return();
    gen_function_definition_tail(&function_token);

    /* function reads() : ?int */
    strcpy(function_token.attribute, "readi");
    gen_function_definition_head(&function_token);
    print("READ ");
    print(TMP1);
    println(" int");
    print("PUSHS ");
    println(TMP1);
    gen_function_return();
    gen_function_definition_tail(&function_token);

    /* function reads() : ?float */
    strcpy(function_token.attribute, "readf");
    gen_function_definition_head(&function_token);
    print("READ ");
    print(TMP1);
    println(" float");
    print("PUSHS ");
    println(TMP1);
    gen_function_return();
    gen_function_definition_tail(&function_token);

    /* function write ( term ) : void
     *
     * (Dynamic number of parameters is not supported in ifj22
     * and this function implementation defines just one
     * argument - multiple calls have to be performed on
     * syntax analysis level) */
    strcpy(function_token.attribute, "write");
    gen_function_definition_head(&function_token);
    println("WRITE LF@term");
    gen_function_return();
    gen_function_definition_tail(&function_token);

    /* function strlen(string $s) : int */ // todo vstavane funckie niektore chybaju
//    strcpy(function_token.attribute, "strlen");
//    gen_function_definition(&function_token);
//    print("STRLEN ");
    // STRLEN ⟨var⟩ ⟨symb⟩
}

static void print_formatted_token(Token *const token, char const flags) {
    char const _FLAG_TYPE_ONLY = 0b10000000 & flags;
    char const _FLAG_TF        = 0b01000000 & flags;
    char const _FLAG_NL        = 0b00100000 & flags;

    switch (token->type) {
        case T_VAR_ID:
            print(_FLAG_TF ? "TF@" : "LF@");
            print(token->attribute);
            break;
        case T_INT_VAL:
            if (_FLAG_TYPE_ONLY) {
                print("int");
            } else {
                print("int@");
                print(token->attribute);
            }
            break;
        case T_FLOAT_VAL:
        case T_FLOAT_EXP_VAL:
            if (_FLAG_TYPE_ONLY) {
                print("float");
            } else {
                print("float@");
                printflt(token->attribute);
            }
            break;
        case T_STRING_VAL:
            if (_FLAG_TYPE_ONLY) {
                print("string");
            } else { // Format string with escape sequences
                print("string@");
                char ch;
                for (int i = 0; i < (int) strlen(token->attribute); ++i) {
                    if ((ch = token->attribute[i]) == '\\' || ch == '#' || (ch <= 32 && ch >= 0)) {
                        printch('\\');
                        printd(ch);
                    } else {
                        printch(ch);
                    }
                }
            }
            break;
        case T_NULL:
            print("nil@nil");
            break;

        /* ***  Operation stack instructions following:  *** */
        case T_PLUS:
            print("ADDS");
            break;
        case T_MINUS:
            print("SUBS");
            break;
        case T_DIV:
            print("DIVS");
            break;
        case T_MUL:
            print("MULS");
            break;
        case T_CONCAT: // TODO
            break;
        case T_EQUAL: // todo
        case T_NOT_EQUAL:


            break;
        case T_GREATER: // todo
            print("S");
            break;
        case T_LESS: // todo
            print("S");
            break;
        case T_GREATER_EQUAL: // todo
            print("S");
            break;
        case T_LESS_EQUAL: // todo
            print("S");
            break;
        default:
            exit(99);
            // todo err exit (99 - internal compiler error) (zatiaľ neviem ako sa bude riešiť error handling)
    }

    if (_FLAG_NL) {
        printeol();
    }
}
