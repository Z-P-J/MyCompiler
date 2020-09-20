#ifndef TEST_H
#define TEST_H

#include "string"

using namespace std;

#define BUFLEN 256
#define MAXLEN 256
#define MAXTOKENLEN 40
#define MAXCHILDREN 4

class Test
{
private:
	/* data */
	enum TokenType
	{
		// ENDFILE,
		// ERROR,
		// IF,
		// ELSE,
		// INT,
		// RETURN,
		// VOID,
		// WHILE,
		// ID,
		// NUM,
		// ASSIGN,
		// EQ,
		// LT,
		// PLUS,
		// MINUS,
		// TIMES,
		// OVER,
		// LPAREN,
		// RPAREN,
		// SEMI,
		// LBRACKET,
		// RBRACKET,
		// LBRACE,
		// RBRACE,
		// COMMA,
		// GT,
		// GEQ,
		// NEQ,
		// LEQ


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
		SEMI, // ;
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

	enum StateType
	{
		START,
		INASSIGN,
		INCOMMENT,
		INNUM,
		INID,
		DONE,
		PRECOMMENT,
		AFTERCOMMENT
	};

	struct
	{
		char *str;
		TokenType tok;
	} ReserverWords[6] = {{"if", IF}, {"else", ELSE}, {"int", INT}, {"return", RETURN}, {"void", VOID}, {"while", WHILE}};

	enum NodeKind //节点类型
	{
		FuncK,
		IntK,
		IdK,
		ParamsK,
		ParamK,
		CompK,
		ConstK,
		CallK,
		ArgsK,
		VoidK,
		Var_DeclK,
		Arry_DeclK,
		Arry_ElemK,
		AssignK /*,WhileK*/,
		OpK,
		Selection_StmtK,
		Iteration_StmtK,
		Return_StmtK
	};
	struct //节点类型和字符串关系
	{
		string str;
		NodeKind nk;
	} nodekind[18] = {{"Funck", FuncK}, {"IntK", IntK}, {"IdK", IdK}, {"ParamsK", ParamsK}, {"ParamK", ParamK}, {"CompK", CompK}, {"ConstK", ConstK}, {"CallK", CallK}, {"ArgsK", ArgsK}, {"VoidK", VoidK}, {"Var_DeclK", Var_DeclK}, {"Arry_DeclK", Arry_DeclK}, {"Arry_ElemK", Arry_ElemK}, {"AssignK", AssignK},
					  /*{"WhileK",WhileK},*/ {"OpK", OpK},
					  {"Selection_StmtK", Selection_StmtK},
					  {"Iteration_StmtK", Iteration_StmtK},
					  {"Return_StmtK", Return_StmtK}};

	struct //符号与字符串关系
	{
		string str;
		TokenType tk;
	} Ope[11] = {{"=", ASSIGN}, {"==", EQ}, {"<", LT}, {"+", PLUS}, {"-", MINUS}, {"*", TIMES}, {"/", OVER}, {">", GT}, {">=", GEQ}, {"!=", NEQ}, {"<=", LEQ}};

	TokenType token;
	struct TreeNode
	{
		struct TreeNode *child[4];
		struct TreeNode *sibling;
		int lineno;
		NodeKind nodekind;
		union {
			TokenType op;
			int val;
			const char *name;
		} attr;
	};

	int lineno = 0;
	int linepos = 0; //读取的字符在lineBuf的位置
	int EOF_FLAG = false;
	int bufsize = 0; //lineBuf的长度
	char lineBuf[BUFLEN];
	FILE *source;
	char tokenString[MAXTOKENLEN + 1];
	string output; //输出文件

	int blank_number = 0;

public:
	Test(/* args */);
	~Test();

	TreeNode *declaration_list();
	TreeNode *declaration();
	TreeNode *params();
	TreeNode *param_list(TreeNode *p);
	TreeNode *param(TreeNode *p);
	TreeNode *compound_stmt();
	TreeNode *local_declaration();
	TreeNode *statement_list();
	TreeNode *statement();
	TreeNode *expression_stmt();
	TreeNode *selection_stmt();
	TreeNode *iteration_stmt();
	TreeNode *return_stmt();
	TreeNode *expression();
	TreeNode *var();
	TreeNode *simple_expression(TreeNode *p);
	TreeNode *additive_expression(TreeNode *p);
	TreeNode *term(TreeNode *p);
	TreeNode *factor(TreeNode *p);
	TreeNode *call(TreeNode *p);
	TreeNode *args();

	void parse();

	void PreOrder(TreeNode *t);
	void UnGetNextChar();
	int GetNextChar();
	TokenType ReservedLookUp(char *s);
	TokenType GetToken();
	string OpeLookUp(TokenType tk);
	string Change(NodeKind nk);
	char *copyString(char *s);
	void match(TokenType expected);
	TreeNode *newNode(NodeKind kind);
};

#endif