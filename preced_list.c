
#include "preced_list.h"
#include "parser.h"

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

void Prec_LL_DeleteAfter( PrecLList *list ) {

	struct PrecLLElement *tmpElementPtr;

	if (list->activeElement != NULL)
	{
		if (list->activeElement->nextElement != NULL) {
			tmpElementPtr = list->activeElement->nextElement;
			list->activeElement->nextElement = tmpElementPtr->nextElement; 
	
			free(tmpElementPtr);
		}
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

void Prec_LL_Next( PrecLList *list ) {
	if (list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->nextElement;
	}
}

void Prec_LL_GetValue(PrecLList *list, Prec_type *dataPtr){ 

	if (list->activeElement == NULL){
		Prec_LL_Error();
	}
	else{

		*dataPtr = list->activeElement->data;
	}
}

void Prec_LL_SetValue( PrecLList *list, Prec_type data ) {

	if (list->activeElement != NULL) {
		list->activeElement->data = data;
	}
}

int Prec_LL_IsActive(PrecLList *list){

	return (list->activeElement != NULL) ? 1 : 0;
}