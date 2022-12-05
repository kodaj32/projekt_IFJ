#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

void bst_local_init(bst_node_t **tree) {
  if(tree != NULL){
    *tree = NULL;
  }
}

void bst_global_init(bst_node_t **tree) {
  if(tree != NULL){
    *tree = NULL;
  }
}

tData_t bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL){
    return false;
  }

  else if (tree->key == key){
    *value = tree->value;
    return true;
  }
  else{
    if(key > tree->key){
      return (bst_search(tree->right, key, value));
    }
    else{
      return (bst_search(tree->left, key, value));
    }
  }
}

int bst_insert(bst_node_t **tree, char key, int value) {
  if(tree == NULL){
    return;
  }
  bst_node_t *rootPtr = *tree;

  if(rootPtr == NULL){
    rootPtr = malloc(sizeof(bst_node_t));
    if(rootPtr == NULL){
      return;
    }
    rootPtr->key = key;
    rootPtr->value = value;
    rootPtr->left= NULL;
    rootPtr->right=NULL;


    if(*tree == NULL){
      *tree = rootPtr;
    }
  }
  else{
    if(key < rootPtr->key){
      bst_insert(&rootPtr->left, key, value);
    }
    else if(rootPtr->key < key){
      bst_insert(&rootPtr->right, key, value);
    }
    else{
      rootPtr->value = value;
    }
  }
}

void bst_delete(bst_node_t **tree, char key) {
  if(tree == NULL){
    return;
  }

  bst_node_t *rootPtr = *tree;

  if(*tree == NULL){
    return;
  }
  else if(rootPtr->key < key){
    bst_delete(&(rootPtr->right), key);
  }
  else if(key < rootPtr->key){
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