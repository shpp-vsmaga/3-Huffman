/* File:: mapshpp.h
 * --------------------------------------------------
 * This interface exports a simplified version of
 * the Map class without sorting by keys.
 */
#ifndef MAPSHPP
#define MAPSHPP

#include <iostream>
#include "vectorshpp.h"

/* Class: MapSHPP
 * -------------------------------------------------
 * This class implements a simple unordered  associative
 * array that contains key-value pairs with unique keys.
 */
template<typename KeyType, typename ValueType>
class MapSHPP{

/* Public methods prototypes*/
public:

    /* Constructor: MapSHPP
     * Usage: MapSHPP<KeyType, ValueType> map;
     * -----------------------------------------------------
     * Initializes a new empty MapSHPP
     */
    MapSHPP();

    /* Destructor: ~MapSHPP*/
    virtual ~MapSHPP();

    /* Method: put
     * Usage: map.put(key, value);
     * ----------------------------------------------------
     * Adds a new element to the map with specified index
     * and value. If such index is present in the map, this
     * method changes it's value
     */
    void put(KeyType, ValueType);

    /* Method: get
     * Usage: value = map.get(key);
     * ---------------------------------------------------
     * Returns value of the element with specified key.
     */
    ValueType get(KeyType key);

    /* Method: isEmpty
     * Usage: if (map.isEmpty())...
     * ----------------------------------------------------
     * Returns true if there are no elements in the map`
     */
    bool isEmpty();

    /* Method: clear
     * Usage: map.clear();
     * ----------------------------------------------------
     * Removes all elements in the map
     */
    void clear();

    /* Method: size
     * Usage: int size = map.size();
     * ----------------------------------------------------
     * Returns current number of the elements if the map
     */
    int size();

    /* Method: remove
     * Usage: map.remove(key);
     * ----------------------------------------------------
     * Removes key-value pair with spevified key
     */
    void remove(KeyType);

    /* Method: getKey
     * Usage: key = map.getKey(index);
     * ----------------------------------------------------
     * Returns key with specified inside index.
     * Created as parody of iteration through the elements
     * of the MapSHPP.
     */
    KeyType getKey(int);

    /* Method: containsKey
     * Usage: if (map.containsKey(key))...
     * ---------------------------------------------------
     * Returns true if specified key exist in the map
     */
    bool containsKey(KeyType key);

    /* Operator: []
     * Usage: map[key] = value;
     * ---------------------------------------------------
     * Returns link to value by the specfied key.
     */
    ValueType& operator[](KeyType);

/* Private methods prototypes and instase variables*/
private:

    /* Structure for storing key-value pairs*/
    struct Element {
        KeyType Key;
        ValueType Value;
    };

    /* Interal structure of the map  - Vector with pairs*/
    VectorSHPP<Element> vector;

    /* Method: findKey
     * Usage: index = findKey(key);
     * ---------------------------------------------------
     * Returns index of the specified key or (-1) if key
     * not found
     */
    int findKey(KeyType);
};


template<typename KeyType, typename ValueType>
MapSHPP<KeyType, ValueType>::MapSHPP(){

}

template<typename KeyType, typename ValueType>
MapSHPP<KeyType, ValueType>::~MapSHPP(){

}

template<typename KeyType, typename ValueType>
void MapSHPP<KeyType, ValueType>::put(KeyType key, ValueType value){
    int index = findKey(key);
    if(index == -1){
        Element tmpElem;
        tmpElem.Key = key;
        tmpElem.Value = value;
        vector.add(tmpElem);

    } else {
        vector[index].Value = value;

    }
}



template<typename KeyType, typename ValueType>
int MapSHPP<KeyType, ValueType>::findKey(KeyType key){
    //std::cout << key << std::endl;
    for(int i = 0; i < vector.size(); i++){
        if (vector[i].Key == key) return i;
    }
    return -1;
}

template<typename KeyType, typename ValueType>
ValueType MapSHPP<KeyType, ValueType>::get(KeyType key){

        int index = findKey(key);
        if(index == -1){
            return ValueType();
        }

        return vector[index].Value;

}

template<typename KeyType, typename ValueType>
int MapSHPP<KeyType, ValueType>::size(){
    return vector.size();
}

template<typename KeyType, typename ValueType>
void MapSHPP<KeyType, ValueType>::remove(KeyType key){
    int index = findKey(key);
    if(index == -1){
        std::cout << "(remove)Key: "<< key<<" not found"<< std::endl;
        exit(1);
    }

    vector.remove(index);
}

template<typename KeyType, typename ValueType>
bool MapSHPP<KeyType, ValueType>::isEmpty(){
    return vector.size() == 0;
}

template<typename KeyType, typename ValueType>
void MapSHPP<KeyType, ValueType>::clear(){
    vector.clear();
}

template<typename KeyType, typename ValueType>
KeyType MapSHPP<KeyType, ValueType>::getKey(int index){
    return vector[index].Key;
}

template<typename KeyType, typename ValueType>
bool MapSHPP<KeyType, ValueType>::containsKey(KeyType key){
    int index = findKey(key);
    if (index == -1) return false;
    return true;
}

template<typename KeyType, typename ValueType>
ValueType& MapSHPP<KeyType, ValueType>::operator [](KeyType key){
    int index = findKey(key);
    if(index == -1){
        std::cout << "MapSHPP::operator[], invalid key" << std::endl;
        exit(1);
    }
    return vector[index].Value;
}

#endif // MAPSHPP

