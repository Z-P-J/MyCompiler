#include "Token.h"

#include "iostream"

using namespace std;
// using namespace Token::TokenType;

    Token::Token() : Token("", EMPTY)
{
}

Token::Token(string str) : Token(str, EMPTY)
{
    parseType();
}

Token::Token(string str, TokenType type) : line_(0), value_(str), type_(type), wordType_(WordType::WORD_ERROR)
{
}

Token::~Token()
{
}

string Token::getValue()
{
    if (wordType_ == WordType::WORD_EOF) {
        return "EOF";
    }
    return value_;
}

Token::TokenType Token::getType()
{
    if (type_ == EMPTY)
    {
        parseType();
    }
    return type_;
}

void Token::parseType()
{
    switch (wordType_)
    {
    case WordType::WORD_EOF:
        type_ = ENDFILE;
        break;

    case WordType::WORD_NUM:
        type_ = NUM;
        break;

    case WordType::WORD_SYMBOL:
        for (Word word : reservedWords)
        {
            if (value_ == word.str) {
                type_ = word.tok;
                return;
            }
        }
        for (Word word : symbolWords)
        {
            if (value_ == word.str) {
                type_ = word.tok;
                return;
            }
        }
        type_ = ID;
        break;

    case WordType::WORD_ERROR:
        type_ = ERROR;
        break;
        
    case WordType::WORD_ANATATION:
    default:
        break;
    }

}

void Token::setType(TokenType type)
{
    type_ = type;
}

void Token::appendChar(char ch)
{
    value_ += ch;
}

void Token::setWordType(WordType type)
{
    wordType_ = type;
}

Token::WordType Token::getWordType()
{
    return wordType_;
}

string Token::toString()
{
    switch (wordType_)
    {
    case WordType::WORD_EOF:
        return "EOF";

    case WordType::WORD_ANATATION:
        return  value_;

    case WordType::WORD_NUM:
        return "NUM:, val= " + value_;

    case WordType::WORD_CHAR:
        return "CHAR, val= " + value_;

    case WordType::WORD_STRING:
        return "STRING, val= " + value_;

    case WordType::WORD_SYMBOL:
        for (Word word : reservedWords)
        {
            if (value_ == word.str)
                return "Reserved Word: " + value_;
        }
        for (Word word : symbolWords)
        {
            if (value_ == word.str)
                return value_;
        }
        return "ID, name= " + value_;

    case WordType::WORD_ERROR:
    default:
        return "ERROR: " + value_;
    }
}

int Token::getLine() {
    return line_;
}

void Token::setLine(int line) {
    line_ = line;
}

