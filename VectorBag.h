/** 
 * CISC 2200: Data Structures<br/>
 * Project 2: Extending the Bag ADT<br/>
 *
 * <p>Header file for a vector-based implementation of an extended Bag
 * ADT. Along with implementing the operations of the Bag ADT,
 * VectorBag also implements union, intersection, and difference.</p>
 *
 * @author Tianmao Jiang
 * @date 29 February 2016
 * @file VectorBag.h
 * @class VectorBag VectorBag is a vector-based implementation 
 *  of BagInterface
 *
 **********************************************************************/

#ifndef _VECTOR_BAG
#define _VECTOR_BAG
   
#include "BagInterface.h"
    
template<class ItemType>
class VectorBag: public BagInterface<ItemType>
{
public:
    int getCurrentSize() const;
    bool isEmpty() const;
    bool add(const ItemType& newEntry);
    bool remove(const ItemType& anEntry);
    void clear();
    bool contains(const ItemType& anEntry) const;
    int getFrequencyOf(const ItemType& anEntry) const;
    vector<ItemType> toVector() const;
    
    VectorBag<ItemType> operator+(VectorBag<ItemType> anotherBag);
    
    VectorBag<ItemType> operator*(VectorBag<ItemType> anotherBag);
    
    VectorBag<ItemType> operator-(VectorBag<ItemType> anotherBag);
    
private:
    vector<ItemType> items;      // where we store the bag
    // Returns either the index of the element in the array items that
    // contains the given target or -1, if the array does not contain 
    // the target.
    int getIndexOf(const ItemType& target) const;   
    
}; // end VectorBag
    
#include "VectorBag.cpp"
#endif
