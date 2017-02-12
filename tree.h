#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include "lex.h"

enum Tree_Node_Type {
	NODE_IF,
	NODE_WHILE,
	NODE_FOR,
	NODE_BLOCK,
	NODE_STATEMENT,
	NODE_DECL
};

enum Expression_Type {
	EXP_BINARY,
	EXP_UNARY
};

enum Expression_Binary_Type {
	BINARY_ADDITION,
	BINARY_SUBTRACTION,
	BINARY_MULTIPLICATION,
	BINARY_DIVISION,
	BINARY_MODULUS,
	BINARY_SHIFT_LEFT,
	BINARY_SHIFT_RIGHT,
	BINARY_BITWISE_AND,
	BINARY_BITWISE_OR,
	BINARY_BITWISE_XOR,
	BINARY_LOGICAL_AND,
	BINARY_LOGICAL_OR,
	BINARY_INCREMENT_BY,
	BINARY_DECREMENT_BY,
	BINARY_MULTIPLY_BY,
	BINARY_DIVIDE_BY,
	BINARY_MODULUS_BY,
	BINARY_AND_BY,
	BINARY_OR_BY,
	BINARY_XOR_BY,
	BINARY_SHIFT_LEFT_BY,
	BINARY_SHIFT_RIGHT_BY
};

enum Expression_Unary_Type {
	UNARY_BITWISE_NOT,
	UNARY_LOGICAL_NOT,
	UNARY_OPEN_PARENTHESIS,
	UNARY_CLOSE_PARENTHESIS,
};

enum Operator_Assoc {
	ASSOC_LEFT,
	ASSOC_RIGHT
};

enum Operator_Operands {
	OP_BINARY,
	OP_UNARY
};

struct Operator_Descriptor {
	Operator_Descriptor(std::string s, int p, Operator_Assoc a, Operator_Operands o)
						: str(s), prec(p), assoc(a), operand_type(o) {}

	std::string str;
	int prec;
	Operator_Assoc assoc;
	Operator_Operands operand_type;
};

struct Expression {
	Expression* parent;
	Expression_Type type;
	std::string word;

	Expression(Expression_Type t): type(t) {}
	virtual ~Expression() {}
	bool isBinaryType(Expression_Binary_Type) const;
	bool isUnaryType(Expression_Unary_Type) const;
};

struct Expression_Operator : public Expression {
	Expression_Operator(Expression_Type t): Expression(t) {}

	const Operator_Descriptor* desc = nullptr;	
};

struct Expression_Binary : public Expression_Operator {
	Expression_Binary(): Expression_Operator(EXP_BINARY) {}
	static Expression_Binary_Type wordToBinaryType(const std::string&);
	
	Expression_Binary_Type optype;	
	Expression* left = nullptr;
	Expression* right = nullptr;
};

struct Expression_Unary : public Expression_Operator {
	Expression_Unary(): Expression_Operator(EXP_UNARY) {}
	static Expression_Binary_Type wordToUnaryType(const std::string&);
	
	Expression_Unary_Type optype;
	Expression* operand = nullptr;
};

class Tree_Node {
	
	public:
		Tree_Node(Tree_Node_Type);
		~Tree_Node();
	
		int line = 0;
		Tree_Node_Type type;

	private:

};


class Parser {
	
	public:
		Parser();
		static Tree_Node* generateSyntaxTree(std::vector<Token>&);

	private:
		Tree_Node* root;
		Token* focus;
		Token* prev_focus;
		std::vector<Token>* tokens;
		int num_tokens;
		int token_index;
		int marked;

		void die(const std::string&);
		void focusTokens(std::vector<Token>&);
		void next();
		void peek();
		void setIndex(int);
		void handleIf();
		void markOperator(int, int);
		bool isOperator(int);
		bool isIdentifier(const std::string&);
		void eatOperator(int);
		void eatIdentifier(const std::string&);
		void printOn();
		Token* atIndex(int index);
		Expression* parseExpression();
};

#endif
