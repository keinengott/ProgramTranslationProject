#include "codegenerator.h"
#include <iostream>
#include "../parser/parsetreeprocessing.h"

std::string generateTarget(Node* root)
{
    ParseTreeProcessing processor;

    processor.processParseTree(root);

    return processor.getTarget();
}
