/**
* @Author Michal Frič
*/

#include <stdio.h>

#define print(str)              printf(str)                 // prints string
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

void static gen_built_in() {

}

void gen_init() {
    println(".IFJcode22");
    println("DEFVAR GF@*result"); // Global variable used to store expression result popped from stack if needed // todo GF@ bude využívané iba interne v rámci compileru ak sa nebude implementovať rozšírenie 'global' a teda premenné v globálnom rámci nemusia používať vyhradené identifikátory prefixované špeciálnymi znakmi
    println("CREATEFRAME");
    println("PUSHFRAME");
    println("JUMP $$main");
    printeol();
    gen_built_in();
}

void gen_variable_definition(char *id) {
    print("DEFVAR LF@");
    println(id);
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
    println("POPS GF@*_result");
    printcnt("JUMPIFEQ if_tail GF@*_result bool@false", counters.if_cnt);
}

void gen_if_tail() {
    printcnt("LABEL if_tail", counters.if_cnt++);
}

void gen_else_head() {
    printcnt("JUMPIFEQ else_tail GF@*_result bool@true", counters.else_cnt);
}

void gen_else_tail() {
    printcnt("LABEL else_tail", counters.else_cnt++);
}