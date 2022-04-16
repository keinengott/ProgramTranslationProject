#include <iostream>
#include "scanner.h"

Token nextToken()
{
    gotoIsCool:
    static int lineNumber = 1;

    std::string tokenInstance = "";
    FSAState currentState = FSAState(0);
    FSAState nextState;
    char currChar = nextChar();
    while (!currentState.final())
    {
        nextState = FSAState::next(currentState.state(), currChar);
        if (nextState.error())
        {
            std::cout<<"SCANNER ERROR: Invalid character "<<currChar<<" at line# "<<lineNumber<<std::endl;
            exit(EXIT_FAILURE);
        }
        if (nextState.final()) break; // return token
        else
        {
            // not final, so build up instance and continue
            currentState = nextState;
            if (nextState.active()) tokenInstance += currChar;
            if(currChar == '\n')
            {
                lineNumber++;
            }
            currChar = nextChar();
        }
    }
    if (nextState.state() == 1000) goto gotoIsCool; // dangerous games are being played. This ignores my artificial comment block token
    // unget last char to process it next iteration
    std::cin.unget();

    return Token{getTokenType(nextState.state(), tokenInstance), tokenInstance, lineNumber};
}

char nextChar()
{
    char c = EOF;
    std::cin.get(c);
    return c;
}
