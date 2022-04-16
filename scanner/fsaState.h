#ifndef FSASTATE_H
#define FSASTATE_H

#include <vector>
#include "fsaTable.h"

class FSAState
{
public:
    FSAState();
    FSAState(int aState);
    int state();
    bool final();
    bool error();
    bool active();
    static FSAState next(int current, char newChar);

private:
    FSATable mTable;
    int mState;
    enum State {
        POST_WHITESPACE = 0, //after seeing whitespace or upon beginning
        POST_DOLLAR = 4, // inside of the dollar signs
        PRE_DOLLAR = 5,
        COMMENT_END = 6
    };
};

#endif // FSASTATE_H
