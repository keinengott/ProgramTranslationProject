#include "tokenstack.h"

TokenStack::TokenStack()
{
 //nothing to initialize yet
}

void TokenStack::push(std::string instance)
{
    mStack.push(instance);
}

void TokenStack::pop()
{
    mStack.pop();
}

int TokenStack::find(std::string instance)
{
    return find(mStack, instance);
}

int TokenStack::find(std::stack<std::string> s, std::string instance)
{
    int where = 0;
    while (!s.empty())
    {
        if (s.top() == instance) return where;
        s.pop();
        where++;
    }
    return -1; // not found
}
