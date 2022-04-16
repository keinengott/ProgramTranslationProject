#ifndef TREE_H
#define TREE_H
#include "node.h"
#include <vector>

class Tree {

public:
    Tree();
    void buildTree(std::vector<std::string> tokens);
    void printInOrder(std::string outFile);
    void printPreOrder(std::string outFile);
    void printPostOrder(std::string outFile);
private:
    Node* root = NULL;
    void printInOrder(Node* node, std::ofstream& file, int level = 0);
    void printPreOrder(Node* node, std::ofstream& file, int level = 0);
    void printPostOrder(Node* node, std::ofstream& file, int level = 0);
    void insertNode(std::string str);
    Node* insertNode(std::string str, Node* node);
    void printNodeInfo(std::ofstream& file, Node* node, int level);
};

#endif // TREE_H
