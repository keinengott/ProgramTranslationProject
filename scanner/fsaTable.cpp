#include "fsaTable.h"

FSATable::FSATable()
{
    fsaTable.push_back({-1,-1,8,9,1,2,-1,3,103,0,7,5,-1});
    fsaTable.push_back({100,100,100,100,100,100,100,100,100,100,100,100,100});
    fsaTable.push_back({101,101,101,101,101,2,2,2,101,101,101,2,101});
    fsaTable.push_back({-1,-1,102,102,102,102,102,3,102,102,102,102,102});
    fsaTable.push_back({4,4,4,4,4,4,4,4,-1,4,4,6,4});
    fsaTable.push_back({-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1});
    fsaTable.push_back({-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1000,-1});
    fsaTable.push_back({-1,-1,-1,-1,-1,2,2,2,-1,-1,-1,-1,-1});                 // leading underscore only valid with numbers and letters
    fsaTable.push_back({100,100,100,1,100,2,100,100,100,100,100,100,100});     // preceding colon 8
    fsaTable.push_back({1,1,100,1,100,100,100,100,100,100,100,100,100});       // preceding equals 9
}

int FSATable::next(int aState, char aChar)
{
    return fsaTable[aState][getColumnIdx(aChar)];
}

int FSATable::getColumnIdx(char c)
{
    if (operatorDelimGroup.find(c) != std::string::npos) return Columns::OP_DELIM;

    switch(c)
    {
    case ':':
        return Columns::COLON;
    case '=':
        return Columns::EQUALS;
    case '>':
        return Columns::GREATER_THAN;
    case '<':
        return Columns::LESS_THAN;
    case '_':
        return Columns::UNDERSCORE;
    case ' ':
        return Columns::WS;
    case '\n':
        return Columns::WS;
    case '$':
        return Columns::DOLLAR_SYMBOL;
    default:
        if (isdigit(c)) return Columns::DIGIT;
        else if (isalpha(c))
        {
            if (islower(c)) return Columns::ALPHA_LOW;
            else return Columns::ALPHA_UP;
        }
        else if (c == EOF) return Columns::EOF_C;
        else return Columns::INVALID;
    }
}
