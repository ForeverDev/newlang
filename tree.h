#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <stdint.h>
#include "lex.h"

#define MOD_STATIC  (0x1 << 0)
#define MOD_CONST   (0x1 << 1)
#define MOD_FOREIGN (0x1 << 2)

enum Ast_Node_Type {
	AST_IF,
	AST_WHILE,
	AST_FOR,
	AST_BLOCK,
	AST_STATEMENT,
	AST_DECLARATION,
	AST_PROCEDURE,
};

enum Datatype_Type {
	DATA_INTEGER,
	DATA_FLOAT,
	DATA_BYTE,
	DATA_PROCEDURE,
	DATA_STRUCT
};

enum Expression_Type {
	EXP_BINARY,
	EXP_UNARY,
	EXP_INTEGER,
	EXP_FLOAT,
	EXP_CAST,
	EXP_STRING,
	EXP_IDENTIFIER
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
	BINARY_SHIFT_RIGHT_BY,
	BINARY_COMPARE,
	BINARY_COMPARE_NOT,
	BINARY_ASSIGNMENT
};

enum Expression_Unary_Type {
	UNARY_BITWISE_NOT,
	UNARY_LOGICAL_NOT,
	UNARY_OPEN_PARENTHESIS,
	UNARY_CLOSE_PARENTHESIS,
};

enum Expression_Leaf {
	LEAF_LEFT,
	LEAF_RIGHT
};

enum Operator_Assoc {
	ASSOC_LEFT,
	ASSOC_RIGHT
};

enum Operator_Operands {
	OP_BINARY,
	OP_UNARY
};

struct Datatype_Base {
	Datatype_Base(Datatype_Type t): type(t) {}
	virtual ~Datatype_Base() {}
	static std::string toString(const Datatype_Base&);
	
	Datatype_Type type;
	std::string type_name;
	int size;
	uint32_t mods = 0;
	bool is_ptr = false;
	int  size_ptr = 0;
	bool is_array = false;
	std::vector<int> size_array;
	std::string as_string;
};

struct Var_Declaration {
	Datatype_Base* dt;
	std::string identifier;	
	bool named = true; // for unnamed function args
	int offset; // from bp or base of struct
	std::string as_string;
};

struct Datatype_Procedure : public Datatype_Base {
	Datatype_Procedure(): Datatype_Base(DATA_PROCEDURE) {}
	
	Datatype_Base* ret;
	std::vector<Var_Declaration*> args;
};

struct Datatype_Struct : public Datatype_Base {

	std::vector<Var_Declaration*> members;
};

struct Datatype_Integer : public Datatype_Base {
	Datatype_Integer(): Datatype_Base(DATA_INTEGER) {
		size = 8;
		type_name = "int";
	}
};

struct Datatype_Float : public Datatype_Base {
	Datatype_Float(): Datatype_Base(DATA_FLOAT) {
		size = 8;
		type_name = "float";
	}
};

struct Datatype_Byte : public Datatype_Base {
	Datatype_Byte(): Datatype_Base(DATA_BYTE) {
		size = 1;
		type_name = "byte";
	}
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
	Expression_Leaf side;
	std::string word;

	Expression(Expression_Type t): type(t) {}
	virtual ~Expression() {}
	void print(int) const;
	bool isBinaryType(Expression_Binary_Type) const;
	bool isUnaryType(Expression_Unary_Type) const;
};

struct Expression_Integer : public Expression {
	Expression_Integer(): Expression(EXP_INTEGER) {}

	int64_t value;
};

struct Expression_Float : public Expression {
	Expression_Float(): Expression(EXP_FLOAT) {}

	double value;
};	

struct Expression_Identifier : public Expression {
	Expression_Identifier(): Expression(EXP_IDENTIFIER) {}

	std::string value;
};

struct Expression_String : public Expression {
	Expression_String(): Expression(EXP_STRING) {}

	std::string value;
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
	static Expression_Unary_Type wordToUnaryType(const std::string&);
	
	Expression_Unary_Type optype;
	Expression* operand = nullptr;
};

struct Expression_Cast : public Expression_Operator {
	Expression_Cast(): Expression_Operator(EXP_CAST) {
		// is this a bad thing to do? for now, every cast has its
		// own desc object instead of a pointer into operator_table....
		// it gets freed by the destructor... maybe change this later
		desc = new Operator_Descriptor("CAST", 10, ASSOC_LEFT, OP_UNARY);
	}
	~Expression_Cast() { 
		delete desc; 
	} 

	Datatype_Base* target;
	Expression* operand = nullptr;
};

struct Ast_Node {
	Ast_Node(Ast_Node_Type t): type(t) {}
	virtual ~Ast_Node() {};
	void print(int) const;
	
	int line = 0;
	Ast_Node* parent = nullptr;
	Ast_Node_Type type; 
};

struct Ast_If : public Ast_Node {
	Ast_If(): Ast_Node(AST_IF) {}
	
	Expression* cond = nullptr;
	Ast_Node* child = nullptr;	
};

struct Ast_While : public Ast_Node {
	Ast_While(): Ast_Node(AST_WHILE) {}
	
	Expression* cond = nullptr;
	Ast_Node* child = nullptr;	
};

struct Ast_For : public Ast_Node {
	Ast_For(): Ast_Node(AST_FOR) {}

	Expression* init = nullptr;
	Expression* cond = nullptr;
	Expression* each = nullptr;
	Ast_Node* child = nullptr;
};

struct Ast_Procedure : public Ast_Node {
	Ast_Procedure(): Ast_Node(AST_PROCEDURE) {}
	
	bool implemented = false;	
	std::string identifier;
	Datatype_Procedure* desc;
	Ast_Node* child = nullptr;
};

struct Ast_Block : public Ast_Node {
	Ast_Block(): Ast_Node(AST_BLOCK) {}

	std::vector<Ast_Node*> children;
	std::vector<Var_Declaration*> locals;
};

struct Ast_Statement : public Ast_Node {
	Ast_Statement(): Ast_Node(AST_STATEMENT) {}

	Expression* expression = nullptr;
};

class Parser {
	
	public:
		Parser();
		static Ast_Node* generateSyntaxTree(std::vector<Token>&);

	private:
		int num_tokens;
		int token_index;
		int marked;
		Ast_Block* root;
		Ast_Block* current_node;
		Ast_Block* current_block;
		Ast_Node* append_target = nullptr;
		Token* focus;
		Token* prev_focus;
		std::vector<Token>* tokens;
		std::vector<Datatype_Base*> defined_types;
		std::vector<Ast_Procedure*> defined_procedures;

		void die(const std::string&);
		void focusTokens(std::vector<Token>&);
		void next();
		Token* peek();
		void setIndex(int);
		void handleIf();
		void handleWhile();
		void handleFor();
		void handleBlock();
		void handleCloseBlock();
		void handleStatement();
		void handleDeclaration();
		void markOperator(const std::string&, const std::string&);
		bool isOperator();
		bool isIdentifier();
		bool onOperator(const std::string&);
		bool onIdentifier(const std::string&);
		void eatOperator(const std::string&);
		void eatIdentifier(const std::string&);
		void printOn();
		void appendNode(Ast_Node*);
		Token* atIndex(int index);
		Expression* parseExpression();
		uint32_t parseModifiers();
		bool matchesDeclaration();
		Datatype_Base* parseDatatype();
		Var_Declaration* parseDeclaration();
		Datatype_Base* getTypeInfo(const std::string&);
};

#endif
