#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkingPaths(Node* node, int currDepth, int& firstleafDepth){
  //base case (no node)
  if (node == nullptr){
    return true; 
  }
  //base case (leaf case)
  if (node->left == nullptr && node->right == nullptr){
    if (firstleafDepth == -1){
      firstleafDepth = currDepth; 
      return true; 
    }
    //then cross check 
    return currDepth == firstleafDepth; 
  }
  return checkingPaths(node->left, currDepth+1, firstleafDepth) && checkingPaths(node->right, currDepth+1, firstleafDepth); 
}


bool equalPaths(Node * root)
{
    // Add your code below
    int firstleafDepth = -1; 
    return checkingPaths(root, 0, firstleafDepth); 
}

