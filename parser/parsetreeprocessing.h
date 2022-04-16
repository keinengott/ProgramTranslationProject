#ifndef PARSETREEPROCESSING_H
#define PARSETREEPROCESSING_H

#include <stack>
#include "node.h"
#include "tokenstack.h"

class ParseTreeProcessing
{
public:
    ParseTreeProcessing();
    void processParseTree(Node* root);
    std::string getTarget();
private:
    enum TreeTraveral
    {
        PREORDER,
        POSTORDER,
        INORDER
    };

    // member variables
    TokenStack mTokenStack;    // Holds token definitions
    std::stack<int> mVarCount; // stack that represents the number of identifiers defined in a block.
    std::string mTarget;
    int mVariableCount; // for var names V1, V2, etc.
    int mLabelCount;

    //methods
    void traverseTree(Node* aNode);
    void processNode(Node* aNode);
    void processNodeLabel(Node* aNode);
    void verifyIdTokenDefined(std::string aInstance, int aLineNum);
    void pushToStack(std::string aInstance, std::string aVal);
    void processIdentifierAtNode(Node* aNode);
    void checkIfDefined(Token aToken);
    void processTarget();
    void processConditional(Node* aNode, std::string aLabel="");
    void rightLeftEval(Node* aRight, Node* aLeft, std::string aOpTokenInstance);

};

#endif // PARSETREEPROCESSING_H
