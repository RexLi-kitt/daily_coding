#include <iostream>
#include "DbLkList.h"
bool Print(const int& value)
{
    std::cout << value << " ";
    return true;
}
int main()
{
    DbLkList<int> list;
    for (int i = 1; i <= 10; ++i)list.AddTail(i);
    list.Reverse();
    list.Traverse(Print);
    std::cout << std::endl;
    return 0;
}
