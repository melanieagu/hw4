#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);

    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);
};

//implement rotateLeft 
template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){
  AVLNode<Key,Value>* rightChild = node->getRight(); //let RC point to node right of node 

  if (rightChild == NULL){
    return; 
  }

  AVLNode<Key,Value>* currParent = node->getParent(); //save node's existing parent 
  node->setRight(rightChild->getLeft()); //connect new RC 

  if (rightChild->getLeft() != NULL){
    rightChild->getLeft()->setParent(node); 
  }

  rightChild->setLeft(node);
  node->setParent(rightChild);

  rightChild->setParent(currParent);

  if (currParent == NULL){
    this->root_ = rightChild; 
  }
  else if (currParent->getLeft() == node){
    //node was LC 
    currParent->setLeft(rightChild);

  } else {
    //reconnect right 
    currParent->setRight(rightChild);
  }
}

//implementRotateRight
template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node) {
  AVLNode<Key,Value>* leftChild = node->getLeft();

  if (leftChild == NULL){
    return; 
  }
  AVLNode<Key,Value>* currParent = node->getParent(); //save node's existing parent

  node->setLeft(leftChild->getRight()); 

  if (leftChild->getRight ()!= NULL){
    leftChild->getRight()->setParent(node);
  }

  leftChild->setRight(node);
  node->setParent(leftChild);

  leftChild->setParent(currParent);

  if (currParent == NULL){
    this->root_ = leftChild; 
  }
  else if (currParent->getRight() == node){
    //node was RC 
    currParent->setRight(leftChild);

  } else {
    //reconnect left 
    currParent->setLeft(leftChild);
  }
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{ 
  //empty tree case 
    if (this->root_ == NULL){
      //create AVLNode 
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL); 
      return; 
    }
    //nonempty case 
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_); 

    while (current != NULL){
      if (new_item.first == current->getKey()){
        current->setValue(new_item.second);
        return; 
      } else if (new_item.first < current->getKey()){
          if (current->getLeft() == NULL){
          //attach new avl node 
          AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, current);

          current->setLeft(newNode);
          insertFix(current, newNode); 
          return; 
          } else {
          current = current->getLeft();
        }
      }
      else {
        if (current->getRight() == NULL){
          //create & attach new AVL node 
          AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
          current->setRight(newNode); 
          insertFix(current, newNode);
          return; 
        }
        else {
          current = current->getRight(); 
        }
      }
    }
}

//implement insertfix 
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
  if (parent == NULL){
    return;
  }

  AVLNode<Key, Value>* grandparent = parent->getParent();

  if (node == parent->getLeft()){
    parent->updateBalance(-1);
  } 
  else {
    parent->updateBalance(1); 
  }

  //check bal 
  if (parent->getBalance() == 0){
    return; 
  } else if (parent->getBalance() == -1 || parent->getBalance() == 1){
    insertFix(grandparent, parent); 
  } else if (parent->getBalance() == -2){
    AVLNode<Key, Value>* leftChild = parent->getLeft();
      if (leftChild->getBalance() == -1){
        //left left 
        rotateRight(parent);
        parent->setBalance(0);
        leftChild->setBalance(0);
      } else {
        //left right 
        AVLNode<Key, Value>* grandchild = leftChild->getRight();
        int8_t grandchildBalance = grandchild->getBalance(); 
        rotateLeft(leftChild); rotateRight(parent);
      
        if (grandchildBalance == -1) {
          parent->setBalance(1); leftChild->setBalance(0);
        } else if (grandchildBalance == 0){
          parent->setBalance(0); leftChild->setBalance(0); 
        } else{
         //if grandchi bal is 1 
        parent->setBalance(0);
        leftChild->setBalance(-1);
        }
        grandchild->setBalance(0);


      }
    } else if (parent->getBalance() == 2) {
      AVLNode<Key, Value>* rightChild = parent->getRight();
        if (rightChild->getBalance() == 1){
          //right right 
          rotateLeft(parent);
          parent->setBalance(0);
          rightChild->setBalance(0); 
        } else {
          //right left 
          AVLNode<Key, Value>* grandchild = rightChild->getLeft();
          int8_t grandchildBalance = grandchild->getBalance();
          rotateRight(rightChild); rotateLeft(parent);

          //other bal cases 
          if (grandchildBalance == 1){
            parent->setBalance(-1); rightChild->setBalance(0);
          } else if (grandchildBalance == 0){
            parent->setBalance(0); rightChild->setBalance(0);
          } else {
            parent->setBalance(0); rightChild->setBalance(1); 
          }
          grandchild->setBalance(0); 
        }
    }
  }


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (node == NULL){
      return; 
    }
    if (node->getLeft() != NULL && node->getRight() != NULL){
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
      nodeSwap(node,pred);
    }
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child; 
    if (node->getLeft() != NULL){
      child = node->getLeft();
    } else {
      child = node->getRight();
    }

    if (parent == NULL){
      this->root_ = child; 

      if (child != NULL){
        child->setParent(NULL); 
      }
      delete node; 
      return; 
    }

    int8_t diff;
    if (parent->getLeft() == node){
      parent->setLeft(child);
      diff = 1; 
    } else {
      parent->setRight(child);
      diff = -1;
    }

    if (child != NULL){
      child->setParent(parent);
    }
    delete node;
    removeFix(parent, diff);
}

//removefix implementation
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff){
  if (node == NULL){
    return; 
  }

  AVLNode<Key, Value>* parent = node->getParent();
  int8_t nextDiff = 0; 

  if (parent != NULL){
    if (parent->getLeft() == node){
      nextDiff = 1; 
    } else{
      nextDiff = -1; 
    }
  }

  node->updateBalance(diff);

  if (node->getBalance() == -1 || node->getBalance() == 1){
    return;
  }
  if (node->getBalance() == 0){
    removeFix(parent, nextDiff);
    return; 
  }

  if (node->getBalance() == -2 ){
    AVLNode<Key, Value>* leftChild = node->getLeft();
  
  if (leftChild->getBalance() <= 0){
    int8_t childBalance = leftChild->getBalance();
    rotateRight(node);

    if (childBalance == 0){
      node->setBalance(-1);
      leftChild->setBalance(1);
      return;
    } else {
      node->setBalance(0);
      leftChild->setBalance(0);
      removeFix(parent, nextDiff);
      return;
    }
  } else {
    AVLNode<Key, Value>* grandchild = leftChild->getRight();
    int8_t grandchildBalance = grandchild->getBalance();
    rotateLeft(leftChild); rotateRight(node);

    if (grandchildBalance == -1 ){
      node->setBalance(1); leftChild->setBalance(0);
    } else if (grandchildBalance == 0){
      node->setBalance(0); leftChild->setBalance(0); 
    } else {
      node->setBalance(0); leftChild->setBalance(-1);
    }

    grandchild->setBalance(0);
    removeFix(parent, nextDiff);
    return; 
  }
  }

  if (node->getBalance() == 2 ){
    AVLNode<Key, Value>* rightChild = node->getRight();
  
  if (rightChild->getBalance() >= 0){
    int8_t childBalance = rightChild->getBalance();
    rotateLeft(node);

    if (childBalance == 0){
      node->setBalance(1);
      rightChild->setBalance(-1);
      return;
    } else {
      node->setBalance(0);
      rightChild->setBalance(0);
      removeFix(parent, nextDiff);
      return;
    }
  } else {
    AVLNode<Key, Value>* grandchild = rightChild->getLeft();
    int8_t grandchildBalance = grandchild->getBalance();

    rotateRight(rightChild); rotateLeft(node);

    if (grandchildBalance == 1){
      node->setBalance(-1); rightChild->setBalance(0);
    } else if (grandchildBalance == 0){
      node->setBalance(0); rightChild->setBalance(0);
    } else {
      node->setBalance(0); rightChild->setBalance(1);
    }
    grandchild->setBalance(0);
    removeFix(parent, nextDiff); 
    return;
  }

}
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
