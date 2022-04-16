#include <iostream>
#include <set>
#include "productions.h"
#include "parser.h"
#include "../scanner/scanner.h"
static bool mHasMained = false;

Node* program(Token& token)
{
    Node* node = getNode("program");
    if (token.tokenInstance == "main")
    {
        token = nextToken();
        node->children.push_back(block(token));
    }
    else if(token.tokenInstance == "data")
    {
        node->children.push_back(vars(token));
        node->children.push_back(block(token));

        return node;
    } else printErrorAndExit(getTokenString(KEYWORD_TK, "main"), getTokenString(token), token.lineNum);
    return NULL;
}

Node* vars(Token& token)
{
    if (!mHasMained && token.tokenInstance == "main")
    {
        mHasMained = true;
        token = nextToken();
    }
    if (token.tokenInstance == "data")
    {
        Node* node = getNode("vars");

        token = nextToken();

        if (token.type == IDENT_TK)
        {
            node->tokens.push_back(token);

            token = nextToken();

            if (token.tokenInstance == ":=")
            {
                token = nextToken();

                if (token.type == NUM_TK)
                {
                    node->tokens.push_back(token);

                    token = nextToken();
                    if (token.tokenInstance == ";")
                    {
                        token = nextToken();
                        node->children.push_back(vars(token));
                        return node;
                    }
                    else printErrorAndExit(getTokenString(OP_DELIM_TK, ";"), getTokenString(token), token.lineNum);
                }
                else printErrorAndExit(getTokenString(NUM_TK, ""), getTokenString(token), token.lineNum);
            }
            else printErrorAndExit(getTokenString(OP_DELIM_TK, ":="), getTokenString(token), token.lineNum);
        }
        else printErrorAndExit(getTokenString(IDENT_TK, ""), getTokenString(token), token.lineNum);
    }

    return NULL;
}

Node* block(Token& token)
{
    if (token.tokenInstance == "begin")
    {
        Node* node = getNode("block");

        token = nextToken();

        node->children.push_back(vars(token));
        node->children.push_back(stats(token));

        if (token.tokenInstance == "end")
        {
            token = nextToken();
            if (token.tokenInstance == ";") token = nextToken();
            return node;
        }
        else printErrorAndExit(getTokenString(KEYWORD_TK, "end"), getTokenString(token), token.lineNum);
    }
    else printErrorAndExit(getTokenString(KEYWORD_TK, "begin"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* stats(Token& token)
{
    Node* node = getNode("stats");

    node->children.push_back(stat(token));
    node->children.push_back(mStat(token));

    return node;
}

Node* mStat(Token& token)
{
    std::set<std::string> stat_productions =
    {"getter", "outter", "begin", "if", "loop", "assign", "proc"};

    if (stat_productions.count(token.tokenInstance) > 0)
    {
        Node* node = getNode("mStat");

        node->children.push_back(stat(token));
        node->children.push_back(mStat(token));

        return node;
    }

    return NULL;
}

Node* stat(Token& token)
{
    Node* node = getNode("stat");

    if (token.tokenInstance == "getter")
    {
        node->children.push_back(in(token));
        return node;
    }

    if (token.tokenInstance == "outter")
    {
        node->children.push_back(out(token));
        return node;
    }

    if (token.tokenInstance == "begin")
    {
        node->children.push_back(block(token));
        return node;
    }

    if (token.tokenInstance == "if")
    {
        node->children.push_back(_if(token));
        return node;
    }

    if (token.tokenInstance == "loop")
    {
        node->children.push_back(loop(token));
        return node;
    }

    if (token.tokenInstance == "assign")
    {
        node->children.push_back(assign(token));
        return node;
    }
    if (token.tokenInstance == "proc")
    {
        node->children.push_back(_goto(token));
        return node;
    }
    if (token.tokenInstance == "void")
    {
        node->children.push_back(label(token));
        return node;
    }

    printErrorAndExit("getter, outter, begin, if, loop, proc, or " +
                      getTokenString(KEYWORD_TK, "assign"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* in(Token& token)
{
    if (token.tokenInstance == "getter")
    {
        Node* node = getNode("in");

        token = nextToken();

        if (token.type == IDENT_TK)
        {
            node->tokens.push_back(token);

            token = nextToken();

            if (token.tokenInstance == ";")
            {
                token = nextToken();
                return node;
            }
            else printErrorAndExit(getTokenString(OP_DELIM_TK, ";"), getTokenString(token), token.lineNum);
        }
        else printErrorAndExit(getTokenString(IDENT_TK, ""), getTokenString(token), token.lineNum);
    }
    else printErrorAndExit(getTokenString(KEYWORD_TK, "getter"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* out(Token& token)
{
    if (token.tokenInstance == "outter")
    {
        Node* node = getNode("out");

        token = nextToken();
        node->children.push_back(expr(token));

        if (token.tokenInstance == ";")
        {
            token = nextToken();
            return node;
        } else printErrorAndExit(getTokenString(OP_DELIM_TK, ";"), getTokenString(token), token.lineNum);
    } else printErrorAndExit(getTokenString(KEYWORD_TK, "outter"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* expr(Token& token)
{
    Node* node = getNode("expr");

    node->children.push_back(N(token));

    if (token.tokenInstance == "-")
    {
        node->tokens.push_back(token);

        token = nextToken();

        node->children.push_back(expr(token));

        return node;
    }

    return node;
}

Node* N(Token& token)
{
    Node* node = getNode("N");

    node->children.push_back(A(token));

    if (token.tokenInstance == "/" || token.tokenInstance == "*")
    {
        node->tokens.push_back(token);

        token = nextToken();

        node->children.push_back(N(token));

        return node;
    }

    return node;
}

Node* A(Token& token)
{
    Node* node = getNode("A");

    node->children.push_back(M(token));

    if (token.tokenInstance == "+")
    {
        node->tokens.push_back(token);

        token = nextToken();

        node->children.push_back(A(token));

        return node;
    }

    return node;
}

Node* M(Token& token)
{
    Node* node = getNode("M");

    if (token.tokenInstance == "*")
    {
        token = nextToken();

        node->children.push_back(M(token));

        return node;
    }

    node->children.push_back(R(token));

    return node;
}

Node* R(Token& token)
{
    Node* node = getNode("R");

    if (token.tokenInstance == "(")
    {
        token = nextToken();
        node->children.push_back(expr(token));

        if (token.tokenInstance == ")")
        {
            token = nextToken();
            return node;
        }
        else printErrorAndExit(getTokenString(OP_DELIM_TK, ")"), getTokenString(token), token.lineNum);
    }

    if (token.type == IDENT_TK || token.type == NUM_TK)
    {
        node->tokens.push_back(token);
        token = nextToken();
        return node;
    }

    printErrorAndExit(getTokenString(IDENT_TK, "") + ", " + getTokenString(NUM_TK, "") + ", or "
                      + getTokenString(OP_DELIM_TK, "("), getTokenString(token), token.lineNum);

    return NULL;
}

Node* _if(Token& token)
{
    if (token.tokenInstance == "if")
    {
        Node* node = getNode("if");
        token = nextToken();

        if (token.tokenInstance == "[")
        {
            token = nextToken();

            node->children.push_back(expr(token));
            node->children.push_back(RO(token));
            node->children.push_back(expr(token));

            if (token.tokenInstance == "]")
            {
                token = nextToken();
                node->children.push_back(stat(token));
                return node;
            }
            else printErrorAndExit(getTokenString(OP_DELIM_TK, "]"), getTokenString(token), token.lineNum);
        }
        else printErrorAndExit(getTokenString(OP_DELIM_TK, "["), getTokenString(token), token.lineNum);
    }
    else printErrorAndExit(getTokenString(KEYWORD_TK, "if"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* loop(Token& token)
{
    if (token.tokenInstance == "loop")
    {
        Node* node = getNode("loop");
        token = nextToken();

        if (token.tokenInstance == "[")
        {
            token = nextToken();

            node->children.push_back(expr(token));
            node->children.push_back(RO(token));
            node->children.push_back(expr(token));

            if (token.tokenInstance == "]")
            {
                token = nextToken();
                node->children.push_back(stat(token));
                return node;
            }
            else printErrorAndExit(getTokenString(OP_DELIM_TK, "]"), getTokenString(token), token.lineNum);
        }
        else printErrorAndExit(getTokenString(OP_DELIM_TK, "["), getTokenString(token), token.lineNum);
    }
    else printErrorAndExit(getTokenString(KEYWORD_TK, "loop"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* RO(Token& token)
{
    if (token.tokenInstance == "=<" || token.tokenInstance == "=>" || token.tokenInstance == "==")
    {
        Node* node = getNode("RO");
        node->tokens.push_back(token);
        token = nextToken();

        if (token.tokenInstance == "=")
        {
            node->tokens.push_back(token);
            token = nextToken();
            return node;
        }

        return node;
    }

    printErrorAndExit("=<, =>, or " +
                      getTokenString(OP_DELIM_TK, "=="), getTokenString(token), token.lineNum);

    return NULL;
}

Node* assign(Token& token)
{
    if (token.tokenInstance == "assign")
    {
        Node* node = getNode("assign");
        token = nextToken();

        if (token.type == IDENT_TK)
        {
            node->tokens.push_back(token);
            token = nextToken();

            if (token.tokenInstance == ":=")
            {
                token = nextToken();
                node->children.push_back(expr(token));

                if (token.tokenInstance == ";")
                {
                    while (token.tokenInstance == ";") token = nextToken();
                    return node;
                }
                else printErrorAndExit(getTokenString(OP_DELIM_TK, ";"), getTokenString(token), token.lineNum);
            }
            else printErrorAndExit(getTokenString(OP_DELIM_TK, ":="), getTokenString(token), token.lineNum);
        }
        else printErrorAndExit(getTokenString(IDENT_TK, ""), getTokenString(token), token.lineNum);
    }
    else printErrorAndExit(getTokenString(KEYWORD_TK, "assign"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* _goto(Token& token)
{
    if (token.tokenInstance == "proc")
    {
        Node* node = getNode("goto");

        token = nextToken();

        if (token.type == IDENT_TK)
        {
            node->tokens.push_back(token);

            token = nextToken();

            if (token.tokenInstance == ";")
            {
                token = nextToken();
                return node;
            }
            else printErrorAndExit(getTokenString(OP_DELIM_TK, ";"), getTokenString(token), token.lineNum);
        }
        else printErrorAndExit(getTokenString(IDENT_TK, ""), getTokenString(token), token.lineNum);
    }
    else printErrorAndExit(getTokenString(KEYWORD_TK, "proc"), getTokenString(token), token.lineNum);

    return NULL;
}

Node* label(Token& token)
{
    if (token.tokenInstance == "void")
    {
        Node* node = getNode("label");

        token = nextToken();

        if (token.type == IDENT_TK)
        {
            node->tokens.push_back(token);

            token = nextToken();

            if (token.tokenInstance == ";")
            {
                token = nextToken();
                return node;
            }
            else printErrorAndExit(getTokenString(OP_DELIM_TK, ";"), getTokenString(token), token.lineNum);
        }
        else printErrorAndExit(getTokenString(IDENT_TK, ""), getTokenString(token), token.lineNum);
    }
    else printErrorAndExit(getTokenString(KEYWORD_TK, "void"), getTokenString(token), token.lineNum);

    return NULL;
}
