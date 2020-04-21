#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "CharScanner.h"

class LexicalAnalyzer
{

public:
    enum State //枚举类型，保存状态
    {
        START,
        INANATATION,
        INANATATION1,
        INANATATION2,
        INNUM,
        INNUM1,
        INNUM2,
        INID,
        IN_ADD,
        IN_SUB,
        // IN_MUL, // 
        // IN_DIV, // 除法
        // IN_GREATER, // 大于
        // IN_LESS, // 小于
        // IN_NOT, // !
        // IN_EQ, // 等号 
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