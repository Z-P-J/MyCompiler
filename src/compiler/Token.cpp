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

Token::Token(string str, TokenType type) : value_(str), type_(type)
{
}

Token::~Token()
{
}

string Token::getValue()
{
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
    // switch (value_)
    // {
    // case EOF:
    //     type_ == ENDFILE;
    //     break;

    // default:
    //     type_ == ERROR;
    //     break;
    // }
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

string Token::toString()
{
    switch (wordType_)
    {
    case WordType::WORD_EOF:
        return "EOF";

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