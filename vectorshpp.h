/* File: vectorshpp.h
 * ----------------------------------------------------------------
 *
 * This is a simple implementation of Vector class, which provides
 * an efficient, save, convenient replacement for the array.
 */

#ifndef VECTORSHPP_H
#define VECTORSHPP_H

#include <iostream>

/* Class: VectorSHPP
 * --------------------------------------------------------------
 *
 * This class stores an ordered list of values similar to array.
 * But it also support inserting and deletig elements.
 */
template<typename ValueType>
class VectorSHPP{

/* Public methods prototypes*/
public:

    /* Constructor: VectorSHPP
     * Usage: VectorSHPP<ValueType> vector;
     * -----------------------------------------------------
     * Initializes a new empty Vector
     */
    VectorSHPP();

    /* Destructor: ~VectorSHPP
     * -----------------------------------------------------
     * Frees any heap storage allocated by this Vector
     */
    virtual ~VectorSHPP();

    /* Method: add
     * Usage: vector.add(value);
     * -----------------------------------------------------
     * Adds a new value to the end of the list
     */
    void add(ValueType);

    /* Method: clear
     * Usage: vector.clear();
     * -----------------------------------------------------
     * Removes all elements in the Vector
     */
    void clear();

    /* Method: get
     * Usage: value = vector.get(index);
     * ------------------------------------------------------
     * Returns elements stored at specified index of this VectorSHPP
     */
    ValueType get(int) const;

    /* Method: insert
     * Usage: vector.insert(index, value);
     * ------------------------------------------------------
     * Inserts value in the specified index of VectorSHPP. All
     * subsequent elements are shifted one position to the right.
     */
    void insert(int, ValueType);

    /* Method: isEmpty
     * Usage: if(vector.isEmpty())...
     * -----------------------------------------------------
     * Returns true, if there are no elements in the VectorSHPP
     */
    bool isEmpty() const;

    /* Method: remove
     * Usage: vector.remove(index);
     * -----------------------------------------------------
     * Removes element at specified position. All subsequent
     * elements are shifted one position to the left.
     */
    void remove(int);

    /* Method: set
     * Usage: vector.set(index. value);
     * ----------------------------------------------------
     * Changes the value of the specified element of the VectorSHPP
     */
    void set(int, ValueType);

    /* Method: size
     * Usage: int size = vector.size();
     * ----------------------------------------------------
	 *
     * Returns current number of the elements.
     */
    int size() const;

    /* Operator: []
     * Usage: vector[index];
     * -----------------------------------------------------
     * Overloads [] operator for compatibility VectorSHPP with
     * standart arrays.
     */
    const ValueType& operator[](int) const;

    /* Copy constructor*/
    VectorSHPP(const VectorSHPP<ValueType> & src);

    /* Operator: =
     * vectorNew = vectorOld;
     * -----------------------------------------------------
     * Overloads assign operator
     */
    VectorSHPP<ValueType> & operator=(const VectorSHPP<ValueType> & src);

/* Private methods prototypes and instase variables*/
private:

    /* Dynamic array for storing elements*/
    ValueType *array;

     /* Current size of the dynamic array*/
    int currentSize;

    /* Current number of the element in array*/
    int count;

    /* Initial size of the array*/
    static const int START_SIZE = 10;

    /* Method: extendArray
     * Usage: extendArray();
     * ------------------------------------------------
     * Increases dynamyc arra in two times
     */
    void extendArray();

    /* Method: deepCoping;
     * Usage: deepCoping(VectorSHPP src);
     * ------------------------------------------------
     * Coping received  VectorSHPP to "this" VectorSHPP
     */
    void deepCoping(const VectorSHPP<ValueType> & src);

};


template<typename ValueType>
VectorSHPP<ValueType>::VectorSHPP(){
    array = new ValueType[START_SIZE];
    currentSize = START_SIZE;
    count = 0;
}


template<typename ValueType>
void VectorSHPP<ValueType>::deepCoping(const VectorSHPP<ValueType> &src){
    this->array = new ValueType[src.currentSize];
    count = src.count;
    currentSize = src.currentSize;
    for (int i = 0; i < src.count; i++){
        array[i] = src.array[i];
    }
}

template<typename ValueType>
VectorSHPP<ValueType>::VectorSHPP(const VectorSHPP<ValueType> & src){
    deepCoping(src);
}

template<typename ValueType>
VectorSHPP<ValueType> & VectorSHPP<ValueType>::operator =(const VectorSHPP<ValueType> & src){
    delete[] array;
    deepCoping(src);
    return *this;
}

template<typename ValueType>
const ValueType & VectorSHPP<ValueType>::operator[](int index) const {
    if(index < 0 || index >= count){
        std::cout << "operator[]::Fatal error: index is not valid" << std::endl;
        exit(1);
    }

    return array[index];
}

template<typename ValueType>
void VectorSHPP<ValueType>::add(ValueType value){
    if (count == currentSize) extendArray();

    array[count] = value;
    count++;
}

template <typename ValueType>
void VectorSHPP<ValueType>::clear(){
    count = 0;
}

template <typename ValueType>
ValueType VectorSHPP<ValueType>::get(int index) const{
    if(index < 0 || index >= count){
        std::cout << "get::Fatal error: index is not valid" << std::endl;
        exit(1);
    }

    return array[index];
}

template <typename ValueType>
void VectorSHPP<ValueType>::insert(int index, ValueType value){
    if (count == currentSize) extendArray();
    if(index < 0 || index >= count){
        std::cout << "insert::Fatal error: index is not valid" << std::endl;
        exit(1);
    }

    ValueType *tmpArray = new ValueType[count - index];
    int j = 0;
    for(int i = index; i < count; i++){
        tmpArray[j] = array[i];
        j++;
    }
    j = 0;
    array[index] = value;
    for (int i = index + 1; i <= count; i++){
        array[i] = tmpArray[j];
        j++;
    }
    count++;

}

template <typename ValueType>
bool VectorSHPP<ValueType>::isEmpty() const{
    return count == 0;
}

template <typename ValueType>
void VectorSHPP<ValueType>::remove(int index){
    if(index < 0 || index >= count){
        std::cout << "remove::Fatal error: index is not valid" << std::endl;
        exit(1);
    }
    for(int i = index; i < count-1; i++){
        array[i] = array[i+1];
    }
    count--;
}


template <typename ValueType>
void VectorSHPP<ValueType>::set(int index, ValueType value){
    if(index < 0 || index >= count){
        std::cout << "set::Fatal error: index is not valid" << std::endl;
        exit(1);
    }
    array[index] = value;
}

template <typename ValueType>
int VectorSHPP<ValueType>::size() const {
    return count;
}

template <typename ValueType>
void VectorSHPP<ValueType>::extendArray(){
    ValueType *oldArray = array;
    currentSize *= 2;
    array = new ValueType[currentSize];

    for (int i = 0; i < count; i++){
        array[i] = oldArray[i];
    }
    delete[] oldArray;
}

template<typename ValueType>
VectorSHPP<ValueType>::~VectorSHPP(){
    //delete[] array;
}

#endif // VECTORSHPP

