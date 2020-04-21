#ifndef TOKEN_H
#define TOKEN_H

#include "string"

class Token
{

public:
    enum TokenType
    {
        EMPTY,

        // 错误、结束
        ENDFILE,
        ERROR,
        // 保留字
        AUTO,
        BREAK,
        CASE,
        CHAR,
        CONST,
        CONTINUE,
        DEFAULT,
        DO,
        DOUBLE,
        ELSE,
        ENUM,
        EXTERN,
        FLOAT,
        FOR,
        GOTO,
        IF,
        INT,
        LONG,
        REGISTER,
        RETURN,
        SHORT,
        SIGNED,
        SIZEOF,
        STATIC,
        STRUCT,
        SWITCH,
        TYPEDEF,
        UNION,
        UNSIGNED,
        VOID,
        VOLATILE,
        WHILE,
        //其他token
        ID,
        NUM,
        CHARACTER,
        STRING,
        // 特殊符号
        //+、一、*、/、++、一、+=、-=、*=、<、<=、>、>=、=、！=、=、；、, 、（、）、[、]、
        //{、}、/*、*/、：
        PLUS,
        MINUS,
        TIMES,
        OVER,
        SELFPLUS,
        SELEMINUS,
        PLUSASSIGN,
        MINUSASSIGN,
        TIMESASSIGN,
        LT,
        LEQ,
        GT,
        GEQ,
        EQ,
        NEQ,
        ASSIGN,
        SEMT,
        COVMMA,
        LPAREN,
        RPAREN,
        LBRACKET,
        RBRACKET,
        LCBRACKET,
        RCBRACKET,
        LCOMMENT,
        RCOMMENT,
        COLON,
        OOOOOOOO
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

    Word reservedWords[9] = {
        {"if", TokenType::IF}, 
        {"int", TokenType::INT}, 
        {"else", TokenType::ELSE}, 
        {"return", TokenType::RETURN}, 
        {"void", TokenType::VOID}, 
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"char", TokenType::CHAR},
        {"float", TokenType::FLOAT}
    };

    Word symbolWords[27] = {
        {"(", TokenType::IF}, 
        {")", TokenType::INT}, 
        {"[", TokenType::ELSE}, 
        {"]", TokenType::RETURN}, 
        {"{", TokenType::VOID}, 
        {"}", TokenType::WHILE},
        {";", TokenType::WHILE},
        {",", TokenType::WHILE},
        {":", TokenType::WHILE},

        {"+", TokenType::IF}, 
        {"-", TokenType::INT}, 
        {"*", TokenType::ELSE}, 
        {"/", TokenType::RETURN}, 
        {"<", TokenType::VOID}, 
        {">", TokenType::VOID}, 
        {"!", TokenType::VOID}, 
        {"=", TokenType::VOID}, 
        {"++", TokenType::WHILE},
        {"--", TokenType::WHILE},
        {"+=", TokenType::WHILE},
        {"-=", TokenType::WHILE},
        {"*=", TokenType::WHILE},
        {"/=", TokenType::WHILE},
        {"<=", TokenType::VOID}, 
        {">=", TokenType::VOID}, 
        {"!=", TokenType::VOID}, 
        {"==", TokenType::WHILE}
    };

    // Word symbolWords[18] = {
    //     {"+", IF}, 
    //     {"-", INT}, 
    //     {"*", ELSE}, 
    //     {"/", RETURN}, 
    //     {"<", VOID}, 
    //     {">", VOID}, 
    //     {"!", VOID}, 
    //     {"=", VOID}, 
    //     {"++", WHILE},
    //     {"--", WHILE},
    //     {"+=", WHILE},
    //     {"-=", WHILE},
    //     {"*=", WHILE},
    //     {"/=", WHILE},
    //     {"<=", VOID}, 
    //     {">=", VOID}, 
    //     {"!=", VOID}, 
    //     {"==", WHILE}
    // };

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

private:
    /* data */
    std::string value_;
    TokenType type_;
    WordType wordType_ = WordType::WORD_ERROR;
    void parseType();
};

#endif