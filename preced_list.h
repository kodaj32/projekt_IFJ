#ifndef PRECED_LIST_H
#define PRECED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"


typedef enum prec_type{
    PLUS,       // +
    MIN,        // -
    DOT,        // .
    DIV,        // /
    MUL,        // *
    EQ,         // ===
    NOT_EQ,     // !==
    LESS,       // <
    GREATER,    // >
    LESS_E,     // <=
    GREATER_E,  // >= 
    LEFT_BR,    // (
    RIGHT_BR,   // )
    ID,         // i
    END_MARKER, // $
}Prec_type;

typedef struct PrecLLElement {
    Prec_type data;
    struct PrecLLElement *nextElement;

} *PrecElementPtr;

typedef struct{
    PrecElementPtr firstElement;
    PrecElementPtr activeElement;
}PrecLList;

void Prec_LL_Init( PrecLList *list );
void Prec_LL_Dispose( PrecLList *list );
void Prec_LL_InsertFirst( PrecLList *list, Prec_type dataPtr );
void Prec_LL_InsertFirst_Elem(PrecLList *list, PrecElementPtr elem);
void Prec_LL_First( PrecLList *list );
void Prec_LL_GetFirst( PrecLList *list, Prec_type *dataPtr );
void Prec_LL_DeleteFirst( PrecLList *list );
void Prec_LL_DeleteAfter( PrecLList *list );
void Prec_LL_InsertAfter_Elem(PrecLList *list, PrecElementPtr elem);
void Prec_LL_InsertAfter( PrecLList *list, Prec_type data );
void Prec_LL_Next( PrecLList *list );
void Prec_LL_GetValue(PrecLList *list, Prec_type *dataPtr);
void Prec_LL_SetValue( PrecLList *list, Prec_type data);
int Prec_LL_IsActive(PrecLList *list);

#endif