#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"

// Typ identifikatoru

typedef enum{
    VAR,                   // premenna(variable)
    FUNC,                  // funkcia(function)
} tType;

typedef enum{
    INT,                   // premenna(variable)
    FLOAT,                 // funkcia(function)
    VOID,
    UNDEFINED,
    STRING,
} tdata_Type;

//Data v uzle

typedef struct tData_t{
    tType type;                         // typ identifikatoru
    tdata_Type data_type;               // datovy typ
    bool defined;                       // pozerame ci volana funkcia bola definovana
    bool called;                        // pozerame ci definovana funkcia bola volana
    struct bst_node *local_scope;       // pointer na lokalnu tabulku
    int params;                         // pocet parametrov funkcie
    LList *func_params;                 // parametre funkcie, zoznam
} *tData_t;

// Uzol stromu
typedef struct bst_node {
  char *key;                // kľúč
  tData_t value;            // hodnota
  struct bst_node *left;    // ľavý potomok
  struct bst_node *right;   // pravý potomok
} bst_node_t;

void bst_init(bst_node_t **tree);
void bst_insert(bst_node_t **tree, char *key, tData_t value);
tData_t bst_search(bst_node_t *tree, char *key, tData_t *value);
void bst_delete(bst_node_t **tree, char *key);
void bst_dispose(bst_node_t **tree);
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);