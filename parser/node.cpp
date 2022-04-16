#include "node.h"

Node* getNode(std::string aLabel)
{
    return (new Node(aLabel));
}
