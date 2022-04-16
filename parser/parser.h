#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include <string>
#include <vector>
#include <algorithm>

Node* parser();
void printErrorAndExit(std::string expected, std::string received, int lineNum);
void preorderTraversal(Node* aNode, int aLevel = 0);
std::string join(const std::vector<std::string>& v, std::string c);

#endif // PARSER_H
