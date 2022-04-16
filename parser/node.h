#ifndef NODE_H
#define NODE_H


#include <string>
#include <vector>

#include "../scanner/token.h"

struct Node
{
    std::string label;
    std::vector<Node*> children;
    std::vector<Token> tokens;
    Node(std::string aLabel) : label(aLabel){}
};
typedef struct Node Node;

Node* getNode(std::string aLabel);

#endif // NODE_H
