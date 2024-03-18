#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;



// You may add any prototypes of helper functions here
int getHeight(Node* node, int level){

    if (node == NULL ){
        return level; 
    }

     if (node->left == NULL){
        return getHeight(node->right, level+1);
     }
     if (node->right == NULL){
        return getHeight(node->left, level+1);
     }
     
    int leftLevel = getHeight(node->left, level+1);
    int rightLevel = getHeight(node->right, level+1); 

    if(leftLevel==rightLevel){
        return leftLevel;
    }
    return -1;


}

bool equalPaths(Node * root)
{
    // Add your code below
    int treeHeight = getHeight(root, 0);
    if(treeHeight == -1){
        return false;
    }
    return true;

}

