#include <iostream>
#include "token.h"
#include <set>
using namespace std;

void printToken(Token aToken)
{
    static int count = 1;
    string tokenNames[] = {"Operator/Delimiter","Identifier","Number","EOF","Keyword"};
    cout<<"Token "<<count++<<endl;
    cout<<"\tToken Type: "<<tokenNames[aToken.type-100]<<"\n\tToken Instance: "<<aToken.tokenInstance<<"\n\tLine Number: "<<aToken.lineNum<<endl;
}

int getTokenType(int aState, string aInstance)
{
    set<string> keywords = {"begin", "end", "loop", "whole", "void", "exit", "getter", "outter", "main", "if", "then", "assign", "data", "proc"};
    if (keywords.count(aInstance) > 0) return KEYWORD_TK;
    return aState;
}

string treeToken(Token aToken)
{
    switch (aToken.type)
    {
        case OP_DELIM_TK:
            return "operDelTk: " + aToken.tokenInstance;
        case IDENT_TK:
            return "idTk: " + aToken.tokenInstance;
        case KEYWORD_TK:
            return "kwTk: " + aToken.tokenInstance;
        case NUM_TK:
            return "#Tk: " + aToken.tokenInstance;
        case EOF_TK:
            return "EOFTk";
        default:
            return "token type undefined";
    }
}

string getTokenString(Token aToken)
{
    return getTokenString(aToken.type, aToken.tokenInstance);
}

string getTokenString(int aTokenType, string aTokenInstance)
{
    switch (aTokenType)
    {
    case OP_DELIM_TK:
        return aTokenInstance + " token";
    case IDENT_TK:
        return "identifier token";
    case NUM_TK:
        return "number token";
    case KEYWORD_TK:
        return aTokenInstance + " keyword";
    case EOF_TK:
        return "EOF token";
    default:
        return "token type undefined";
    }
}
