#include <stdio.h>
#include <stdlib.h>

typedef struct LLElement {
    void *dataPtr;
    struct LLElement *nextElement;

} *ListElementPtr;

typedef struct{
    ListElementPtr firstElement;
    ListElementPtr activeElement;
}LList;

void LL_Init( LList *list );
void LL_Dispose( LList *list );
void LL_InsertFirst( LList *list, void *dataPtr );
void LL_First( LList *list );
void LL_GetFirst( LList *list, void *dataPtr );
void LL_DeleteFirst( LList *list );
void LL_DeleteAfter( LList *list );
void LL_InsertAfter( LList *list, void *data );
void LL_Next( LList *list );
void LL_GetValue(LList *list, void *dataPtr);
void LL_SetValue( LList *list, void *data );
int LL_IsActive(LList *list);
