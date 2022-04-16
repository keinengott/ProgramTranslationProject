#include "testscanner.h"


void testScanner()
{
    Token tk = nextToken();
    while (tk.type != EOF_TK)
    {
        printToken(tk);
        tk = nextToken();
    }
    printToken(tk); // capture EOF token
}
