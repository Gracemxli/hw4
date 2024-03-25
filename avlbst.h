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
     void insertFix( const AVLNode<Key, Value> *parent, const AVLNode<Key, Value> *node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_==NULL){
        AVLNode<Key, Value> *node = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
        this->root_ = node;
        return;
    }
  //if ((root_->left!=NULL&&root->right ==NULL)||if (root_->left==NULL&&root->right !=NULL))
    AVLNode<Key, Value> *scan = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(scan!=NULL){
       if(scan->getKey()==new_item.first){
            scan->setValue(new_item.second);
            return;
       }
       else if(scan->getKey()>new_item.first){
            if (scan->getLeft()==NULL){
                AVLNode<Key, Value> *node = new AVLNode<Key,Value>(new_item.first, new_item.second, scan);
                scan->setLeft(node);
                return;
            }
            scan = scan->getLeft();
       }
       else{
            if (scan->getRight()==NULL){
                AVLNode<Key, Value> *node = new AVLNode<Key,Value>(new_item.first, new_item.second, scan);
                scan->setRight(node);
                return;
            }
            scan = scan->getRight();
       }
    }
    /* If empty tree => set n as root, b(n) = 0, done!
• Else insert n (by walking the tree to a leaf, p, and
inserting the new node as its child), set balance
to 0, and look at its parent, p
– If b(p) was -1, then b(p) = 0. Done!
– If b(p) was +1, then b(p) = 0. Done!
– If b(p) was 0, then update b(p) and call insert-fix(p, n) */
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (const AVLNode<Key, Value> *parent, const AVLNode<Key, Value> *node)
{

  /* Precondition: p and n are balanced: {-1,0,-1}
• Postcondition: g, p, and n are balanced: {-1,0,-1}
• If p is null or parent(p) is null, return
• Let g = parent(p)
• Assume p is left child of g [For right child swap left/right, +/-]
– b(g) += -1 // Update g's balance to new accurate value for now
– Case 1: b(g) == 0, return
– Case 2: b(g) == -1, insertFix(g, p) // recurse
– Case 3: b(g) == -2
• If zig-zig then rotateRight(g); b(p) = b(g) = 0
• If zig-zag then rotateLeft(p); rotateRight(g);
– Case 3a: b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
– Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
– Case 3c: b(n) == +1 then b(p)= -1; b(g) = 0; b(n) = 0; */
}

#endif
