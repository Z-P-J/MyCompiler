#ifndef SYNTAX_PARSER_H
#define SYNTAX_PARSER_H

#include "string"
#include "vector"
#include "Token.h"
// #include "Token.cpp"

using namespace std;

#define MAXCHILDREN 4

class SyntaxParser
{
private:

	enum NodeKind //节点类型
	{
		FuncK,
		VoidK,
		IntK,
		IdK,
		ParamsK,
		ParamK,
		CompK,
		ConstK,
		CallK,
		ArgsK,
		Var_DeclK,
		Arry_DeclK,
		Arry_ElemK,
		AssignK,
		OpK,
		Selection_StmtK,
		Iteration_StmtK,
		Return_StmtK
	};

	struct //节点类型和字符串关系
	{
		string str;
		NodeKind kind;
	} nodekind[18] = {
		{"Funck", FuncK}, 
		{"VoidK", VoidK}, 
		{"IntK", IntK}, 
		{"IdK", IdK}, 
		{"ParamsK", ParamsK}, 
		{"ParamK", ParamK}, 
		{"CompK", CompK}, 
		{"ConstK", ConstK}, 
		{"CallK", CallK}, 
		{"ArgsK", ArgsK}, 
		{"Var_DeclK", Var_DeclK}, 
		{"Arry_DeclK", Arry_DeclK}, 
		{"Arry_ElemK", Arry_ElemK}, 
		{"Assign", AssignK},
		{"Op", OpK},
		{"If", Selection_StmtK},
		{"While", Iteration_StmtK},
		{"Return", Return_StmtK}
	};

	struct TreeNode
	{
		struct TreeNode *child[MAXCHILDREN];
		struct TreeNode *sibling;
		NodeKind nodekind;
		Token *token;
	};

	void onError();
	void nextToken();
	TreeNode *buildNode(NodeKind kind);
	void match(Token::TokenType expected);

	TreeNode *declarationList();
	TreeNode *declaration();
	TreeNode *params();
	TreeNode *paramList(TreeNode *p);
	TreeNode *param(TreeNode *p);
	TreeNode *compoundStmt();
	TreeNode *localDeclaration();
	TreeNode *statementList();
	TreeNode *statement();
	TreeNode *expressionStmt();
	TreeNode *selectionStmt();
	TreeNode *iterationStmt();
	TreeNode *returnStmt();
	TreeNode *expression();
	TreeNode *var();
	TreeNode *simpleExpression(TreeNode *p);
	TreeNode *additiveExpression(TreeNode *p);
	TreeNode *term(TreeNode *p);
	TreeNode *factor(TreeNode *p);
	TreeNode *call(TreeNode *p);
	TreeNode *args();

	void printTree(TreeNode *t);
	void printNode(string blank, NodeKind kind, string other);

	string output; //输出文件

	int blank_number = 0;

	std::vector<Token *> tokens_;
	Token *token_;
	Token::TokenType tokenType_;

	// int currentLine_ = 0;
	// int tempLine_ = 0;

public:
	SyntaxParser(vector<Token *> token);
	~SyntaxParser();

	void parse();
};

#endif