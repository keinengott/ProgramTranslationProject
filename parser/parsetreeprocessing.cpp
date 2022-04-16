#include <iostream>
#include "parsetreeprocessing.h"
#include "parser.h"

static void semanticError(int aLineNum, std::string aReason);
void printErrorAndExit(std::string aMsg);
std::string getAssemblyCmd(std::string aTokenInstance);

ParseTreeProcessing::ParseTreeProcessing() :
    mVariableCount(0),
    mLabelCount(0)
{
    mVarCount.push(0);
}

void ParseTreeProcessing::processParseTree(Node* root)
{
    traverseTree(root);
    processTarget();
}

// preorder traversal
void ParseTreeProcessing::traverseTree(Node* aNode)
{
    if (aNode == NULL) return;

    processNode(aNode);
}

void ParseTreeProcessing::processTarget()
{
    for (int i = 0; i < mVariableCount; i++)
    {
        mTarget+="V" + std::to_string(i)+" 0\n";
    }
    mTarget += "STOP\n"; // program end
}

std::string ParseTreeProcessing::getTarget()
{
    return mTarget;
}

void ParseTreeProcessing::processNode(Node* aNode)
{
    processNodeLabel(aNode);

    if (aNode->tokens.size() != 0) processIdentifierAtNode(aNode);
}

void ParseTreeProcessing::processNodeLabel(Node* aNode)
{
    if (aNode->label == "vars")
    {
        Token idToken = aNode->tokens.front();
        Token idTokenVal = aNode->tokens.at(1);
        verifyIdTokenDefined(idToken.tokenInstance, idToken.lineNum);
        pushToStack(idToken.tokenInstance, idTokenVal.tokenInstance);
    }

    if (aNode->label == "block")
    {
        // starting new count for block
        mVarCount.push(0);

        for (auto child: aNode->children) traverseTree(child);

        // calling pop "varCount" times
        for (int i = 0; i < mVarCount.top(); i++)
        {
            mTokenStack.pop();
            mTarget += "POP\n";
        }

        mVarCount.pop();

        return;
    }

    if (aNode->label  == "if")
    {
        processConditional(aNode);
        return;
    }
    if (aNode->label == "loop")
    {
        std::string label = "L" + std::to_string(mLabelCount++);
        mTarget += label + ": NOOP\n";
        //passing string to flag that this is in loop
        processConditional(aNode, label);
        return;
    }
    if (aNode->label == "in") //input
    {
        std::string temp = "V" + std::to_string(mVariableCount++);
        mTarget += "READ " + temp + "\n";
        mTarget += "LOAD " + temp + "\n";
        std::string tk = aNode->tokens.front().tokenInstance;
        mTarget += "STACKW " + std::to_string(mTokenStack.find(tk)) +"\n";

        return;
    }

    if (aNode->label == "out") //output
    {
        traverseTree(aNode->children.front());

        std::string temp = "V" + std::to_string(mVariableCount++);
        mTarget += "STORE " + temp + "\n";
        mTarget += "WRITE " + temp + "\n";

        return;
    }
    // need to check if assign is called on undefined vars
    if (aNode->label == "assign")
    {
        traverseTree(aNode->children.front());
        Token tk = aNode->tokens.front();
        mTarget+="STACKW "+std::to_string(mTokenStack.find(tk.tokenInstance));
        mTarget+="\n";
        return;
    }
    if (aNode->label  == "expr")
    {
        if (aNode->tokens.size() == 1) // N - expr
        {
            Node* left = aNode->children.at(0);  // N
            Node* right = aNode->children.at(1); // expr
            Token tk = aNode->tokens.front();

            rightLeftEval(right, left, tk.tokenInstance);
        }
        else // evaluate N
            traverseTree(aNode->children.front());

        return;
    }
    if (aNode->label == "N")
    {
        if (aNode->tokens.size() == 1)
        {
            Node* left = aNode->children.at(0);  // N
            Node* right = aNode->children.at(1); // A
            Token tk = aNode->tokens.front();

            rightLeftEval(right, left, tk.tokenInstance);
        }
        else // evaluate A
        {
            traverseTree(aNode->children.front());
        }
        return;
    }

    if (aNode->label == "A")
    {
        if (aNode->tokens.size() == 1)
        {
            Node* left = aNode->children.at(0);  // M
            Node* right = aNode->children.at(1); // A
            Token tk = aNode->tokens.front();

            rightLeftEval(right, left, tk.tokenInstance);
        }
        else // evaluate M
        {
            traverseTree(aNode->children.front());
        }

        return;
    }

    if (aNode->label == "M")
    {
        Node* child = aNode->children.front();

        if (child->label == "R") traverseTree(child);

        else
        {
            traverseTree(child);
            mTarget += "MULT -1\n";
        }

        return;
    }

    if (aNode->label == "R")
    {
        if (aNode->tokens.size() == 1)
        {
            Token tk = aNode->tokens.front();

            if (tk.type == IDENT_TK) mTarget += "STACKR " + std::to_string(mTokenStack.find(tk.tokenInstance)) + "\n";
            else mTarget += "LOAD " + tk.tokenInstance + "\n"; // load number into acc
        }
        else traverseTree(aNode->children.front());

        return;
    }

    // check children of vars
    if (aNode->children.size() > 0)
    {
        for (auto child: aNode->children) traverseTree(child);
    }
}

void ParseTreeProcessing::processConditional(Node* aNode, std::string aLabel)
{
    Node* RO_node = aNode->children.at(1);

    std::string opToken = RO_node->tokens.at(0).tokenInstance;

    if (RO_node->tokens.size() > 1) opToken += RO_node->tokens.at(1).tokenInstance;

    rightLeftEval(aNode->children.at(2), aNode->children.at(0), "-");

    std::string label = "L" + std::to_string(mLabelCount++);;

    if (opToken == "=")
    {
        mTarget += getAssemblyCmd("=<") + " " + label + "\n";
        mTarget += getAssemblyCmd("=>") + " " + label + "\n";
    }
    else
    {
        mTarget += getAssemblyCmd(opToken) + " " + label + "\n";
    }
    Node* statNode = aNode->children.at(3);
    traverseTree(statNode);

    // if in loop, jump back to beginning label
    if (aLabel != "") mTarget += "BR " + aLabel + "\n";

    mTarget += label + ": NOOP\n";
}

void ParseTreeProcessing::rightLeftEval(Node* aRight, Node* aLeft, std::string aOpTokenInstance)
{
    traverseTree(aRight);

    std::string temp_var = "V" + std::to_string(mVariableCount++);
    mTarget += "STORE " + temp_var + "\n";

    traverseTree(aLeft);

    std::string cmd = getAssemblyCmd(aOpTokenInstance);

    mTarget += cmd + " " + temp_var + "\n";
}

void ParseTreeProcessing::processIdentifierAtNode(Node* aNode)
{
    for (auto tk: aNode->tokens)
    {
        if (tk.type != IDENT_TK) continue;
        if (aNode->label != "vars") checkIfDefined(tk);
    }
}


// make sure this var hasbeen defined
void ParseTreeProcessing::verifyIdTokenDefined(std::string aInstance, int aLineNum)
{
    int tk_idx = mTokenStack.find(aInstance);

    if (mVarCount.top() > 0 && tk_idx >= 0 && tk_idx < mVarCount.top()) semanticError(aLineNum, "'" + aInstance + "' is already defined in this block");
}

// pushed token to stack and adds to the var count
void ParseTreeProcessing::pushToStack(std::string aInstance, std::string aVal)
{
    mTokenStack.push(aInstance);
    mVarCount.top()++;
    mTarget += "LOAD " + aVal + "\n";
    mTarget += "PUSH\n";
    mTarget += "STACKW 0\n";
}


// Make sure the var is defined before we use it
void ParseTreeProcessing::checkIfDefined(Token aToken)
{
    if (mTokenStack.find(aToken.tokenInstance) < 0) semanticError(aToken.lineNum, "'" + aToken.tokenInstance + "' has not been defined");
}

static void semanticError(int aLineNum, std::string aReason)
{
    printErrorAndExit("semantics error: line " + std::to_string(aLineNum) + ": " + aReason);
}

void printErrorAndExit(std::string aMsg)
{
    std::cout << aMsg << std::endl;
    std::exit(EXIT_FAILURE);
}

std::string getAssemblyCmd(std::string aTokenInstance)
{
    if (aTokenInstance == "+") return "ADD";
    else if (aTokenInstance == "-") return "SUB";
    else if (aTokenInstance == "*") return "MULT";
    else if (aTokenInstance == "/") return "DIV";
    else if (aTokenInstance == ">") return "BRZNEG";
    else if (aTokenInstance == "<") return "BRZPOS";
    else if (aTokenInstance == "=>") return "BRNEG";
    else if (aTokenInstance == "=<") return "BRPOS";
    else if (aTokenInstance == "==") return "BRZERO";

    printErrorAndExit("getAssemblyCmd: no assembly command for '" + aTokenInstance + "' operator");

    return "";
}
