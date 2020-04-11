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
            // if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ',' || c == ':' || c == ';') {
            //     rollback = false;
            //     currentState_ = DONE;
            //     token.setWordType(Token::WordType::WORD_SYMBOL);
            // } else 
            if (c == EOF) {
                token.setWordType(Token::WordType::WORD_EOF);
                rollback = false;
                currentState_ = DONE;
            }
            
            

            switch (currentState_)
            {
            case START:
            if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ',' || c == ':' || c == ';') {
                rollback = false;
                currentState_ = DONE;
                token.setWordType(Token::WordType::WORD_SYMBOL);
            } else if (c == ' ' || c == '\n' || c == '\t')
                    continue;
                else if (isdigit(c))
                    currentState_ = INNUM;
                else if (isalpha(c))
                    currentState_ = INID;
                else if (c == '>' || c == '<' || c == '!' || c == '=')
                    currentState_ = INCOMPARE;
                else if (c == '+' || c == '-' || c == '*' || c == '%')
                    currentState_ = INOPERATE;
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
                // if (!rollback)
                //     token.appendChar(c);
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