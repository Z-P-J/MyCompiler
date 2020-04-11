#include "CharScanner.h"

#include "iostream"
#include "fstream"


using namespace std;

CharScanner::CharScanner(char* path) : currentLine_(1), isEnd_(false)
{
    file_.open(path, ios::in);
}

CharScanner::~CharScanner()
{
    
}

char CharScanner::getNextChar() {
    if (rollback_) {
        rollback_ = false;
        return currentChar_;
    }
    if (isEnd_) {
        return EOF;
    }

    if (!(file_ >> noskipws >> currentChar_)) {
        isEnd_ = true;
        currentChar_ = EOF;
        file_.close();
    }

    if (currentChar_ == '\n') {
        currentLine_++;
    }

    // if (!file_.eof()) {
    //     file_ >> noskipws >> currentChar_;
    // } else {
    //     isEnd_ = true;
    //     currentChar_ = EOF;
    //     file_.close();
    // }
    return currentChar_;
}

bool CharScanner::isEnd() {
    return isEnd_;
}

void CharScanner::rollback() {
    if (!isEnd_) {
        rollback_ = true;
        // file_.seekg(-2L, ios::cur); //  * (off_t)sizeof(char)
    }
}

int CharScanner::getCurrentLine() {
    return currentLine_;
}