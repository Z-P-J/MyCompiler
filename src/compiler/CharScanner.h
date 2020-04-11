#ifndef CHAR_SCANNER_H
#define CHAR_SCANNER_H

#include "fstream"

class CharScanner
{
private:
    /* data */
    int currentLine_;
    std::fstream file_;
    char currentChar_;
    bool isEnd_;
    bool rollback_ = false;
public:
    CharScanner(char* path);
    ~CharScanner();
    char getNextChar();
    bool isEnd();
    void rollback();
    int getCurrentLine();
};

#endif
