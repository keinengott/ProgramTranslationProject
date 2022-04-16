#ifndef FSATABLE_H
#define FSATABLE_H

#include <vector>
#include <array>
#include <string>

#define FSA_TBL_COLUMNS 13


class FSATable{
public:
    FSATable();

    enum Columns {
        GREATER_THAN,
        LESS_THAN,
        COLON,
        EQUALS,
        OP_DELIM,
        ALPHA_LOW,
        ALPHA_UP,
        DIGIT,
        EOF_C,
        WS,
        UNDERSCORE,
        DOLLAR_SYMBOL,
        INVALID
    };

    int next(int aState, char aChar);
    std::vector<std::array<int, FSA_TBL_COLUMNS> > getFsaTable() {return fsaTable;}
private:
    int getColumnIdx(char c);
    std::string operatorDelimGroup = "+-*/%.(),{};[]";
    std::vector<std::array<int, FSA_TBL_COLUMNS> > fsaTable;
};

#endif // FSATABLE_H
