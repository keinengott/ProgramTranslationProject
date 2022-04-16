#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H

#include "node.h"
#include "../scanner/token.h"

Node* program(Token& token);
Node* block(Token& token);
Node* vars(Token& token);
Node* expr(Token& token);
Node* N(Token& token);
Node* A(Token& token);
Node* M(Token& token);
Node* R(Token& token);
Node* stats(Token& token);
Node* mStat(Token& token);
Node* stat(Token& token);
Node* in(Token& token);
Node* out(Token& token);
Node* _if(Token& token);
Node* loop(Token& token);
Node* assign(Token& token);
Node* RO(Token& token);
Node* conditional(Token& token);
Node* _goto(Token& token);
Node* label(Token& token);

#endif // PRODUCTIONS_H
