#include "LexicalAnalyzer.h"

#include "CharScanner.cpp"
#include "Token.cpp"
#include "iostream"
#include "string"

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(char *filePath)
    : scanner_(new CharScanner(filePath)), currentState_(START)
{
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

void LexicalAnalyzer::analyze()
{
    // int lineCount = 1;
    while (!scanner_->isEnd())
    {
        // string token = "";
        Token token;
        currentState_ = START;
        while (currentState_ != DONE)
        {
            char c = scanner_->getNextChar();
            // cout << "c=" << c << endl;
            bool rollback = true;

            if (c == EOF)
            {
                token.setWordType(Token::WordType::WORD_EOF);
                rollback = false;
                currentState_ = DONE;
            }

            switch (currentState_)
            {
            case START:
                if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ',' || c == ':' || c == ';')
                {
                    rollback = false;
                    currentState_ = DONE;
                    token.setWordType(Token::WordType::WORD_SYMBOL);
                }
                else if (c == ' ' || c == '\n' || c == '\t')
                    continue;
                else if (isdigit(c))
                    currentState_ = INNUM;
                else if (isalpha(c))
                    currentState_ = INID;
                else if (c == '>' || c == '<' || c == '!' || c == '=')
                    currentState_ = INCOMPARE;
                else if (c == '*') //  || c == '/'
                    currentState_ = INOPERATE;
                else if (c == '/')
                    currentState_ = INANATATION;
                else if (c == '+')
                    currentState_ = IN_ADD;
                else if (c == '-')
                    currentState_ = IN_SUB;
                else if (c == '\'')
                    currentState_ = INCHAR;
                else if (c == '"')
                    currentState_ = INSTRING;
                else
                {
                    rollback = false;
                    token.setWordType(Token::WordType::WORD_ERROR);
                    currentState_ = DONE;
                }
                break;

            case INANATATION:
                if (c == '*') {
                    // 是注释
                    currentState_ = INANATATION1;
                } else {
                    // 不是注释，而是除法符号
                    if (c == '=')
                        rollback = false;
                    currentState_ = DONE;
                    token.setWordType(Token::WordType::WORD_SYMBOL);
                }
                break;

            case INANATATION1:
                if (c == '*') {
                    currentState_ = INANATATION2;
                } else if (c == EOF) {
                    currentState_ = DONE;
                    token.setWordType(Token::WordType::WORD_ERROR);
                }
                break;

            case INANATATION2:
                if (c == '/') {
                    rollback = false;
                    currentState_ = DONE;
                    token.setWordType(Token::WordType::WORD_ANATATION);
                } else if (c == EOF) {
                    currentState_ = DONE;
                    token.setWordType(Token::WordType::WORD_ERROR);
                } else {
                    currentState_ = INANATATION1;
                }
                break;

            case INNUM:
                if (!isdigit(c))
                {
                    if (c == '.')
                        currentState_ = INNUM1;
                    else
                    {
                        token.setWordType(Token::WordType::WORD_NUM);
                        currentState_ = DONE;
                    }
                }
                break;

            case INNUM1:
                if (isdigit(c))
                    currentState_ = INNUM2;
                else
                {
                    token.setWordType(Token::WordType::WORD_NUM);
                    currentState_ = DONE;
                }
                break;

            case INNUM2:
                if (!isdigit(c))
                {
                    token.setWordType(Token::WordType::WORD_NUM);
                    currentState_ = DONE;
                }
                break;

            case INID:
                if (c != '_' && !isalpha(c) && !isdigit(c))
                {
                    token.setWordType(Token::WordType::WORD_SYMBOL);
                    currentState_ = DONE;
                }
                break;
            case IN_ADD:
                if (c == '+')
                {
                    rollback = false;
                }
            case IN_SUB:
                if (c == '-')
                {
                    rollback = false;
                }
            case INCOMPARE:
                if (c == '=')
                    rollback = false;
                currentState_ = DONE;
                token.setWordType(Token::WordType::WORD_SYMBOL);
                break;

            case INOPERATE:
                if (c == '=')
                    rollback = false;
                currentState_ = DONE;
                token.setWordType(Token::WordType::WORD_SYMBOL);
                break;

            case INCHAR:
                if (c == '\'')
                {
                    rollback = false;
                    token.setWordType(Token::WordType::WORD_CHAR);
                    currentState_ = DONE;
                }
                break;

            case INSTRING:
                if (c == '"')
                {
                    rollback = false;
                    currentState_ = DONE;
                    token.setWordType(Token::WordType::WORD_STRING);
                }
                break;
            }

            if (currentState_ == DONE)
            {
                if (rollback)
                    scanner_->rollback();
                else
                    token.appendChar(c);
                cout << scanner_->getCurrentLine() << ": " << token.toString() << endl;
            }
            else
                token.appendChar(c);
        }
    }
}

int main()
{
    char *path = "./src/compiler/TEST.txt";
    LexicalAnalyzer analyzer(path);
    analyzer.analyze();

    system("pause");
    return 0;
}