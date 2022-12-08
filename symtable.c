#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"


void symtable_error(){
  fprintf(stderr, "*ERROR* Allocation has failed");
  exit(99);
}

void bst_init(bst_node_t **tree) {
  if(tree != NULL){
    *tree = NULL;
  }
}

tData_t bst_search(bst_node_t *tree, char *key, tData_t *value) {
  
  if (tree == NULL){
    return NULL;
  }

  else if (strcmp(tree->key, key) == 0){
    *value = tree->value;
    return *value;
  }
  else{
    if(strcmp(key, tree->key) > 0){
      return (bst_search(tree->right, key, value));
    }
    else{
      return (bst_search(tree->left, key, value));
    }
  }
}

void bst_insert(bst_node_t **tree, char *key, tData_t value) {
  if(tree == NULL){
    return;
  }
  bst_node_t *rootPtr = *tree;

  if(rootPtr == NULL){
    rootPtr = malloc(sizeof(bst_node_t));
    if(rootPtr == NULL){
      symtable_error();
    }
    else{
      rootPtr->key = key;
      rootPtr->value = value;
      rootPtr->left= NULL;
      rootPtr->right=NULL;
    }




    if(*tree == NULL){
      *tree = rootPtr;
    }
  }
  else{
    if(strcmp(key, rootPtr->key) < 0){
      bst_insert(&rootPtr->left, key, value);
    }
    else if(strcmp(rootPtr->key, key) > 0){
      bst_insert(&rootPtr->right, key, value);
    }
    else{
      rootPtr->value = value;
    }
  }
}

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if(tree == NULL){
    return;
  }
  bst_node_t *rootPtr = *tree;
  if (rootPtr->right != NULL){
    bst_replace_by_rightmost(target, &rootPtr->right);
  }
  else{
    target->key = rootPtr->key;
    target->value = rootPtr->value;
    *tree = rootPtr->left;
    free(rootPtr);
  }

}

void bst_delete(bst_node_t **tree, char *key) {
  if(tree == NULL){
    return;
  }

  bst_node_t *rootPtr = *tree;

  if(*tree == NULL){
    return;
  }
  else if(strcmp(rootPtr->key, key) > 0){
    bst_delete(&(rootPtr->right), key);
  }
  else if(strcmp(key, rootPtr->key) < 0){
    bst_delete(&(rootPtr->left), key);
  }
  else{
    if(rootPtr->left == NULL && rootPtr->right == NULL){
      *tree = NULL;
    }
    else if(rootPtr->left == NULL && rootPtr->right != NULL){
      *tree = rootPtr->right;
    }
    else if(rootPtr->left != NULL && rootPtr->right == NULL){
      *tree = rootPtr->left;
    }
    else{
      bst_replace_by_rightmost(rootPtr, &((*tree)->left));
      return;
    }
    free(rootPtr);
  }

}

void bst_dispose(bst_node_t **tree) {
  if(tree == NULL){
    return;
  }

  if(*tree != NULL){
    bst_dispose(&((*tree)->left));
    bst_dispose(&((*tree)->right));
    free(*tree);
    *tree = NULL;
  }
}

void add_funcs(bst_node_t **tree){
  tData_t new_func = malloc(sizeof(struct tData_t));
  if(new_func == NULL){
    symtable_error();
  }
  else{
    new_func->type = FUNC;
    new_func->defined = true;
    new_func->called = false;
    new_func->local_scope = NULL;
    new_func->data_type = UNDEFINED;
    new_func->params = 0;
    new_func->func_params = malloc(sizeof(LList));
    Token t;
    strcpy(t.attribute,"term");
    t.type = T_VAR_ID;
    LL_InsertFirst(new_func->func_params, t);

    new_func->params = -1;
    bst_insert(tree, "write", new_func);
    new_func->params = 0;
    bst_insert(tree, "reads", new_func);
    bst_insert(tree, "readi", new_func);
    bst_insert(tree, "readf", new_func);
    new_func->params = 1;
    bst_insert(tree, "floatval", new_func);
    bst_insert(tree, "intval", new_func);
    bst_insert(tree, "strval", new_func);
    bst_insert(tree, "ord", new_func);
    bst_insert(tree, "chr", new_func);
    bst_insert(tree, "strlen", new_func);
    new_func->params = 3;
    bst_insert(tree, "substring", new_func);
    
  }

}