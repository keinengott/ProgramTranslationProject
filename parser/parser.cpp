#include <iostream>
#include "parser.h"
#include "../scanner/scanner.h"
#include "../scanner/token.h"
#include "productions.h"
static void printNode(Node* aNode, int aLevel);
static void printTokens(std::vector<Token> aTokens);
static void printIndentation(int aLevel);

Node* parser()
{
    Token token = nextToken();
    Node* rootNode = program(token);

    if (token.type != EOF_TK)
    {
        printErrorAndExit(getTokenString(EOF_TK, ""), getTokenString(token), token.lineNum);
    }
    return rootNode;
}

void preorderTraversal(Node* aNode, int aLevel)
{
    if (aNode == NULL)
        return;

    printNode(aNode, aLevel);

    if (aNode->children.size() == 0)
        return;

    for (auto child: aNode->children)
        preorderTraversal(child, aLevel + 1);
}

static std::string getReason(std::string expected, std::string received, int lineNum)
{
    return "parser error: line " + std::to_string(lineNum) +
        ": expecting " + expected +
        ", but received " + received;
}

void printErrorAndExit(std::string expected, std::string received, int lineNum)
{
    std::string reason = getReason(expected, received, lineNum);
    std::cout<<reason<<std::endl;
}

// indent based on level
static void printIndentation(int aLevel)
{
    for (int i = 0; i < aLevel; i++)
        std::cout << "  ";
}

static void printTokens(std::vector<Token> aTokens)
{
    if (aTokens.size() == 0)
        return;

    std::cout << ": ";

    std::vector<std::string> tokenVec;

    for (auto token: aTokens)
        tokenVec.push_back(treeToken(token));

    std::cout << "{" + join(tokenVec, ", ") + "}";
}

static void printNode(Node* aNode, int aLevel)
{
    printIndentation(aLevel);

    std::cout << " " << aNode->label;

    printTokens(aNode->tokens);

    std::cout << std::endl;
}

std::string join(const std::vector<std::string>& v, std::string c)
{
    std::string out;

    for (std::vector<std::string>::const_iterator p = v.begin(); p != v.end(); ++p)
    {
        out += *p;

        if (p != v.end() - 1)
            out += c;
    }

    return out;
}
