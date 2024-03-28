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

/*
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
};
*/


#include <queue>
#include <set>
#include <cassert>


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    int getHeightDiff(AVLNode<Key,Value>* scan){
        int lBalance = -1;
        int rBalance =-1; 
        if(scan->getLeft()!=NULL){
            lBalance= scan->getLeft()->getBalance();
        }
        if(scan->getRight()!=NULL){
            rBalance= scan->getRight()->getBalance();
        }
        int maxBalance = lBalance;
        if(lBalance<rBalance){
            maxBalance=rBalance;
        }

        scan->setBalance(maxBalance+1);
        int balanceDiff = rBalance-lBalance;
        return balanceDiff;

    }
    AVLNode<Key,Value>* zigzig(AVLNode<Key,Value> *z){
        AVLNode<Key,Value> *y  = z->getLeft();
        AVLNode<Key,Value> *x  = y->getLeft();

        y->setParent(z->getParent());
        z->setLeft(y->getRight());
        y->setRight(z);
        z->setParent(y);

        getHeightDiff(x);
        getHeightDiff(z);
        getHeightDiff(y);

        return y;
       
    }
    AVLNode<Key,Value>* zigzag(AVLNode<Key,Value> *z){
        AVLNode<Key,Value> *y  = z->getLeft();
        AVLNode<Key,Value> *x  = y->getRight();

        y->setParent(x);
        y->setRight(x->getLeft());
        z->setLeft(x->getRight());
        x->setLeft(y);
        x->setRight(z);
        x->setParent(z->getParent());
        z->setParent(x);

        getHeightDiff(y);
        getHeightDiff(z);
        getHeightDiff(x);

        return x;
       
    }
    AVLNode<Key,Value>* zagzig(AVLNode<Key,Value> *z){
        AVLNode<Key,Value> *y  = z->getRight();
        AVLNode<Key,Value> *x  = y->getLeft();

        y->setParent(x);
        y->setLeft(x->getRight());
        z->setRight(x->getLeft());
        x->setRight(y);
        x->setLeft(z);
        x->setParent(z->getParent());
        z->setParent(x);


        getHeightDiff(y);
        getHeightDiff(z);
        getHeightDiff(x);

        return x;
       
    }
    AVLNode<Key,Value>* zagzag(AVLNode<Key,Value> *z){
        AVLNode<Key,Value> *y  = z->getRight();
        AVLNode<Key,Value> *x  = y->getRight();

        y->setParent(z->getParent());
        z->setRight(y->getLeft());
        y->setLeft(z);
        z->setParent(y);

        getHeightDiff(x);
        getHeightDiff(z);
        getHeightDiff(y);

        return y;
       
    }

    void dump() const
    {
        static int debugger = 1;
        debugger ++;
        AVLNode<Key, Value>* thisRoot = static_cast<AVLNode<Key, Value>*>(this->root_);
        // https://dreampuf.github.io/GraphvizOnline/
        std::queue<AVLNode<Key, Value>*> q;
        std::set<AVLNode<Key, Value>*> s; // Tree shouldn't have repeats
        if (thisRoot!=NULL) {
            q.push(thisRoot);
            s.insert(thisRoot);
        }
        std::cout << "\ndigraph G {";
        while (!q.empty()) {
            AVLNode<Key, Value>* n=q.front(); q.pop();
            AVLNode<Key, Value>* lr[]={n->getLeft(), n->getRight()};
            std::string color="green";
            if (n->getParent()) {
                if (n->getParent()->getLeft()==n) { color="red"; }
                std::cout << '"' << n->getKey() << ":" << (int)n->balance_ << "\" [ color="<<color<<"];";
            }
            for (int i=0; i<2; i++) {
                AVLNode<Key, Value>*c=lr[i];
                if (c==NULL)
                    continue;
                q.push(c);
                std::cout<<'"'<<n->getKey()<< ":" << (int)n->balance_ <<"\"->\""<<c->getKey() << ":" << (int)c->balance_ << "\";";
                assert(c->getParent()==n && s.find(c)==s.end());
                s.insert(c);
            }
        }
        std::cout << "}\n";
    }
};
//delete later
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
                break;
            }
            scan = scan->getLeft();
       }
       else{
            if (scan->getRight()==NULL){
                AVLNode<Key, Value> *node = new AVLNode<Key,Value>(new_item.first, new_item.second, scan);
                scan->setRight(node);
                break;
            }
            scan = scan->getRight();
       }
    }
    while(scan!=NULL){
        int balanceDiff= getHeightDiff(scan);
        if (balanceDiff<=-2){
            int left = getHeightDiff(scan->getLeft());
            if(left<0){
                dump();
                scan = zigzig(scan); 
                
                dump();
            }
            else{
                dump();
                AVLNode<Key,Value> *temp = scan->getParent();
                scan = zigzag(scan);
                if(temp!=NULL){
                    if(scan->getValue()>temp->getValue()){
                      temp->setRight(scan);
                    }
                    else{
                        temp->setLeft(scan);
                    }   
                }
                dump();
            }
        }
        if (balanceDiff>=2){
            int right= getHeightDiff(scan->getRight());
            if(right<0){
                dump();
                scan =zagzig(scan);
                dump();
            }
            else{
                dump();
                scan = zagzag(scan);
                dump();
            }
            

        }
        if(scan->getParent()==NULL){
            this->root_=scan;
        }
        scan = scan->getParent();
        dump();
    }    
    
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




#endif
