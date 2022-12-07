#include <stdbool.h>
#include "preced_list.h"

void Prec_LL_Error(){
    fprintf(stderr,"*ERROR* The program has performed an illegal operation.\n");
    exit(99);
}

void Prec_LL_Init(PrecLList *list){

    list->activeElement = NULL;
	list->firstElement = NULL;
}

void Prec_LL_Dispose( PrecLList *list ) {

	struct PrecLLElement *tmpElemPtr;

	while (list->firstElement != NULL)
	{
		tmpElemPtr = list->firstElement;
		list->firstElement = list->firstElement->nextElement;
		free(tmpElemPtr);

	}

	list->activeElement = NULL;
}

void Prec_LL_InsertFirst( PrecLList *list, Prec_type data ) {

	struct PrecLLElement *newElemPtr = malloc(sizeof(struct PrecLLElement));

	if (newElemPtr == NULL) {
		Prec_LL_Error();
	}

	newElemPtr->data = data;
	newElemPtr->nextElement = list->firstElement;

	list->firstElement = newElemPtr;
}

void Prec_LL_First( PrecLList *list ) {

	list->activeElement = list->firstElement;
}

void Prec_LL_GetFirst( PrecLList *list, Prec_type *dataPtr ) {

	if (list->firstElement == NULL) {
		Prec_LL_Error();
	}
	else{
		*dataPtr = list->firstElement->data;
	}
}

void Prec_LL_GetFirstTerminal( PrecLList *list, Prec_type *dataPtr ) {

	Prec_LL_First(list);

	if (list->firstElement == NULL) {
		Prec_LL_Error();
	}
	else{
		bool not_found = true;
		while (not_found) {

			if ((list->activeElement->data == NON_TERMINAL) || (list->activeElement->data == HANDLE)) {
				Prec_LL_Next(list);
			}
			else {
				*dataPtr = list->activeElement->data;
				not_found = false;
			}
		}		
	}
}

void Prec_LL_RuleReduction( PrecLList *list ) {

	struct PrecLLElement *firstPtr = malloc(sizeof(struct PrecLLElement));

	if (firstPtr == NULL) {
		Prec_LL_Error();
	}

	Prec_LL_GetFirst(list, &firstPtr->data);

	if (firstPtr->data == ID) {

		Prec_LL_DeleteFirst(list);
		Prec_LL_GetFirst(list, &firstPtr->data);

		if (firstPtr->data == HANDLE) {

			// rule E -> i
			Prec_LL_DeleteFirst(list);
			Prec_LL_InsertFirst(list, NON_TERMINAL);
		}
	}
	else if (firstPtr->data == RIGHT_BR) {

		Prec_LL_DeleteFirst(list);
		Prec_LL_GetFirst(list, &firstPtr->data);

		if (firstPtr->data == NON_TERMINAL) {

			Prec_LL_DeleteFirst(list);
			Prec_LL_GetFirst(list, &firstPtr->data);

			if (firstPtr->data == LEFT_BR) {

				Prec_LL_DeleteFirst(list);
				Prec_LL_GetFirst(list, &firstPtr->data);

				if (firstPtr->data == HANDLE) {

					// rule E -> (E)
					Prec_LL_DeleteFirst(list);
					Prec_LL_InsertFirst(list, NON_TERMINAL);
				}
			}
		}
	}
	else if (firstPtr->data == NON_TERMINAL) {

		Prec_LL_DeleteFirst(list);
		Prec_LL_GetFirst(list, &firstPtr->data);

		if ((firstPtr->data == PLUS) || (firstPtr->data == MIN) || (firstPtr->data == MUL) || 
			(firstPtr->data == DIV) || (firstPtr->data == DOT) || (firstPtr->data == EQ) || 
			(firstPtr->data == NOT_EQ) || (firstPtr->data == LESS) || (firstPtr->data == GREATER) || 
			(firstPtr->data == LESS_E) || (firstPtr->data == GREATER_E)) {

			Prec_LL_DeleteFirst(list);
			Prec_LL_GetFirst(list, &firstPtr->data);

			if (firstPtr->data == NON_TERMINAL) {

				Prec_LL_DeleteFirst(list);
				Prec_LL_GetFirst(list, &firstPtr->data);

				if (firstPtr->data == HANDLE) {

					// all other rules
					Prec_LL_DeleteFirst(list);
					Prec_LL_InsertFirst(list, NON_TERMINAL);
				}
			}
		}
	}
	else {
		//error 
	}

	free(firstPtr);
}

void Prec_LL_DeleteFirst( PrecLList *list ) {

	struct PrecLLElement *tmpElementPtr;

	if (list->firstElement != NULL) {
		tmpElementPtr = list->firstElement;
		if (list->activeElement == list->firstElement) {
			list->activeElement = NULL;
		}

		list->firstElement = list->firstElement->nextElement;	
		free(tmpElementPtr);
	}
}

void Prec_LL_InsertAfter( PrecLList *list, Prec_type data ) {

	struct PrecLLElement *newElementPtr;
	
	if (list->activeElement != NULL)
	{
		newElementPtr = malloc(sizeof(struct PrecLLElement));
		if (newElementPtr == NULL){
			Prec_LL_Error();
		}

		newElementPtr->data = data;
		newElementPtr->nextElement = list->activeElement->nextElement;
		list->activeElement->nextElement = newElementPtr;
	}
}

void Prec_LL_InsertBeforeFirstTerminal( PrecLList *list, Prec_type data ) {

	struct PrecLLElement *newElementPtr;
	
	Prec_LL_First(list);

	if (list->activeElement != NULL)
	{
		newElementPtr = malloc(sizeof(struct PrecLLElement));
		if (newElementPtr == NULL){
			Prec_LL_Error();
		}

		bool not_found = true;
		while (not_found) {

			if ((list->activeElement->data == NON_TERMINAL) || (list->activeElement->data == HANDLE)) {
				
				if ((list->activeElement->nextElement->data != NON_TERMINAL) || 
					(list->activeElement->nextElement->data != HANDLE)) {

					// aktivny element je nonterminal, ale dalsi element je terminal
					// insert handle za active element
					newElementPtr->data = data;
					newElementPtr->nextElement = list->activeElement->nextElement;
					list->activeElement->nextElement = newElementPtr;
					not_found = false;
				}
				else {
					Prec_LL_Next(list);
				}				
			}
			else { // prvy element je terminal, insert handle first
				
				Prec_LL_InsertFirst(list, data);
				not_found = false;				
			}
		}
	}
}

void Prec_LL_Next( PrecLList *list ) {
	if (list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->nextElement;
	}
}

int Prec_LL_IsActive(PrecLList *list){

	return (list->activeElement != NULL) ? 1 : 0;
}