#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"

// Type of identifier

typedef enum{
    VAR,                   //variable
    FUNC,                  //function
} tType;

// Data type

typedef enum{
    INT,                   
    FLOAT,                 
    VOID,
    UNDEFINED,
    STRING,
} tdata_Type;

//Data in node

typedef struct tData_t{
    tType type;                         // type of identifier
    tdata_Type data_type;               // data type
    bool defined;                       // true if the function was defined, otherwise false
    bool called;                        // true if function was called otherwise false
    struct bst_node *local_scope;       // pointer to local table
    int params;                         // count of function params
    LList *func_params;                 // list of function params
} *tData_t;

// Uzol stromu
typedef struct bst_node {
  char *key;                // key to determine where to insert, find or delete
  tData_t value;            // data
  struct bst_node *left;    // left child
  struct bst_node *right;   // right child
} bst_node_t;

/**
 * Initialization of binary search tree
*/
void bst_init(bst_node_t **tree);

/**
 * Inserting node into BST with key and storing data. Key decides by its value in ASCII if it goes left/right
*/
void bst_insert(bst_node_t **tree, char *key, tData_t value);

/**
 * Searching in BST with key and returns data
*/
tData_t bst_search(bst_node_t *tree, char *key, tData_t *value);

/**
 * Delete the node from the BST by its okay together with data
*/
void bst_delete(bst_node_t **tree, char *key);

/**
 * Free the whole tree
*/
void bst_dispose(bst_node_t **tree);

/**
 * Function used in delete. Find the rightmost node
*/
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);

/**
 * Add predefined function into BST
*/
void add_funcs(bst_node_t **tree);

/**
 * Error printing to stderr when allocation fails
*/
void symtable_error();