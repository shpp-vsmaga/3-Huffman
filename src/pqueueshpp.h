/* File: pqueueshpp.h
 * -----------------------------------------------------
 * This file exports a simple version of the Priority Queue class
 * based on two Vectors.
 */
#ifndef PQUEUESHPP_H
#define PQUEUESHPP_H

#include <iostream>
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
    /* Constructor: PQueueSHPP
     * Usage: PQueueSHPP<ValueType> pqueue;
     * -----------------------------------------------
     * Initializes a new empty priority queue
     */
    PQueueSHPP();

    /* Destructor: ~PQueueSHPP
     * ----------------------------------------------
     * Frees all allocated memory for the queue elements
     */
    virtual ~PQueueSHPP();

    /* Method: enqueue
     * Usage: pqueue.enqueue(value, priority);
     * -----------------------------------------------
     * Adds new element to the queue with specified value
     * and priority.
     */
    void enqueue(ValueType, double);

    /* Method; dequeue
     * Usage: value = pqueue.dequeue();
     * -----------------------------------------------
     * Removes element with highest priority and returns
     * it's value
     */
    ValueType dequeue();

    /* Method: peek
     * Usage: value = pqueue.peek();
     * -------------------------------------------------
     * Returns value of the element with hightest priority
     * without removing it
     */
    ValueType peek();

    /* Method: changePriority
     * Usage: pqueue.changePriority(value, newPriority);
     * --------------------------------------------------
     * Change the priority of the specified value
     */
    void changePriority(ValueType, double);

    /* Method: peekPriority
     * Usage: double priority = pqueue.peekProirity();
     * ------------------------------------------------
     * Returns the highest priority currently present in
     * the queue
     */
    double peekPriority();

    /* Method: clear
     * Usage: pqueue.clear();
     * -----------------------------------------------
     * Removes all elements in the queue
     *
     */
    void clear();

    /* Method: isEmpty
     * Usage: if (pqueue.isEmpty())...
     * ----------------------------------------------
     * Returns true if queue is empty
     */
    bool isEmpty();

    /* Method: size
     * Usage: int size = pqueue.size();
     * ----------------------------------------------
     * Returns current number of the elements
     * in the queue
     */
    int size();

private:

   VectorSHPP<double> priorities;

   VectorSHPP<ValueType> values;

   int findIndex(ValueType);


};

template<typename ValueType>
PQueueSHPP<ValueType>::PQueueSHPP(){}


template<typename ValueType>
PQueueSHPP<ValueType>::~PQueueSHPP(){}


template<typename ValueType>
void PQueueSHPP<ValueType>::enqueue(ValueType value, double priority){
    bool trigger = true;
    for(int i = 0; i < priorities.size(); i++){
        if(priority < priorities[i]){
            priorities.insert(i, priority);
            values.insert(i, value);
            trigger = false;
            break;
        }
    }

    if(trigger){
        priorities.add(priority);
        values.add(value);
    }
}

template<typename ValueType>
ValueType PQueueSHPP<ValueType>::dequeue(){
    ValueType tmpValue = values[0];
    values.remove(0);
    priorities.remove(0);
    return tmpValue;
}

template<typename ValueType>
ValueType PQueueSHPP<ValueType>::peek(){
    return values[0];
}

template<typename ValueType>
void PQueueSHPP<ValueType>::changePriority(ValueType value, double newPriority){
    int index = findIndex(value);
    if(index != -1){
    values.remove(index);
    priorities.remove(index);
    enqueue(value, newPriority);
    } else {
        std::cout << "Fatal error: value is not found!" << std::endl;
        exit(1);
    }
}

template<typename ValueType>
double PQueueSHPP<ValueType>::peekPriority(){
    return priorities[0];
}

template<typename ValueType>
void PQueueSHPP<ValueType>::clear(){
    priorities.clear();
    values.clear();
}

template<typename ValueType>
bool PQueueSHPP<ValueType>::isEmpty(){
    return values.isEmpty();
}

template<typename ValueType>
int PQueueSHPP<ValueType>::size(){
    return values.size();
}


template<typename ValueType>
int PQueueSHPP<ValueType>::findIndex(ValueType value){
    for(int i = 0; i < values.size(); i++){
        if(value == values[i]) return i;
    }
    return -1;
}

#endif // PQUEUESHPP

