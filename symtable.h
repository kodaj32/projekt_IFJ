#include <stdlib.h>
#include <stdbool.h>

// Typ identifikatoru

typedef enum{
    VAR,                   // premenna(variable)
    FUNC,                  // funkcia(function)
} tType;

//Data v uzle

typedef struct tData{
    tType type;                         // typ identifikatoru
    bool defined;                       // pozerame ci volana funkcia bola definovana
    bool called;                        // pozerame ci definovana funkcia bola volana
    struct bst_node *local_scope;       // pointer na lokalnu tabulku
    int params;                         // pocet parametrov funkcie
} *tData_t;

// Uzol stromu
typedef struct bst_node {
  char key;               // kľúč
  int value;              // hodnota
  struct bst_node *left;  // ľavý potomok
  struct bst_node *right; // pravý potomok
} bst_node_t;

void bst_local_init(bst_node_t **tree);
void bst_global_init(bst_node_t **tree);
int bst_insert(bst_node_t **tree, char key, int value);
tData_t bst_search(bst_node_t *tree, char key, int *value);
void bst_delete(bst_node_t **tree, char key);
void bst_dispose(bst_node_t **tree);