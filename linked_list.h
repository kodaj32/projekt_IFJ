#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

typedef struct LLElement {
    Token data;
    struct LLElement *nextElement;

} *ListElementPtr;

typedef struct{
    ListElementPtr firstElement;
    ListElementPtr activeElement;
}LList;

void LL_Init( LList *list );
void LL_Dispose( LList *list );
void LL_InsertFirst( LList *list, Token dataPtr );
void LL_First( LList *list );
void LL_GetFirst( LList *list, Token *dataPtr );
void LL_DeleteFirst( LList *list );
void LL_DeleteAfter( LList *list );
void LL_InsertAfter( LList *list, Token data );
void LL_Next( LList *list );
void LL_GetValue(LList *list, Token *dataPtr);
void LL_SetValue( LList *list, Token data);
int LL_IsActive(LList *list);
