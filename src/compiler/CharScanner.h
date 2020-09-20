#ifndef CHAR_SCANNER_H
#define CHAR_SCANNER_H

#include "fstream"
#include "vector"
#include "string"

using namespace std;

class CharScanner
{
private:
    /* data */
    int currentLineNumber_;
    string currentLine_;
    std::fstream file_;
    char currentChar_;
    bool isEnd_;
    bool rollback_ = false;
    vector<string> lines_;
    // int pointer = 0;
    
public:
    // CharScanner(char* path);
    CharScanner(string path);
    ~CharScanner();
    char getNextChar();
    // char getNextChar2();
    bool isEnd();
    void rollback();
    int getCurrentLine();
    vector<string> getLines();
};

#endif
