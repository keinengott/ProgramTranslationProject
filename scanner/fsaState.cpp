#include "fsaState.h"
#define FINAL 100
#define ERROR -1
#define COMMENT_DONE 1000

FSAState::FSAState() :
    mTable(FSATable()),
    mState(0)
{
}

FSAState::FSAState(int aState) :
    mTable(FSATable()),
    mState(aState)
{
}

int FSAState::state()
{
    return mState;
}

bool FSAState::final()
{
    return  mState >= FINAL;
}

bool FSAState::error()
{
    return mState == ERROR;
}

bool FSAState::active()
{
    return mState != POST_DOLLAR && mState != PRE_DOLLAR && mState != COMMENT_END && mState != POST_WHITESPACE;
}

FSAState FSAState::next(int current, char newChar)
{
    FSATable table = FSATable();
    int nextState = table.next(current, newChar);
    return FSAState(nextState);
}
