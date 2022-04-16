#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>

struct Node
{
    std::vector<std::string> data;
    Node* left, *right;
    Node(std::string data)
    {
        this->data.push_back(data);
        left = right = NULL;
    }
};
typedef struct Node Node;

#endif // NODE_H
