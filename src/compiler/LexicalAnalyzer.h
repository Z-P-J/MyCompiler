#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "CharScanner.h"

class LexicalAnalyzer
{

public:
    enum State //枚举类型，保存状态
    {
        START,
        INNUM,
        INNUM1,
        INNUM2,
        INID,
        INCOMPARE,
        INOPERATE,
        INSTRING,
        INCHAR,
        DONE
    };

    LexicalAnalyzer(char *filePath);
    ~LexicalAnalyzer();
    void analyze();

private:
    /* data */
    // const char* filePath_;
    CharScanner *scanner_;
    State currentState_ = START;
};

#endif