
/* Predchystaný stack na precedenčnú analýzu potiahnutý z IAL,
 * ešte ho budem upravovať
 */

#include "stack.h"

int STACK_SIZE = MAX_STACK;
int error_flag;
int solved;

/**
 * Vytiskne upozornění, že došlo k chybě při práci se zásobníkem.
 *
 * @param error_code Interní identifikátor chyby
 */
void Stack_Error( int error_code ) {
	static const char *SERR_STRINGS[MAX_SERR + 1] = {
			"Unknown error",
			"Stack error: INIT",
			"Stack error: PUSH",
			"Stack error: TOP"
	};

	if (error_code <= 0 || error_code > MAX_SERR)
		error_code = 0;
	printf("%s\n", SERR_STRINGS[error_code]);
	error_flag = 1;
}

/**
 * Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 *
 * @param stack Ukazatel na strukturu zásobníku
 */
void Stack_Init( Stack *stack ) {

	// ak pride neplatny stack parameter, vola funckiu
	if (stack == NULL) {
		Stack_Error(SERR_INIT);
	}
	else {
		stack->topIndex = -1; // nastavi topIndex na -1
	}
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník prázdný, jinak vrací hodnotu 0.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník prázdný, jinak nulu
 */
int Stack_IsEmpty( const Stack *stack ) {

    return ((stack->topIndex == -1) ? 1 : 0); // ak podmienka plati, vrati 1 inak vrati 0
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník plný, jinak nulu
 */
int Stack_IsFull( const Stack *stack ) {

    return ((stack->topIndex == (STACK_SIZE - 1)) ? 1 : 0); // ak je podmienka plati, vrati 1 inak vrati 0
}

/**
 * Vrací znak z vrcholu zásobníku prostřednictvím parametru dataPtr.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void Stack_Top( const Stack *stack, char *dataPtr ) {

	// ak je zasobnik prazdny, zavola funkciu
    if (Stack_IsEmpty(stack) == 1) {
		Stack_Error(SERR_TOP);
	}
	else {
		*dataPtr = stack->array[stack->topIndex]; // nastavi *dataPtr na hodnotu prvku na vrchole zasobniku
	}
}


/**
 * Odstraní prvek z vrcholu zásobníku. 
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void Stack_Pop( Stack *stack ) {

    // ak nie je zasobnik prazdny, zniz topIndex o 1
	if (Stack_IsEmpty(stack) == 0) {
		stack->topIndex--;
	}
}


/**
 * Vloží znak na vrchol zásobníku. 
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param data Znak k vložení
 */
void Stack_Push( Stack *stack, char data ) {

    // ak zasobnik nie je plny, vlozi data na vrchol zasobniku a zvysi topIndex o 1
	if (Stack_IsFull(stack) == 0) {
		stack->array[stack->topIndex+1] = data;
		stack->topIndex++;
	}
	else {
		Stack_Error(SERR_PUSH); // vola funkciu v pripade ze zasobnik je plny 
	}
}