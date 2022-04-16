#include <iostream>
#include <fstream>
#include "tree.h"

enum DIRECTION {
    LEFT  = 0,
    RIGHT = 1,
    ROOT  = 2,
    ERROR = 3
};

Tree::Tree(){}

void Tree::buildTree(std::vector<std::string> tokens)
{
    for (std::string token : tokens)
    {
        insertNode(token);
    }
}

void Tree::insertNode(std::string str)
{
    if (root == NULL) {
        root = new Node(str);
        return;
    }
    insertNode(str, root);
    return;
}

Node* Tree::insertNode(std::string str, Node* node)
{
    if (node == NULL) return new Node(str);
    char firstChar = str.front();
    char secondChar;
    if (str.length() >= 2) secondChar = str[1];

    if (firstChar < node->data.front().front()) node->left = insertNode(str, node->left);
    else if (firstChar > node->data.front().front()) node->right = insertNode(str, node->right);
    else { // First char is equal in this case
        unsigned where = ERROR;
        //this loop determines where to place the data based on the second char and whether the data already exists
        for (std::string s : node->data){
            if (s.length() > 1 && secondChar < s[1]) where = LEFT;
            else if (s.length() > 1 && secondChar > s[1]) where = RIGHT;
            else {
                if (s.compare(str) == 0){
                    where = ERROR;
                    break;
                }
                else where = ROOT;
            }
        }
        switch (where){
        case LEFT:
            node->left = insertNode(str, node->left);
            break;
        case RIGHT:
            node->right = insertNode(str, node->right);
            break;
        case ROOT:
            node->data.push_back(str);
            break;
        case ERROR:
            std::cerr<<str<<" already in node. Throwing out duplicate"<<std::endl;
            return node;
            break;
        default:
            std::cerr<<"Something went wrong. Error "<<where<<" "<<__FILE__<<":"<<__LINE__<<std::endl;
        }
    }
    return node; //silencing compiler warning
}

void Tree::printInOrder(std::string outFile)
{
    std::ofstream file(outFile + ".inorder");
    if (root != NULL) printInOrder(root, file);
}

void Tree::printPreOrder(std::string outFile)
{
    std::ofstream file(outFile + ".preorder");
    if (root != NULL) printPreOrder(root, file);
}

void Tree::printPostOrder(std::string outFile)
{
    std::ofstream file(outFile + ".postorder");
    if (root != NULL) printPostOrder(root, file);
}

void Tree::printInOrder(Node* node, std::ofstream& file, int level)
{
    if (node == NULL) return;
    printInOrder(node->left, file, level+1);
    printNodeInfo(file, node, level);
    printInOrder(node->right, file, level+1);
}

void Tree::printPreOrder(Node* node, std::ofstream& file, int level)
{
    if (node == NULL) return;
    printNodeInfo(file, node, level);
    printPreOrder(node->left, file, level+1);
    printPreOrder(node->right, file, level+1);
}

void Tree::printPostOrder(Node* node, std::ofstream& file, int level)
{
    if (node == NULL) return;
    printPostOrder(node->left, file, level+1);
    printPostOrder(node->right, file, level+1);
    printNodeInfo(file, node, level);
}

void Tree::printNodeInfo(std::ofstream& file, Node* node, int level)
{
    std::string indentation(level*2, ' ');
    file << indentation << node->data.front().substr(0,2) << " ";
    for (std::string it : node->data)
    {
        file << it << " ";
    }
    file << std::endl;
}
