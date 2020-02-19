#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Item.h"
#include "ItemStack.h"
#include "Inventory.h"

/**
 * This is the Bodge-Unit-Testing... PseUdO-Framework
 *
 * Bodge - A clumsy or inelegant job, usually a temporary repair;
 * a patch, a repair. (From Wiktionary)
 */

#define bodgeAssert(expression) \
if (!(expression)) { \
    std::cout << " FAILURE: "\
              << __func__ << ":" << __LINE__\
              << " -> (" << #expression << ")\n";\
    return false;\
}
// End Macro

// Unit Test Pseudo-Framework
//-----------------------------------------------------------------------------
using UnitTestFunction = std::function<bool()>;
using UnitTestPair = std::pair<UnitTestFunction, std::string>;


void runTest(const UnitTestFunction& testFunction, std::string description)
{
    std::cout << (testFunction() ? "PASSED" : "FAILED")
              << " -> " << description
              << std::endl;
}

//-----------------------------------------------------------------------------
// Unit Tests - Support Data
//-----------------------------------------------------------------------------
const Inventory EMPTY_INVENTORY;

const std::vector<Item> TEST_ITEMS = {
    {0, "Air"},
    {1, "Dirt"},
    {2, "Potato"},
    {3, "Makefile"},
    {4, "Procrastination"}
};



//-----------------------------------------------------------------------------
// Unit Tests - Test Functions
//-----------------------------------------------------------------------------
bool testDefaultConstructor()
{
    bodgeAssert(EMPTY_INVENTORY.utilizedSlots() == 0);
    bodgeAssert(EMPTY_INVENTORY.emptySlots() == 10);
    bodgeAssert(EMPTY_INVENTORY.totalSlots() == 10);
    bodgeAssert(!EMPTY_INVENTORY.isFull());

    // I should really check display() and/or operator<< here. However, I will
    // do that in a seperate `testDisplay` function

    return true;
}

//-----------------------------------------------------------------------------
bool testConstructorSizeN()
{
    Inventory invWith8Slots(8);

    bodgeAssert(invWith8Slots.utilizedSlots() == 0);
    bodgeAssert(invWith8Slots.emptySlots() == 8);
    bodgeAssert(invWith8Slots.totalSlots() == 8);
    bodgeAssert(!invWith8Slots.isFull());

    // I should really check display() and/or operator<< here. However, I will
    // do that in a seperate `testDisplay` function

    return true;
}

//-----------------------------------------------------------------------------
/**
 * Add ItemStacks to an Inventory without filling the Inventory or attempting
 * to add duplicate Items
 */
bool testAddItemStackNoCheck()
{
    std::vector<ItemStack> stacksToAdd = {
        {TEST_ITEMS[0], 1},
        {TEST_ITEMS[1], 4},
        {TEST_ITEMS[2], 8}
    };

    Inventory aBag(4);

    aBag.addItems(stacksToAdd[0]);
    aBag.addItems(stacksToAdd[1]);
    aBag.addItems(stacksToAdd[2]);

    bodgeAssert(!aBag.isFull());
    bodgeAssert(aBag.utilizedSlots() == 3);
    bodgeAssert(aBag.emptySlots() == 1);
    bodgeAssert(aBag.totalSlots() == 4);

    // Retrieve each of the items and check that they were added
    Inventory::const_iterator it = aBag.begin();

    bodgeAssert(*(it++) == stacksToAdd[0]);
    bodgeAssert(*(it++) == stacksToAdd[1]);
    bodgeAssert(*(it++) == stacksToAdd[2]);

    // Check that there are no more ItemStacks to retrieve
    bodgeAssert(it == aBag.end());

    return true;
}

/**
 * Add ItemStacks to an Inventory without filling the Inventory or attempting
 * to add duplicate Items
 */
bool testAddItemWithDuplicateItems()
{
    std::vector<ItemStack> stacksToAdd = {
        {TEST_ITEMS[0], 1},
        {TEST_ITEMS[1], 4},
        {TEST_ITEMS[1], 5}
    };

    Inventory aBag(4);

    aBag.addItems(stacksToAdd[0]);
    aBag.addItems(stacksToAdd[1]);
    aBag.addItems(stacksToAdd[2]);

    bodgeAssert(!aBag.isFull());
    bodgeAssert(aBag.utilizedSlots() == 2);
    bodgeAssert(aBag.emptySlots() == 2);
    bodgeAssert(aBag.totalSlots() == 4);

    // Retrieve each of the items and check that they were added
    Inventory::const_iterator it = aBag.begin();

    bodgeAssert(*(it++) == stacksToAdd[0]);

    // Expect the merged stack to be returned.
    ItemStack mergedStack(TEST_ITEMS[1], 9);
    bodgeAssert(*(it++) == mergedStack);

    // Check that there are no more ItemStacks to retrieve
    bodgeAssert(it == aBag.end());

    return true;
}

/**
 * Add ItemStacks to an Inventory and fill it.
 * Then try to add one more ItemStack.
 */
bool testAddItemAfterFull()
{
    std::vector<ItemStack> stacksToAdd = {
        {TEST_ITEMS[0], 1},
        {TEST_ITEMS[1], 4},
        {TEST_ITEMS[4], 8}
    };

    Inventory aBag(2);

    aBag.addItems(stacksToAdd[0]);
    aBag.addItems(stacksToAdd[1]);

    bodgeAssert(aBag.isFull());
    bodgeAssert(aBag.utilizedSlots() == 2);
    bodgeAssert(aBag.emptySlots() == 0);
    bodgeAssert(aBag.totalSlots() == 2);

    // This add should fail
    // Procrastination is bad
    bodgeAssert(!(aBag.addItems(stacksToAdd[2])));

    // Retrieve each of the items and check that they were added
    Inventory::const_iterator it = aBag.begin();

    bodgeAssert(*(it++) == stacksToAdd[0]);
    bodgeAssert(*(it++) == stacksToAdd[1]);

    // Check that there are no more ItemStacks to retrieve
    bodgeAssert(it == aBag.end());

    return true;
}

//-----------------------------------------------------------------------------
/**
 * Helper function for testDisplay.
 *
 * Convert any type with an operator<< defined to a std::string
 */
template<class T>
std::string toStr(const T& thing)
{
    std::ostringstream outs;
    outs << thing;

    return outs.str();
}

//-----------------------------------------------------------------------------
bool testDisplay()
{
    std::vector<ItemStack> stacksToAdd = {
        {TEST_ITEMS[0], 1},
        {TEST_ITEMS[1], 4}
    };

    // Set up the expected strings for each ItemStack
    std::vector<std::string> stacksAsStrings(stacksToAdd.size());

    for (int i = 0; i < stacksToAdd.size(); i++) {
        stacksAsStrings[i] = toStr(stacksToAdd[i]);
    }

    // Set up the test Inventory
    Inventory aBag(2);
    aBag.addItems(stacksToAdd[0]);
    aBag.addItems(stacksToAdd[1]);

    // Check for the expected ItemStack lines within the larger output
    const std::string bagString = toStr(aBag);

    bodgeAssert(bagString.find(stacksAsStrings[0]) != std::string::npos);
    bodgeAssert(bagString.find(stacksAsStrings[1]) != std::string::npos);

    // Check for the Summary line
    const std::string expected = "-Used " + std::to_string(aBag.utilizedSlots())
                               + " of " + std::to_string(aBag.totalSlots())
                               + " slots";

    bodgeAssert(toStr(aBag).find(expected) != std::string::npos);

    //--------------------------------------------------------------------------
    // Check the entire output string. This should really be a seperate test
    //--------------------------------------------------------------------------
    std::string expectedOverall = " " + expected + "\n"
                                + "  " + stacksAsStrings[0] + "\n"
                                + "  " + stacksAsStrings[1] + "\n";

    bodgeAssert(toStr(aBag) == expectedOverall);

    return true;
}

//-----------------------------------------------------------------------------
bool testCopyConstructorForEmpty()
{
    Inventory aCopy = EMPTY_INVENTORY;

    bodgeAssert(aCopy.utilizedSlots() == 0);
    bodgeAssert(aCopy.emptySlots() == 10);
    bodgeAssert(aCopy.totalSlots() == 10);
    bodgeAssert(!aCopy.isFull());

    // Check that both have the same data and are distinct copies
    bodgeAssert(aCopy == EMPTY_INVENTORY);
    bodgeAssert(&aCopy != &EMPTY_INVENTORY);

    // bodgeAssert(aCopy.begin() != EMPTY_INVENTORY.begin());

    return true;
}

//-----------------------------------------------------------------------------
bool testCopyConstructor()
{
    std::vector<ItemStack> stacksToAdd = {
        {TEST_ITEMS[0], 1},
        {TEST_ITEMS[1], 4},
        {TEST_ITEMS[4], 8}
    };

    Inventory source(4);

    for (const ItemStack& next : stacksToAdd) {
        source.addItems(next);
    }

    Inventory aCopy(source);

    bodgeAssert(aCopy.utilizedSlots() == 3);
    bodgeAssert(aCopy.emptySlots() == 1);
    bodgeAssert(aCopy.totalSlots() == 4);
    bodgeAssert(!aCopy.isFull());

    // Check that both have the same data and are distinct copies
    bodgeAssert(aCopy == source);
    bodgeAssert(&aCopy != &source);

    using ConstIterator = Inventory::const_iterator;
    ConstIterator it = source.begin();

    for (const ItemStack& nextToCheck : aCopy) {
        bodgeAssert(nextToCheck == *it);          // Compare ItemStacks
        bodgeAssert(&nextToCheck != &(*(it++)));  // Compare Memory Addresses
    }

    return true;
}

//-----------------------------------------------------------------------------
bool testAssignmentOperator()
{
    std::vector<ItemStack> stacksToAdd = {
        {TEST_ITEMS[0], 1},
        {TEST_ITEMS[1], 4},
        {TEST_ITEMS[4], 8}
    };

    Inventory source(4);

    for (const ItemStack& next : stacksToAdd) {
        source.addItems(next);
    }

    Inventory aCopy = source;

    bodgeAssert(aCopy.utilizedSlots() == 3);
    bodgeAssert(aCopy.emptySlots() == 1);
    bodgeAssert(aCopy.totalSlots() == 4);
    bodgeAssert(!aCopy.isFull());

    // Check that both have the same data and are distinct copies
    bodgeAssert(aCopy == source);
    bodgeAssert(&aCopy != &source);

    using ConstIterator = Inventory::const_iterator;
    ConstIterator it = source.begin();

    for (const ItemStack& nextToCheck : aCopy) {
        bodgeAssert(nextToCheck == *it);          // Compare ItemStacks
        bodgeAssert(&nextToCheck != &(*(it++)));  // Compare Memory Addresses
    }

    return true;
}

//-----------------------------------------------------------------------------
bool testMergeStacks()
{
    // The Inventory mergeStacks function deals only with ItemStack objects. It is a utility
    // function of Inventory.

    ItemStack stack1(TEST_ITEMS[0], 12);
    ItemStack stack2(TEST_ITEMS[0], 1);
    ItemStack stack3(TEST_ITEMS[0], 0);
    ItemStack stack4(TEST_ITEMS[0], 5);

    Inventory::mergeStacks(stack1, stack2);
    bodgeAssert(stack1.size() == 13);

    Inventory::mergeStacks(stack3, stack4);
    bodgeAssert(stack3.size() == 5);

    Inventory::mergeStacks(stack1, stack3);
    bodgeAssert(stack1.size() == 18);

    return true;
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    UnitTestPair tests[] = {
        {testDefaultConstructor, "testDefaultConstructor"},
        {testConstructorSizeN, "testConstructorSizeN"},
        {testAddItemStackNoCheck, "testAddItemStackNoCheck"},
        {testAddItemWithDuplicateItems, "testAddItemWithDuplicateItems"},
        {testAddItemAfterFull, "testAddItemAfterFull"},
        {testCopyConstructorForEmpty, "testCopyConstructorForEmpty"},
        {testCopyConstructor, "testCopyConstructor"},
        {testAssignmentOperator, "testAssignmentOperator"},
        {testDisplay, "testDisplay"},
        {testMergeStacks, "testMergeStacks"}
    };

    for (const UnitTestPair& testPair : tests) {
        runTest(testPair.first, testPair.second);
    }

    return 0;
}

