#ifndef TOKENSTACK_H
#define TOKENSTACK_H

#include <stack>
#include "../scanner/token.h"

class TokenStack
{
public:
    TokenStack();
    void push(std::string instance);
    void pop();
    int find(std::string instance);
private:
    std::stack<std::string> mStack;
    int find(std::stack <std::string> s, std::string instance);
};

#endif // TOKENSTACK_H
