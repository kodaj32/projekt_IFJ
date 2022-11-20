
#include "linked_list.h"
#include "scanner.h"


void LL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
}

void LL_Init( LList *list ) {

	list->activeElement = NULL;
	list->firstElement = NULL;
}

void LL_Dispose( LList *list ) {

	struct LLElement *tmpElemPtr;

	while (list->firstElement != NULL)
	{
		tmpElemPtr = list->firstElement;
		list->firstElement = list->firstElement->nextElement;
		free(tmpElemPtr);

	}

	list->activeElement = NULL;
}

void LL_InsertFirst( LList *list, int data ) {

	struct LLElement *newElemPtr = malloc(sizeof(struct LLElement));

	if (newElemPtr == NULL) {
		LL_Error();
	}

	newElemPtr->data = data;
	newElemPtr->nextElement = list->firstElement;

	list->firstElement = newElemPtr;
}


void LL_First( LList *list ) {

	list->activeElement = list->firstElement;
}


void LL_GetFirst( LList *list, int *dataPtr ) {

	if (list->firstElement == NULL) {
		LL_Error();
	}
	else{
		*dataPtr = list->firstElement->data;
	}
}

void LL_DeleteFirst( LList *list ) {

	struct LLElement *tmpElementPtr;

	if (list->firstElement != NULL) {
		tmpElementPtr = list->firstElement;
		if (list->activeElement == list->firstElement) {
			list->activeElement = NULL;
		}

		list->firstElement = list->firstElement->nextElement;	
		free(tmpElementPtr);
	}
}

void LL_DeleteAfter( LList *list ) {

	struct LLElement *tmpElementPtr;

	if (list->activeElement != NULL)
	{
		if (list->activeElement->nextElement != NULL) {
			tmpElementPtr = list->activeElement->nextElement;
			list->activeElement->nextElement = tmpElementPtr->nextElement; 
	
			free(tmpElementPtr);
		}

	}
}

void LL_InsertAfter( LList *list, int data ) {

	struct LLElement *newElementPtr;
	
	if (list->activeElement != NULL)
	{
		newElementPtr = malloc(sizeof(struct LLElement));
		if (newElementPtr == NULL){
			LL_Error();
		}

		newElementPtr->data = data;
		newElementPtr->nextElement = list->activeElement->nextElement;
		list->activeElement->nextElement = newElementPtr;

	}
}

void LL_Next( LList *list ) {
	if (list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->nextElement;
	}
}

void LL_GetValue(LList *list, int *dataPtr){

	if (list->activeElement == NULL){
		LL_Error();
	}
	else{
		*dataPtr = list->activeElement->data;
	}
}

void LL_SetValue( LList *list, int data ) {

	if (list->activeElement != NULL) {
		list->activeElement->data = data;
	}
}

int LL_IsActive(LList *list){

	return (list->activeElement != NULL) ? 1 : 0;
}