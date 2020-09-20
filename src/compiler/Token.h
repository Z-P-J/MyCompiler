#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "string"

class Token
{

public:
    enum TokenType
    {
        EMPTY,
        ENDFILE,
		ERROR,
		IF, // if
		ELSE, // else 
		INT, // int
		RETURN, // return
		VOID, // void
		WHILE, // while
		ID, // id
		NUM, // num
		ASSIGN, // =
		EQ, // ==
		PLUS, // + 
		MINUS, // -
		TIMES, // *
		OVER, // /

        // 分号semicolon
		SEMI, // ;

        // 小括号parenthesis
        // 中括号brackets
        // 大括号brace
        LPAREN, // ( 
		RPAREN, // )
		LBRACKET, // [
		RBRACKET, // ]
		LBRACE, // {
		RBRACE, // }
		COMMA, //,
        LT, // <
		GT, // >
		GEQ, // >=
		NEQ, // !=
		LEQ // <=
    };

    enum WordType
    {
        WORD_ANATATION,
        WORD_SYMBOL,
        WORD_STRING,
        WORD_CHAR,
        WORD_NUM,
        WORD_EOF,
        WORD_ERROR
    };

    struct Word
    {
        std::string str;
        TokenType tok;
    };

    Word reservedWords[6] = {
        {"if", TokenType::IF}, 
        {"int", TokenType::INT}, 
        {"else", TokenType::ELSE}, 
        {"return", TokenType::RETURN}, 
        {"void", TokenType::VOID}, 
        {"while", TokenType::WHILE}
    };

    Word symbolWords[27] = {
        {"[", TokenType::LBRACKET}, 
        {"]", TokenType::RBRACKET}, 
        {"(", TokenType::LPAREN}, 
        {")", TokenType::RPAREN}, 
        {"{", TokenType::LBRACE}, 
        {"}", TokenType::RBRACE},
        {";", TokenType::SEMI},
        {",", TokenType::COMMA},

        {"+", TokenType::PLUS}, 
        {"-", TokenType::MINUS}, 
        {"*", TokenType::TIMES}, 
        {"/", TokenType::OVER}, 
        {"<", TokenType::LT}, 
        {">", TokenType::GT},
        {"=", TokenType::ASSIGN}, 
        // {"++", TokenType::WHILE},
        // {"--", TokenType::WHILE},
        // {"+=", TokenType::WHILE},
        // {"-=", TokenType::WHILE},
        // {"*=", TokenType::WHILE},
        // {"/=", TokenType::WHILE},
        {"<=", TokenType::LEQ}, 
        {">=", TokenType::GEQ}, 
        {"!=", TokenType::NEQ}, 
        {"==", TokenType::EQ}
    };

    Token();
    Token(std::string str);
    Token(std::string str, TokenType type);
    ~Token();
    std::string getValue();
    TokenType getType();
    void setType(TokenType type);
    void appendChar(char ch);
    std::string toString();
    void setWordType(WordType type);
    WordType getWordType();
    int getLine();
    void setLine(int line);

private:
    /* data */
    int line_;
    std::string value_;
    TokenType type_;
    WordType wordType_;
    void parseType();
};


#endif