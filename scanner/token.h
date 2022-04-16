#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenID {
    OP_DELIM_TK = 100, //final
    IDENT_TK,
    NUM_TK,
    EOF_TK,
    KEYWORD_TK
};

struct Token {
    int type;
    std::string tokenInstance;
    int lineNum;
};
typedef struct Token Token;

void printToken(Token aToken);
int getTokenType(int aState, std::string aInstance);
std::string treeToken(Token aToken);
std::string getTokenString(Token aToken);
std::string getTokenString(int aTokenType, std::string aTokenInstance);

#endif // TOKEN_H
