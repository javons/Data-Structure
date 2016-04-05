/**
 * CISC 2200: Data Structures<br/>
 * Project 2: Extending the Bag ADT<br/>
 *
 * <p>Implementation file for a vector-based implementation of an 
 * extended Bag ADT. Along with implementing the operations of the Bag 
 * ADT, VectorBag also implements union, intersection, and 
 * difference.</p>
 *
 * @author Tianmao Jiang
 * @date 29 February 2016
 * @file VectorBag.cpp
 *
 **********************************************************************/

#include "VectorBag.h"

/** Gets the current number of entries in this bag.
 *  @return The integer number of entries currently in the bag. 
 */
template<class ItemType>
int VectorBag<ItemType>::getCurrentSize() const {
    return items.size();
}

/** Sees whether this bag is empty.
 *  @return True if the bag is empty, or false if not.
 */
template<class ItemType>
bool VectorBag<ItemType>::isEmpty() const {
    return items.size() == 0;
}

/** Adds a new entry to this bag.
 *  @post If successful, newEntry is stored in the bag and the count 
 *   of items in the bag has increased by 1.
 *  @param newEntry The object to be added as a new entry.
 *  @return True if addition was successful, or false if not.
 */
template<class ItemType>
bool VectorBag<ItemType>::add(const ItemType& newEntry) {
    items.push_back(newEntry);
    return true;
}

/** Removes one occurrence of a given entry from this bag, if possible.
 *  @post If successful, anEntry has been removed from the bag and the
 *   count of items in the bag has decreased by 1.
 *  @param anEntry The entry to be removed.
 *  @return True if removal was successful, or false if not.
 */
template<class ItemType>
bool VectorBag<ItemType>::remove(const ItemType& anEntry) {
    int i = getIndexOf(anEntry);
    if (i < 0)
	return false;
    else {
	items.erase(items.begin() + i);
	return true;
    }
}

/** Removes all entries from this bag.
 *  @post Bag contains no items, and the count of items is 0.
 */
template<class ItemType>
void VectorBag<ItemType>::clear() {
    items.clear();
}

/** Tests whether this bag contains a given entry.
 *  @param anEntry The entry to locate.
 *  @return True if bag contains anEntry, or false otherwise.
 */
template<class ItemType>
bool VectorBag<ItemType>::contains(const ItemType& anEntry) const {
    bool found = false;
    unsigned int i = 0;
    while (!found && i < items.size()) {
	if (items[i] == anEntry)
	    found = true;
	i++;
    }   
    return found;
}

/** Counts the number of times a given entry appears in bag.
 *  @param anEntry The entry to be counted.
 *  @return The number of times anEntry appears in the bag.
 */
template<class ItemType>
int VectorBag<ItemType>::getFrequencyOf(const ItemType& anEntry) const {
    int count = 0;
    for (unsigned int i = 0; i < items.size(); i++) {
	if (items[i] == anEntry)
	    count++;
    }
    return count;
}

/** Empties and then f ills a given vector with all entries that are 
 *  in this bag.
 *  @return A vector containing all the entries in the bag.
 */    
template<class ItemType>
vector<ItemType> VectorBag<ItemType>::toVector() const {
    vector<ItemType> bagContents;
    for (unsigned int i = 0; i < items.size(); i++)
	bagContents.push_back(items[i]);
    return bagContents;
}

/** Creates a new bag that combines the contents of this bag and a 
 *   second given bag without affecting the original two bags.
 *  Note that we can't call this operation "<code>union</code>", since
 *  "<code>union</code>" is a reserved word.
 *  @param anotherBag The given bag.
 *  @return A bag that is the union of the two bags.
 */
template<class ItemType>
VectorBag<ItemType>
VectorBag<ItemType>::operator+(VectorBag<ItemType> anotherBag) {
    VectorBag<ItemType> newBag = *this;
    for (int i = 0; i < anotherBag.getCurrentSize(); i++)
	newBag.add(anotherBag.toVector()[i]);
    return newBag;
}

/** Creates a new bag that contains those objects that occur in both 
 *  this bag and a second given bag without affecting the original two
 *  bags.
 *  @param anotherBag The given bag.
 *  @return A bag that is the intersection of the two bags.
 */
template<class ItemType>
VectorBag<ItemType>
VectorBag<ItemType>::operator*(VectorBag<ItemType> anotherBag) {
    VectorBag<ItemType> newBag = *this;
    VectorBag<ItemType> result;
    int m = newBag.getCurrentSize();
    int n = anotherBag.getCurrentSize();
    int i = 0, j = 0;
    while (i < m && j < n) {
	if (newBag.toVector()[i] > anotherBag.toVector()[j])
	    j++;
	else if (newBag.toVector()[i] < anotherBag.toVector()[j])
	    i++;
	else {
	    result.add(newBag.toVector()[i]);
	    i++;
	    j++;
	}
    }
    return result;
}

/** Creates a new bag of objects that would be left in this bag after 
 *  removing those that also occur in a second given bag without 
 *  affecting the original two bags.
 *  @param anotherBag The given bag.
 *  @return A bag that is the difference of the two bags.
 */
template<class ItemType>
VectorBag<ItemType>
VectorBag<ItemType>::operator-(VectorBag<ItemType> anotherBag) {
    VectorBag<ItemType> newBag = *this;
    for (int i = 0; i < anotherBag.getCurrentSize(); i++) {
	if (newBag.contains(anotherBag.toVector()[i])) {
	    newBag.remove(anotherBag.toVector()[i]);
	}
    }
    return newBag;
}

// return the index of target item, return -1 if it fails to obtain
template<class ItemType>
int VectorBag<ItemType>::getIndexOf(const ItemType& target) const {
    int i = 0;
    if (contains(target) == false)
	i = -1;
    else {
	while (target != items[i])
	    i++;
    }
    return i;
}
	
	
	
