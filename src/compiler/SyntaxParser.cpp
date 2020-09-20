#include "SyntaxParser.h"

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <strstream>
// #include "Token.cpp"
#include "LexicalAnalyzer.cpp"

using namespace std;

SyntaxParser::SyntaxParser(vector<Token *> tokens) : tokens_(tokens)
{
}

SyntaxParser::~SyntaxParser()
{
}

void SyntaxParser::onError()
{
	cout << "Syntax error at line " << token_->getLine() << ": " << token_->getValue() << endl;
	system("PAUSE");
}

void SyntaxParser::nextToken()
{
	if (tokens_.size() == 0)
	{
		token_ = nullptr;
		tokenType_ = Token::TokenType::ENDFILE;
		return;
	}
	// tempLine_ = currentLine_;
	Token *token = tokens_.front();
	tokens_.erase(tokens_.begin());
	token_ = token;
	// currentLine_ = token_->getLine();
	tokenType_ = token->getType();
	if (tokenType_ == Token::TokenType::EMPTY)
	{
		nextToken();
	}
	else if (tokenType_ == Token::TokenType::ERROR)
	{
		onError();
	}
}

void SyntaxParser::match(Token::TokenType expected)
{
	if (tokenType_ == expected)
		nextToken();
	else
		onError();
}

SyntaxParser::TreeNode *SyntaxParser::buildNode(NodeKind kind)
{
	TreeNode *t = new TreeNode;
	int i;
	for (i = 0; i < MAXCHILDREN; i++)
	{
		t->child[i] = nullptr;
	}
	t->sibling = nullptr;
	t->nodekind = kind;
	t->token = new Token;
	return t;
}

//declaration_list->declaration_list declaration | declaration
SyntaxParser::TreeNode *SyntaxParser::declarationList()
{
	TreeNode *t = declaration();
	TreeNode *p = t;
	while ((tokenType_ != Token::INT) && (tokenType_ != Token::VOID) && (tokenType_ != Token::ENDFILE))
	{
		nextToken();
		if (tokenType_ == Token::TokenType::ENDFILE)
			break;
	}
	while (tokenType_ == Token::TokenType::INT || tokenType_ == Token::TokenType::VOID)
	{
		TreeNode *q;
		q = declaration();
		if (q != nullptr)
		{
			if (t == nullptr)
				t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	match(Token::TokenType::ENDFILE);
	return t;
}

/*
declaration → var-declaration|fun-declaration
var-declaration → type-specifier ID;|type-specifier ID[NUM];
type-specifier → int|void
fun-declaration → type-specifier ID(params)|compound-stmt
*/
SyntaxParser::TreeNode *SyntaxParser::declaration()
{
	TreeNode *t = nullptr;
	TreeNode *p = nullptr;
	TreeNode *q = nullptr;
	TreeNode *s = nullptr;
	TreeNode *a = nullptr;
	if (tokenType_ == Token::TokenType::INT)
	{
		p = buildNode(IntK);
		match(Token::TokenType::INT);
	}
	else if (tokenType_ == Token::TokenType::VOID)
	{
		p = buildNode(VoidK);
		match(Token::TokenType::VOID);
	}
	else
		onError();

	if (p != nullptr && tokenType_ == Token::TokenType::ID)
	{
		q = buildNode(IdK);
		q->token = token_;
		match(Token::TokenType::ID);
		if (tokenType_ == Token::TokenType::LPAREN)
		{
			t = buildNode(FuncK);
			t->child[0] = p;
			t->child[1] = q;

			match(Token::TokenType::LPAREN);
			t->child[2] = params();
			match(Token::TokenType::RPAREN);
			t->child[3] = compoundStmt();
		}
		else if (tokenType_ == Token::TokenType::LBRACKET)
		{
			t = buildNode(Var_DeclK);
			a = buildNode(Arry_DeclK);
			t->child[0] = p;
			t->child[1] = a;
			match(Token::TokenType::LBRACKET);
			s = buildNode(ConstK);
			s->token = token_;
			match(Token::TokenType::NUM);
			a->child[0] = q;
			a->child[1] = s;
			match(Token::TokenType::RBRACKET);
			match(Token::TokenType::SEMI);
		}
		else if (tokenType_ == Token::TokenType::SEMI)
		{
			t = buildNode(Var_DeclK);
			t->child[0] = p;
			t->child[1] = q;
			match(Token::TokenType::SEMI);
		}
		else
			onError();
	}
	else
		onError();
	return t;
}

/*
params → params-list|void
*/
SyntaxParser::TreeNode *SyntaxParser::params()
{
	TreeNode *t = buildNode(ParamsK);
	TreeNode *p = nullptr;
	if (tokenType_ == Token::TokenType::VOID)
	{
		p = buildNode(VoidK);
		match(Token::TokenType::VOID);
		if (tokenType_ == Token::TokenType::RPAREN)
		{
			if (t != nullptr)
				t->child[0] = p;
		}
		else
			t->child[0] = paramList(p);
	}
	else if (tokenType_ == Token::TokenType::INT)
		t->child[0] = paramList(p);
	else
		onError();
	return t;
}

/*
param-list → param-list,param|param
*/
SyntaxParser::TreeNode *SyntaxParser::paramList(TreeNode *k)
{
	TreeNode *t = param(k);
	TreeNode *p = t;
	k = nullptr;
	while (tokenType_ == Token::TokenType::COMMA)
	{
		TreeNode *q = nullptr;
		match(Token::TokenType::COMMA);
		q = param(k);
		if (q != nullptr)
		{
			if (t == nullptr)
				t = q;
			else
				p->sibling = q;
			p = q;
		}
	}
	return t;
}

/*
param → type-specifier ID|type-specifier ID[]
*/
SyntaxParser::TreeNode *SyntaxParser::param(TreeNode *k)
{
	TreeNode *t = buildNode(ParamK);
	TreeNode *p = nullptr;
	TreeNode *q = nullptr;
	if (k == nullptr && tokenType_ == Token::TokenType::VOID)
	{
		p = buildNode(VoidK);
		match(Token::TokenType::VOID);
	}
	else if (k == nullptr && tokenType_ == Token::TokenType::INT)
	{
		p = buildNode(IntK);
		match(Token::TokenType::INT);
	}
	else if (k != nullptr)
		p = k;

	if (p != nullptr)
	{
		t->child[0] = p;
		if (tokenType_ == Token::TokenType::ID)
		{
			q = buildNode(IdK);
			q->token = token_;
			t->child[1] = q;
			match(Token::TokenType::ID);
		}
		else
			onError();

		if (tokenType_ == Token::TokenType::LBRACKET && (t->child[1] != nullptr))
		{
			match(Token::TokenType::LBRACKET);
			t->child[2] = buildNode(IdK);
			match(Token::TokenType::RBRACKET);
		}
		else
			return t;
	}
	else
		onError();
		
	return t;
}

/*
compound-stmt → {local-declarations statement-list}
*/
SyntaxParser::TreeNode *SyntaxParser::compoundStmt()
{
	TreeNode *t = buildNode(CompK);
	match(Token::TokenType::LBRACE);
	t->child[0] = localDeclaration();
	t->child[1] = statementList();
	match(Token::TokenType::RBRACE);
	return t;
}

/*
local-declarations → local-declarations var-declaration|empty
*/
SyntaxParser::TreeNode *SyntaxParser::localDeclaration()
{
	TreeNode *t = nullptr;
	TreeNode *q = nullptr;
	TreeNode *p = nullptr;
	while (tokenType_ == Token::TokenType::INT || tokenType_ == Token::TokenType::VOID)
	{
		p = buildNode(Var_DeclK);
		if (tokenType_ == Token::TokenType::INT)
		{
			TreeNode *q1 = buildNode(IntK);
			p->child[0] = q1;
			match(Token::TokenType::INT);
		}
		else if (tokenType_ == Token::TokenType::VOID)
		{
			TreeNode *q1 = buildNode(VoidK);
			p->child[0] = q1;
			match(Token::TokenType::INT);
		}
		if ((p != nullptr) && (tokenType_ == Token::TokenType::ID))
		{
			TreeNode *q2 = buildNode(IdK);
			q2->token = token_;
			p->child[1] = q2;
			match(Token::TokenType::ID);

			if (tokenType_ == Token::TokenType::LBRACKET)
			{
				TreeNode *q3 = buildNode(Var_DeclK);
				p->child[3] = q3;
				match(Token::TokenType::LBRACKET);
				match(Token::TokenType::RBRACKET);
				match(Token::TokenType::SEMI);
			}
			// else if (tokenType_ == Token::TokenType::SEMI)
			// {
			// 	match(Token::TokenType::SEMI);
			// }
			else
				match(Token::TokenType::SEMI);
		}
		else
			onError();

		if (p != nullptr)
		{
			if (t == nullptr)
				t = p;
			else
				q->sibling = p;
			q = p;
		}
	}
	return t;
}

/*
statement-list → statement-list statement|empty
*/
SyntaxParser::TreeNode *SyntaxParser::statementList()
{
	TreeNode *t = statement();
	TreeNode *p = t;
	while (Token::TokenType::IF == tokenType_ || Token::TokenType::LBRACKET == tokenType_ || Token::TokenType::ID == tokenType_ || Token::TokenType::WHILE == tokenType_ || Token::TokenType::RETURN == tokenType_ || Token::TokenType::SEMI == tokenType_ || Token::TokenType::LPAREN == tokenType_ || Token::TokenType::NUM == tokenType_)
	{
		TreeNode *q;
		q = statement();
		if (q != nullptr)
		{
			if (t == nullptr)
				t = q;
			else
				p->sibling = q;
			p = q;
		}
	}
	return t;
}

/*
statement → expression-stmt|compound-stmt|selection-stmt|
iteration-stmt|return-stmt
*/
SyntaxParser::TreeNode *SyntaxParser::statement()
{
	TreeNode *t = nullptr;
	switch (tokenType_)
	{
	case Token::TokenType::IF:
		t = selectionStmt();
		break;
	case Token::TokenType::WHILE:
		t = iterationStmt();
		break;
	case Token::TokenType::RETURN:
		t = returnStmt();
		break;
	case Token::TokenType::LBRACE:
		t = compoundStmt();
		break;
	case Token::TokenType::ID:
	case Token::TokenType::SEMI:
	case Token::TokenType::LPAREN:
	case Token::TokenType::NUM:
		t = expressionStmt();
		break;
	default:
		nextToken();
		break;
	}
	return t;
}

/*
expression-stmt → expression;|;
*/
SyntaxParser::TreeNode *SyntaxParser::expressionStmt()
{
	TreeNode *t = nullptr;
	if (tokenType_ != Token::TokenType::SEMI)
		t = expression();	

	match(Token::TokenType::SEMI);
	return t;
}

/*
selection-stmt → if(expression) statement | if(expression) statement else statement
*/
SyntaxParser::TreeNode *SyntaxParser::selectionStmt()
{
	TreeNode *t = buildNode(Selection_StmtK);
	match(Token::TokenType::IF);
	match(Token::TokenType::LPAREN);
	if (t != nullptr)
		t->child[0] = expression();
	match(Token::TokenType::RPAREN);
	t->child[1] = statement();
	if (tokenType_ == Token::TokenType::ELSE)
	{
		match(Token::TokenType::ELSE);
		if (t != nullptr)
			t->child[2] = statement();
	}
	return t;
}

/*
iteration-stmt → while(expression) statement
*/
SyntaxParser::TreeNode *SyntaxParser::iterationStmt()
{
	TreeNode *t = buildNode(Iteration_StmtK);
	match(Token::TokenType::WHILE);
	match(Token::TokenType::LPAREN);
	if (t != nullptr)
		t->child[0] = expression();
	match(Token::TokenType::RPAREN);
	if (t != nullptr)
		t->child[1] = statement();
	return t;
}

/*
return-stmt → return;|return expression;
*/
SyntaxParser::TreeNode *SyntaxParser::returnStmt()
{
	TreeNode *t = buildNode(Return_StmtK);
	match(Token::TokenType::RETURN);
	if (tokenType_ != Token::TokenType::SEMI && t != nullptr)
			t->child[0] = expression();
	match(Token::TokenType::SEMI);
	return t;
}

/*
expression → var=expression|simple-expression
*/
SyntaxParser::TreeNode *SyntaxParser::expression()
{
	TreeNode *t = var();
	if (t == nullptr)
		t = simpleExpression(t);
	else
	{
		TreeNode *p = nullptr;
		if (tokenType_ == Token::TokenType::ASSIGN)
		{
			p = buildNode(AssignK);
			match(Token::TokenType::ASSIGN);
			p->child[0] = t;
			p->child[1] = expression();
			return p;
		}
		else
			t = simpleExpression(t);
	}
	return t;
}

/*
var → ID|ID[expression]
*/
SyntaxParser::TreeNode *SyntaxParser::var()
{
	TreeNode *t = nullptr;
	TreeNode *p = nullptr;
	TreeNode *q = nullptr;
	if (tokenType_ == Token::TokenType::ID)
	{
		p = buildNode(IdK);
		p->token = token_;
		match(Token::TokenType::ID);
		if (tokenType_ == Token::TokenType::LBRACKET)
		{
			match(Token::TokenType::LBRACKET);
			q = expression();
			match(Token::TokenType::RBRACKET);

			t = buildNode(Arry_ElemK);
			t->child[0] = p;
			t->child[1] = q;
		}
		else
			t = p;
	}
	return t;
}

/*
simple-expression → additive-expression relop additive-expression | additive-expression
addop → +|-
*/
SyntaxParser::TreeNode *SyntaxParser::simpleExpression(TreeNode *k)
{
	TreeNode *t = additiveExpression(k);
	k = nullptr;
	if (Token::TokenType::EQ == tokenType_ || Token::TokenType::GT == tokenType_ || Token::TokenType::GEQ == tokenType_ || Token::TokenType::LT == tokenType_ || Token::TokenType::LEQ == tokenType_ || Token::TokenType::NEQ == tokenType_)
	{
		TreeNode *q = buildNode(OpK);
		q->token = token_;
		q->child[0] = t;
		t = q;
		match(tokenType_);
		t->child[1] = additiveExpression(k);
	}
	return t;
}

/*
additive-expression → additive-expression addop term|term
*/
SyntaxParser::TreeNode *SyntaxParser::additiveExpression(TreeNode *k)
{
	TreeNode *t = term(k);
	k = nullptr;
	while ((tokenType_ == Token::TokenType::PLUS) || (tokenType_ == Token::TokenType::MINUS))
	{
		TreeNode *q = buildNode(OpK);
		q->token = token_;
		q->child[0] = t;
		match(tokenType_);
		q->child[1] = term(k);
		t = q;
	}
	return t;
}

/*
term → term mulop factor|factor
mulop → *|/
*/
SyntaxParser::TreeNode *SyntaxParser::term(TreeNode *k)
{
	TreeNode *t = factor(k);
	k = nullptr;
	while ((tokenType_ == Token::TokenType::TIMES) || (tokenType_ == Token::TokenType::OVER))
	{
		TreeNode *q = buildNode(OpK);
		q->token = token_;
		q->child[0] = t;
		t = q;
		match(tokenType_);
		q->child[1] = factor(k);
	}
	return t;
}

/*
factor → (expression)|var|call|NUM
*/
SyntaxParser::TreeNode *SyntaxParser::factor(TreeNode *k)
{
	TreeNode *t = nullptr;
	if (k != nullptr)
	{
		if (tokenType_ == Token::TokenType::LPAREN && k->nodekind != Arry_ElemK)
			t = call(k);
		else
			t = k;
	}
	else
	{
		switch (tokenType_)
		{
		case Token::TokenType::LPAREN:
			match(Token::TokenType::LPAREN);
			t = expression();
			match(Token::TokenType::RPAREN);
			break;
		case Token::TokenType::ID:
			k = var();
			if (Token::TokenType::LPAREN == tokenType_ && k->nodekind != Arry_ElemK)
				t = call(k);
			else
				t = k;
			break;
		case Token::TokenType::NUM:
			t = buildNode(ConstK);
			if (t != nullptr && tokenType_ == Token::TokenType::NUM)
				t->token = token_;
			match(Token::TokenType::NUM);
			break;
		default:
			nextToken();
			break;
		}
	}
	return t;
}

/*
call → ID(args)
*/
SyntaxParser::TreeNode *SyntaxParser::call(TreeNode *k)
{
	TreeNode *t = buildNode(CallK);
	if (k != nullptr)
		t->child[0] = k;
	match(Token::TokenType::LPAREN);
	if (tokenType_ != Token::TokenType::RPAREN && k != nullptr)
		t->child[1] = args();
	match(Token::TokenType::RPAREN);
	return t;
}

/*
args → arg-list|empty
arg-list → arg-list, expression|expression
*/
SyntaxParser::TreeNode *SyntaxParser::args()
{
	TreeNode *t = buildNode(ArgsK);
	TreeNode *s = nullptr;
	TreeNode *p = nullptr;
	if (tokenType_ != Token::TokenType::RPAREN)
	{
		s = expression();
		p = s;
		while (tokenType_ == Token::TokenType::COMMA)
		{
			TreeNode *q;
			match(Token::TokenType::COMMA);
			q = expression();
			if (q != nullptr)
			{
				if (s == nullptr)
					s = q;
				else
					p->sibling = q;
				p = q;
			}
		}
	}
	if (s != nullptr)
		t->child[0] = s;
	return t;
}

void SyntaxParser::printTree(TreeNode *t)
{
	string blank = "  ";
	int i;
	for (i = 0; i < blank_number; i++)
		blank += " ";
	if (t != nullptr)
	{
		if (t->nodekind == OpK || t->nodekind == IdK || t->nodekind == ConstK)
		{
			string value = t->token->getValue();
			if (value == "")
				value = " ";
			printNode(blank, t->nodekind, value);
		}
		else
			printNode(blank, t->nodekind, "");
	}
	for (i = 0; i < MAXCHILDREN; i++)
	{
		if (t->child[i] != nullptr)
		{
			blank_number += 2;
			printTree(t->child[i]);
			blank_number -= 2;
		}
	}
	if (t->sibling != nullptr)
		printTree(t->sibling);
}

void SyntaxParser::printNode(string blank, NodeKind kind, string other)
{
	int i;
	for (i = 0; i < 19; i++)
	{
		if (kind == nodekind[i].kind)
		{
			string str = nodekind[i].str;
			string out = blank + str;
			if (other != "")
				out += (": " + other);
			cout << out << endl;
			output += out + "\n";
			return;
		}
	}
}

void SyntaxParser::parse()
{
	cout << "\nSyntaxParser starts to parse...\n"
		 << endl;
	TreeNode *t;
	nextToken();
	t = declarationList();
	cout << "Syntax tree:" << endl;
	printTree(t);

	ofstream write;
	write.open("./SyntaxParser-Result.txt");
	write << "Syntax tree:" << endl;
	write << output;
	write.close();
}

int main()
{
	cout << "Please enter the file path:" << endl;
	string path;
	cin >> path;
	LexicalAnalyzer analyzer(path);
	SyntaxParser parser(analyzer.analyze());
	parser.parse();
	system("PAUSE");
	return 0;
}
