/* File: pqueueshpp.h
 * -----------------------------------------------------
 * This file exports a simple version of the Priority Queue class
 * based on the binary heap.
 */
#ifndef PQUEUESHPP_H
#define PQUEUESHPP_H

#include <iostream>
//#include <vector>
#include <string>
#include "vectorshpp.h"

/* Class: PQueueSHPP<ValueType>
 * ---------------------------------------------------
 * This clas implements priority queue of a specified ValueType
 * elements
 */
template<typename ValueType>
class PQueueSHPP{

    /* Public methods prototypes*/
public:
    /** Constructor: PQueueSHPP
     * Usage: PQueueSHPP<ValueType> pqueue;
     * -----------------------------------------------
     * Initializes a new empty priority queue
     */
    PQueueSHPP();

    /** Destructor: ~PQueueSHPP
     * ----------------------------------------------
     * Frees all allocated memory for the queue elements
     */
    virtual ~PQueueSHPP();

    /** Method: enqueue
     * Usage: pqueue.enqueue(value, priority);
     * -----------------------------------------------
     * Adds new element to the queue with specified value
     * and priority.
     */
    void enqueue(ValueType, double);

    /** Method; dequeue
     * Usage: value = pqueue.dequeue();
     * -----------------------------------------------
     * Removes element with highest priority and returns
     * it's value
     */
    ValueType dequeue();

    /** Method: peek
     * Usage: value = pqueue.peek();
     * -------------------------------------------------
     * Returns value of the element with hightest priority
     * without removing it
     */
    ValueType peek();


    /** Method: peekPriority
     * Usage: double priority = pqueue.peekProirity();
     * ------------------------------------------------
     * Returns the highest priority currently present in
     * the queue
     */
    double peekPriority();

    /** Method: clear
     * Usage: pqueue.clear();
     * -----------------------------------------------
     * Removes all elements in the queue
     *
     */
    void clear();

    /** Method: isEmpty
     * Usage: if (pqueue.isEmpty())...
     * ----------------------------------------------
     * Returns true if queue is empty
     */
    bool isEmpty();

    /** Method: size
     * Usage: int size = pqueue.size();
     * ----------------------------------------------
     * Returns current number of the elements
     * in the queue
     */
    int size();


    /**
     * Copy constructor
     * ---------------------------------------------
     * @param src Source instance of PQueue
     */
    PQueueSHPP(const PQueueSHPP<ValueType>& src);

    PQueueSHPP<ValueType> & operator=(const PQueueSHPP<ValueType>& src);



private:

    /* Structure for saving elements of the queue*/
    struct Node{
        ValueType value;
        double priority;
        Node* left;
        Node* right;
        Node* parent;

    };

    /*Vector for array-based representaton og the heap*/
    VectorSHPP<Node*> vec;
    int counter;

    /**
     * Method: shiftUp
     * ------------------------------------------------
     * This method used for shifting up Node in the heap
     * to the right position of the priority
     * @param node Pointer to the moved node
     */
    void shiftUp(Node* node);

    /**
     * Method: shiftDown
     * ------------------------------------------------
     * This method used for shifting down Node in the heap
     * to the right position of the priority
     * @param node Pointer to the moved node
     */
    void shiftDown(Node* node);

    /**
     * Method: replace
     * ----------------------------------------------
     * This method replace the value and priority
     * of two received nodes
     * @param node1
     * @param node2
     */
    void replace(Node* node1, Node* node2);

    /**
     * Method: deleteHeap
     * ---------------------------------------------
     * This method recursively removes all nodes of the heap
     * @param node Root node of the heap
     */
    void deleteHeap(Node* node);

    /**
     * Method: deepCopy
     * ----------------------------------------------
     * This method provides deep coping for copy constructor
     * and operator = ;
     * @param src
     */
    void deepCopy(const PQueueSHPP<ValueType> & src);

};

template<typename ValueType>
PQueueSHPP<ValueType>::PQueueSHPP(){
    counter = 0;
}


template<typename ValueType>
PQueueSHPP<ValueType>::~PQueueSHPP(){}


template<typename ValueType>
void PQueueSHPP<ValueType>::replace(Node *node1, Node *node2){
    double tmpPriority = node1->priority;
    ValueType tmpValue = node1->value;
    node1->priority = node2->priority;
    node1->value = node2->value;
    node2->priority = tmpPriority;
    node2->value = tmpValue;
}

template<typename ValueType>
void PQueueSHPP<ValueType>::shiftUp(Node *node){
    if (node->parent != 0 && node->parent->priority >= node->priority){
        replace(node, node->parent);
        shiftUp(node->parent);
    }
}

template<typename ValueType>
void PQueueSHPP<ValueType>::shiftDown(Node *node){
    if (node->left != 0 && node->right != 0){
        if (node->left->priority < node->right->priority && node->left->priority < node->priority){
            replace(node->left, node);
            shiftDown(node->left);
        } else if (node->left->priority >= node->right->priority && node->right->priority < node->priority) {
            replace(node->right, node);
            shiftDown(node->right);
        }
    } else if (node->left == 0 && node->right != 0 && node->right->priority < node->priority){
        replace(node->right, node);
        shiftDown(node->right);
    } else if (node->right == 0 && node->left != 0 && node->left->priority < node->priority){
        replace(node->left, node);
        shiftDown(node->left);
    }
}

template<typename ValueType>
void PQueueSHPP<ValueType>::deleteHeap(Node *node){
    if (node->left != 0){
        deleteHeap(node->left);
    }
    if (node->right != 0){
        deleteHeap(node->right);
    }
    delete node;
    counter = 0;
}

template<typename ValueType>
void PQueueSHPP<ValueType>::deepCopy(const PQueueSHPP<ValueType> & src){
    for (unsigned int i = 0; i < src.vec.size(); i++){
        Node* newNode = new Node;
        newNode->priority = src.vec[i]->priority;
        newNode->value = src.vec[i]->value;
        this->counter = src.counter;
        this->vec.add(newNode);
    }

    for(unsigned int i = 0; i < vec.size(); i++){

        if (2 * i + 1 < vec.size()){
            vec[i]->left = vec[2 * i + 1];
        } else {
            vec[i]->left = 0;
        }

        if (2 * i + 2 < vec.size()){
            vec[i]->right = vec[2 * i +2];
        } else {
            vec[i]->right = 0;
        }

        if( i > 0){
           vec[i]->parent = vec[(i - 1) / 2];
        } else {
            vec[i]->parent = 0;
        }
    }
}


//------------------------------------------------------------------------------

template<typename ValueType>
PQueueSHPP<ValueType>::PQueueSHPP(const PQueueSHPP<ValueType>& src){
    deepCopy(src);
}

template<typename ValueType>
PQueueSHPP<ValueType> & PQueueSHPP<ValueType>::operator =(const PQueueSHPP<ValueType>& src){
    deepCopy(src);
    return *this;
}


template<typename ValueType>
void PQueueSHPP<ValueType>::enqueue(ValueType value, double priority){
    Node* newNode = new Node;
    newNode->left = newNode->right = newNode->parent = 0;
    newNode->value = value;
    newNode->priority = priority;
    vec.add(newNode);
    counter++;
    if(vec.size() > 1){
        newNode->parent = vec[(vec.size() - 2) / 2];
        if (vec.size() % 2 == 0){
            newNode->parent->left = newNode;
        } else {
            newNode->parent->right = newNode;
        }
        shiftUp(newNode);
    }

}

template<typename ValueType>
ValueType PQueueSHPP<ValueType>::dequeue(){

    if (!isEmpty()){
        ValueType result;
        if (vec.size() > 1){
            Node* startNode = vec[0];
            Node* endNode = vec[vec.size() - 1];
            result = startNode->value;
            startNode->priority = endNode->priority;
            startNode->value = endNode->value;
            if (vec.size() % 2 == 0){
                endNode->parent->left = 0;
            } else {
                endNode->parent->right = 0;
            }
            vec.remove(vec.size() - 1);
            //vec.pop_back();
            delete endNode;
            shiftDown(startNode);
        } else if (vec.size() == 1){
            result = vec[0]->value;
            delete vec[0];
            vec.remove(0);

        }
        counter--;
        return result;
    }
   return 0;
}

template<typename ValueType>
ValueType PQueueSHPP<ValueType>::peek(){
    return vec[0]->value;
}


template<typename ValueType>
double PQueueSHPP<ValueType>::peekPriority() {
    return vec[0]->priority;
}

template<typename ValueType>
void PQueueSHPP<ValueType>::clear(){
    deleteHeap(vec[0]);
    vec.clear();
    counter = 0;
}

template<typename ValueType>
bool PQueueSHPP<ValueType>::isEmpty() {

    return (vec.size() == 0);
}

template<typename ValueType>
int PQueueSHPP<ValueType>::size() {

    return vec.size();
}




#endif // PQUEUESHPP

