#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "CharScanner.h"
#include "vector"
#include "Token.h"

class LexicalAnalyzer
{

public:
    enum State //枚举类型，保存状态
    {
        START,
        IN_ANATATION,
        IN_ANATATION1,
        IN_ANATATION2,
        IN_NUM,
        IN_NUM1,
        IN_NUM2,
        IN_ID,
        // IN_ADD,
        // IN_SUB,
        IN_COMPARE,
        // IN_OPERATE,
        // INSTRING,
        // INCHAR,
        DONE
    };

    // LexicalAnalyzer(char *filePath);
    LexicalAnalyzer(string filePath);
    ~LexicalAnalyzer();
    vector<Token *> analyze();
    // void analyze2();

private:
    char getNextChar();
    // const char* filePath_;
    CharScanner *scanner_;
    State currentState_ = START;
};

#endif