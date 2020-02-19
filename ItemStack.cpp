#include <iomanip>

#include "ItemStack.h"

//------------------------------------------------------------------------------
ItemStack::ItemStack()
    :item(0, "Air"),
    quantity(0)
{
}

//------------------------------------------------------------------------------
ItemStack::ItemStack(const Item& item, int s)
    :item(item),
    quantity(s)
{
}

//------------------------------------------------------------------------------
void ItemStack::addItemsFrom(const ItemStack& other)
{
    this->quantity += other.quantity;
}

//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream &outs, const ItemStack& prt)
{
    outs << std::right << "(" << std::setw(2) << prt.size() << ") "
         << (prt.getItem()).getName();

    return outs;
}
