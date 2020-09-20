#include "CharScanner.h"

#include "iostream"
#include "fstream"

using namespace std;

// CharScanner::CharScanner(char *path)
//     : currentLineNumber_(1),
//       currentLine_(""),
//       isEnd_(false)
// {
//     file_.open(path, ios::in);
//     while (!file_)
//     {
//         cout << "Failed to open file, please check if file is exist or not." << endl;
//         string p;
//         cin >> p;
//         file_.open(p, ios::in);
//     }
// }

CharScanner::CharScanner(string path)
    : currentLineNumber_(1),
      currentLine_(""),
      isEnd_(false)
{
    file_.open(path, ios::in);
    while (!file_)
    {
        cout << "Failed to open file, please check if file is exist or not." << endl;
        string p;
        cin >> p;
        file_.open(p, ios::in);
    }
    
}

CharScanner::~CharScanner()
{
}

vector<string> CharScanner::getLines()
{
    return lines_;
}

/*
 获取下一个字符
 算法2：先读取一行，然后逐个读取字符，一行读完后接着读下一行，目前有bug待修复
*/
// char CharScanner::getNextChar2()
// {

//     if (isEnd_)
//     {
//         return EOF;
//     }

//     if (currentLineNumber_ == 1 && pointer == 0 && currentLine_ == "")
//     {
//         if (getline(file_, currentLine_))
//         {
//             lines_.push_back(currentLine_);
//             if (currentLine_.length() > pointer)
//             {
//                 return currentLine_[pointer++];
//             }
//             else
//             {
//                 pointer = 0;
//                 currentLine_ = "";
//             }
//         }
//         else
//         {
//             isEnd_ = true;
//             return EOF;
//         }
//     }

//     if (!currentLine_.empty())
//     {
//         if (currentLine_.length() > pointer)
//         {
//             return currentLine_[pointer++];
//         }
//         else
//         {
//             pointer = 0;
//         }
//     }

//     if (getline(file_, currentLine_))
//     {
//         lines_.push_back(currentLine_);
//         currentLineNumber_++;
//         pointer = 0;
//         if (currentLine_.empty()) {
//             return getNextChar2();
//         } else {
//             return currentLine_[pointer++];
//         }
//     }
//     else
//     {
//         isEnd_ = true;
//         return EOF;
//     }
// }

/*
 获取下一个字符
 算法1：逐个读取文本的字符
*/
char CharScanner::getNextChar()
{
    if (rollback_)
    {
        rollback_ = false;
        if (currentChar_ == '\n')
            currentLineNumber_++;
        return currentChar_;
    }

    if (isEnd_)
        return EOF;

    if (!(file_ >> noskipws >> currentChar_))
    {
        isEnd_ = true;
        currentChar_ = EOF;
        file_.close();
    }

    switch (currentChar_)
    {
    case '\n':
        currentLineNumber_++;
    case EOF:
        lines_.push_back(currentLine_);
        currentLine_ = "";
        break;

    default:
        if (currentLine_ == "" && (currentChar_ == '\t' || currentChar_ == ' '))
            break;
        currentLine_ += currentChar_;
        break;
    }
    return currentChar_;
}

bool CharScanner::isEnd()
{
    return isEnd_;
}

void CharScanner::rollback()
{
    if (!isEnd_)
    {
        if (currentChar_ == '\n' && currentLineNumber_ > 1)
            currentLineNumber_ -= 1;
        rollback_ = true;
    }
}

int CharScanner::getCurrentLine()
{
    return currentLineNumber_;
}