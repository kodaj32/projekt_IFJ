/**
* @Author Michal Frič <xfric02@vutbr.cz>
*/

#include <stdio.h>

#include "generator.h"
#include "scanner.h"

#define print(str)              printf("%s", str)           // prints string
#define println(str)            printf("%s\n", str)         // prints string & EOL
#define printeol()              printf("\n")                // prints EOL
#define printcnt(str, cnt)      printf("%s%d\n", str, cnt)  // prints string & counter & EOL
#define printd(dec)             printf("%d", dec)           // prints decimal (integer)

struct counters {
    unsigned while_cnt;
    unsigned if_cnt;
    unsigned else_cnt;
};

static struct counters counters = {};

void gen_init() {
    println(".IFJcode22");
    println("DEFVAR GF@result"); // Global variable used to store expression result popped from stack or function return value
    println("CREATEFRAME");
    println("PUSHFRAME");
    println("JUMP $$main");
    printeol();
    gen_built_in();
}

void gen_variable_definition(char const *id) {
    print("DEFVAR LF@");
    println(id);
}

void gen_variable_assignment_of_variable(char const *id1, char const *id2) {
    print("MOVE LF@");
    print(id1);
    print(" LF@");
    println(id2);
}

void gen_variable_assignment_of_literal(char const *id, char const *literal, Type_token const literal_type) {
    print("MOVE LF@");
    print(id);
    print(" ");
    print(get_type_string_of_literal(literal_type));
    println(literal);
}

void gen_variable_assignment_of_function(char const *id) {
    print("MOVE LF@");
    print(id);
    println(" GF@result");
}

void gen_while_head() {
    printcnt("LABEL while_head", counters.while_cnt);
}

void gen_while_cond() {
    println("PUSHS bool@false"); // todo bude tam vzdy bool, alebo môže byť aj int? treba konverziu int2bool? toto záleží na tom ako sa budú spracovávať výrazi
    // todo "Pokud výsledná hodnota výrazu není pravdivostní (tj. pravda či nepravda - v základním zadání pouze jako výsledek aplikace relačních operátorů dle sekce 5.1), tak se hodnota null, prázdný řetězec, 0 a "0" považují za nepravdu a ostatní hodnoty jako pravda."
    printcnt("JUMPIFEQS while_tail", counters.while_cnt);
}

void gen_while_tail() {
    printcnt("JUMP while_head", counters.while_cnt);
    printcnt("LABEL while_tail", counters.while_cnt++);
}

void gen_if_head() { // todo rovnaký problém ako v gen_while_cond()
    println("POPS GF@result");
    printcnt("JUMPIFEQ if_tail GF@result bool@false", counters.if_cnt);
}

void gen_if_tail() {
    printcnt("LABEL if_tail", counters.if_cnt++);
}

void gen_else_head() {
    printcnt("JUMPIFEQ else_tail GF@result bool@true", counters.else_cnt);
}

void gen_else_tail() {
    printcnt("LABEL else_tail", counters.else_cnt++);
}

void gen_function_call_preparation() {
    println("CREATEFRAME");
}

void gen_function_parameter_pass_of_variable(char const *id1, char const *id2) {
    print("DEFVAR TF@");
    println(id1);
    print("MOVE TF@")
    print(id1);
    print(" LF@");
    println(id2);
}

void gen_function_parameter_pass_of_literal(char const *id, char const *literal, Type_token const literal_type) {
    print("DEFVAR TF@");
    println(id);
    print("MOVE TF@")
    print(id);
    print(" ");
    print(get_type_string_of_literal(literal_type));
    println(literal);
}

void gen_function_call(char const *id) {
    print("CALL $");
    println(id);
}

void gen_function_definition(char const *id) {
    print("LABEL $");
    println(id);
    println("PUSHFRAME");
}

void gen_function_return() { // todo return value bude asi na vrchole zásobníku (záleží ako sa implementujú výrazi)
    println("POPFRAME");
    println("RETURN");
}

void gen_built_in() {

}

static char* get_type_string_of_literal(Type_token const literal_type) {
    switch (literal_type) {
        case T_INT_VAL:
            return "int@";
        case T_FLOAT_VAL:
        case T_FLOAT_EXP_VAL:
            return "float@";
        case T_STRING_VAL:
            return "string@";
        case T_NULL:
            return "nil@";
        default:
            // todo err exit (99 - internal compiler error)
    }
}

int main() {
    return 0;
}

// todo passing function return value through stack