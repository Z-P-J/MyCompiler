#include "LexicalAnalyzer.h"

#include "CharScanner.cpp"
#include "Token.cpp"
#include "iostream"
#include "string"
// #include "vector"
#include "map"
#include "fstream"

using namespace std;

// LexicalAnalyzer::LexicalAnalyzer(char *filePath)
//     : scanner_(new CharScanner(filePath)), currentState_(START)
// {
// }

LexicalAnalyzer::LexicalAnalyzer(string filePath)
    : scanner_(new CharScanner(filePath)), currentState_(START)
{
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

char LexicalAnalyzer::getNextChar()
{
    return scanner_->getNextChar();
}

vector<Token *> LexicalAnalyzer::analyze()
{
    cout << "\nLexicalAnalyzer starts to analyze...\n" << endl;
    map<int, vector<Token *>> tokenMap;
    while (!scanner_->isEnd())
    {
        Token *token = new Token;
        currentState_ = START;

        while (currentState_ != DONE)
        {
            char c = getNextChar();
            bool rollback = true;

            switch (currentState_)
            {
            case START:
                if (c == EOF)
                {
                    token->setWordType(Token::WordType::WORD_EOF);
                    rollback = false;
                    currentState_ = DONE;
                }
                else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ',' || c == ';' || c == '*' || c == '+' || c == '-')
                {
                    rollback = false;
                    currentState_ = DONE;
                    token->setWordType(Token::WordType::WORD_SYMBOL);
                }
                else if (c == ' ' || c == '\n' || c == '\t')
                    continue;
                else if (isdigit(c))
                    currentState_ = IN_NUM;
                else if (isalpha(c))
                    currentState_ = IN_ID;
                else if (c == '>' || c == '<' || c == '!' || c == '=')
                    currentState_ = IN_COMPARE;
                else if (c == '/')
                    currentState_ = IN_ANATATION;
                else
                {
                    rollback = false;
                    token->setWordType(Token::WordType::WORD_ERROR);
                    currentState_ = DONE;
                }
                break;

            case IN_ANATATION:
                if (c == '*')
                    currentState_ = IN_ANATATION1;
                else
                {
                    if (c == '=')
                        rollback = false;
                    currentState_ = DONE;
                    token->setWordType(Token::WordType::WORD_SYMBOL);
                }
                break;

            case IN_ANATATION1:
                if (c == '*')
                    currentState_ = IN_ANATATION2;
                else if (c == EOF)
                {
                    currentState_ = DONE;
                    token->setWordType(Token::WordType::WORD_ERROR);
                }
                break;

            case IN_ANATATION2:
                if (c == '/')
                {
                    rollback = false;
                    currentState_ = DONE;
                    token->setWordType(Token::WordType::WORD_ANATATION);
                }
                else if (c == EOF)
                {
                    currentState_ = DONE;
                    token->setWordType(Token::WordType::WORD_ERROR);
                }
                else
                    currentState_ = IN_ANATATION1;
                break;

            case IN_NUM:
                // if (!isdigit(c))
                // {
                //     if (c == '.')
                //         currentState_ = IN_NUM1;
                //     else
                //     {
                //         token->setWordType(Token::WordType::WORD_NUM);
                //         currentState_ = DONE;
                //     }
                // }

                if (!isdigit(c))
                {
                    token->setWordType(Token::WordType::WORD_NUM);
                    currentState_ = DONE;
                }
                break;

            // case IN_NUM1:
            //     if (isdigit(c))
            //         currentState_ = IN_NUM2;
            //     else
            //     {
            //         token->setWordType(Token::WordType::WORD_NUM);
            //         currentState_ = DONE;
            //     }
            //     break;

            // case IN_NUM2:
            //     if (!isdigit(c))
            //     {
            //         token->setWordType(Token::WordType::WORD_NUM);
            //         currentState_ = DONE;
            //     }
            //     break;

            case IN_ID:
                if (c != '_' && !isalpha(c) && !isdigit(c))
                {
                    token->setWordType(Token::WordType::WORD_SYMBOL);
                    currentState_ = DONE;
                }
                break;
            case IN_COMPARE:
                if (c == '=')
                    rollback = false;
                currentState_ = DONE;
                token->setWordType(Token::WordType::WORD_SYMBOL);
                break;
            }

            if (currentState_ == DONE)
            {
                if (rollback)
                    scanner_->rollback();
                else
                    token->appendChar(c);
                if (token->getWordType() != Token::WordType::WORD_ANATATION)
                {
                    int currentLine = scanner_->getCurrentLine();
                    token->setLine(currentLine);
                    auto iter = tokenMap.find(currentLine);
                    if (iter != tokenMap.end())
                        iter->second.push_back(token);
                    else
                    {
                        vector<Token *> tokens;
                        tokens.push_back(token);
                        tokenMap.insert(pair<int, vector<Token *>>(currentLine, tokens));
                    }
                }
            }
            else
                token->appendChar(c);
        }
    }
    cout << endl;
    vector<Token *> tokens;

    string output;
    ofstream write;
	write.open("./LexicalAnalyzer-Result.txt");
    for (int i = 1; i <= scanner_->getLines().size(); i++)
    {
        write << i << ": " << scanner_->getLines()[i - 1] << endl;
        cout << i << ": " << scanner_->getLines()[i - 1] << endl;

        auto iter = tokenMap.find(i);
        if (iter != tokenMap.end())
        {
            for (Token *token : iter->second)
            {
                cout << "    " << i << ": " << token->toString() << endl;
                write << "    " << i << ": " << token->toString() << endl;
                tokens.push_back(token);
            }
        }
    }
	write.close();
    return tokens;
}

// int main()
// {
//     cout << "Please enter the file path:" << endl;
// 	string path;
// 	cin >> path;
//     LexicalAnalyzer analyzer(path);
//     analyzer.analyze();

//     system("pause");
//     return 0;
// }