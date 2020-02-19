#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <iostream>
#include <vector>
#include <list>

#include "ItemStack.h"

/**
 * An Inventory is composed of n slots. Each slot may store only
 * one type of item--specified by *slots*.
 * <p>
 * Once all slots are filled, no additional Item types may be
 * stored. Individual slots may contain any number of the same
 * Item.
 */
class Inventory {
    public:

        /**
         * Aliases for the different possible containers
         * <p>
         *  After your code is working, switch between them.
         * You should have identical results for all of them.
         */
        using ItemStackCollection = std::list<ItemStack>;
        // using ItemStackCollection = std::vector<ItemStack>;

        using iterator = ItemStackCollection::iterator;
        using const_iterator = ItemStackCollection::const_iterator;

    private:
        /**
         * All `ItemStack`s in _this_ `Inventory`
         */
        ItemStackCollection allItemStacks;
        int slots;    ///< Capacity

    public:
        /**
         * Default to 10 slots
         */
        Inventory();

        /**
         * Create an inventory with n slots
         *
         * @pre n > 0
         */
        Inventory(int n);

        /**
         * Duplicate an existing Inventory
         */
        Inventory(const Inventory& src);

        /**
         * Empty all Inventory slots.
         */
        ~Inventory();

        /**
         * Add one or more items to the inventory list
         *
         * @return true if *stack* was added and false otherwise
         */
        bool addItems(ItemStack itemStack);

        /**
         * Check the number of used/utilized (i.e., non-empty).
         */
        int utilizedSlots() const;

        /**
         * Check the number of unused (i.e., empty) slots.
         */
        int emptySlots() const;

        /**
         * Retrieve the total size (number of slots in total).
         */
        int totalSlots() const;

        /**
         * Check if this inventory is full
         *
         * @return (occupied < slots) // **technically a typo**
         */
        bool isFull() const;

        /**
         * Print a Summary of the Inventory and all Items contained within
         */
        void display(std::ostream& outs) const;

        // Begin Iterator Support (begin/end)
        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;
        // End Iterator Support

        /**
         *
         */
        Inventory& operator=(Inventory rhs);

        /**
         * Swap the contents of two `Inventory`s
         * (Yes this should be spelled "ies"). However, we
         * need to recognize that Inventory is the type of both
         * `lhs` and `rhs`
         * <p>
         * I am using a friend function here and only here (under protest)
         * <p>
         * [Refer here](http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)
         */
        friend
        void swap(Inventory& lhs, Inventory& rhs);

    private:

        /**
         * Find Node containing and ItemStack with a matching id
         *
         * @param itemStack ItemStack for which we want a match
         *
         * @return pointer to a Node containing a matching ItemStack
         *     or nullptr if no such Node exists
         */
        iterator findMatchingItemStackIterator(const ItemStack& itemStack);

        /**
         * Add a new ItemStack to an empty Inventory slot.
         * <p>
         * This is simliar to the code we discussed in Review-02
         *
         * When this method is invoked all special cases
         * have already been covered in `addItems`.
         * <p>
         * Abstraction and Interfaces
         */
        void addItemStackNoCheck(ItemStack itemStack);

    public:
        /**
         * Merge two item stacks.
         *
         * @param lhs item stack where items need to be `add`ed
         * @param rhs item stack with the *number* of items to add
         *
         * @pre lhs.item == rhs.item
         */
        static void mergeStacks(ItemStack& lhs, const ItemStack& rhs);
};

//------------------------------------------------------------------------------
inline
bool Inventory::addItems(ItemStack itemStack)
{
    iterator matchingIterator = findMatchingItemStackIterator(itemStack);

    // A match was found
    if (matchingIterator != this->end()){
        Inventory::mergeStacks(*matchingIterator, itemStack);

        return true;
    }

    // There is no space for a new type of `ItemStack`
    if (this->isFull()) {
        return false;
    }

    // This is a new type of item and there is plenty of room
    addItemStackNoCheck(itemStack);
    return true;
}

/**
 * Print the Inventory through use of the display member function
 */
inline
std::ostream& operator<<(std::ostream& outs, const Inventory& prt)
{
    prt.display(outs);
    return outs;
}

/**
 * Compare two Inventory objects, without direct access
 */
bool operator==(const Inventory& lhs, const Inventory& rhs);

#endif
