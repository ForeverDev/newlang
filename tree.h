#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <stdint.h>
#include "lex.h"

#define MOD_STATIC  (0x1 << 0)
#define MOD_CONST   (0x1 << 1)
#define MOD_FOREIGN (0x1 << 2)

class Parser;

enum Ast_Node_Type {
	AST_IF,
	AST_WHILE,
	AST_FOR,
	AST_BLOCK,
	AST_STATEMENT,
	AST_RETURN,
	AST_DECLARATION,
	AST_PROCEDURE,
};

enum Datatype_Type {
	DATA_INTEGER,
	DATA_FLOAT,
	DATA_BYTE,
	DATA_BOOL,
	DATA_VOID,
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
	BINARY_GREATER_THAN,
	BINARY_LESS_THAN,
	BINARY_GREATER_THAN_OR_EQUAL,
	BINARY_LESS_THAN_OR_EQUAL,
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
	BINARY_ASSIGNMENT,
	BINARY_MEMBER_DEREFERENCE
};

enum Expression_Unary_Type {
	UNARY_BITWISE_NOT,
	UNARY_LOGICAL_NOT,
	UNARY_OPEN_PARENTHESIS,
	UNARY_CLOSE_PARENTHESIS,
	UNARY_DEREFERENCE,
	UNARY_ADDRESS_OF
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
	bool matches(const Datatype_Base&) const;
	
	Datatype_Type type;
	int size = 0;
	uint32_t mods = 0;
	bool is_ptr = false;
	int ptr_dim = 0;
	bool is_array = false;
	std::vector<int> size_array;
	std::string as_string;
};

struct Var_Declaration {
	static std::string toString(const Var_Declaration&);

	Datatype_Base* dt;
	std::string identifier;	
	bool named = true; // for unnamed function args
	int offset; // from bp or base of struct
	std::string as_string;
};

struct Datatype_Procedure : public Datatype_Base {
	Datatype_Procedure(): Datatype_Base(DATA_PROCEDURE) {
		size = 8;
	}
	
	Datatype_Base* ret;
	std::vector<Var_Declaration*> args;
};

struct Datatype_Struct : public Datatype_Base {
	Datatype_Struct(): Datatype_Base(DATA_STRUCT) {}
	std::string tostr() const;

	std::vector<Var_Declaration*> members;
};

struct Datatype_Integer : public Datatype_Base {
	Datatype_Integer(): Datatype_Base(DATA_INTEGER) {
		size = 8;
	}
};

struct Datatype_Float : public Datatype_Base {
	Datatype_Float(): Datatype_Base(DATA_FLOAT) {
		size = 8;
	}
};

struct Datatype_Byte : public Datatype_Base {
	Datatype_Byte(): Datatype_Base(DATA_BYTE) {
		size = 1;
	}
};

struct Datatype_Bool : public Datatype_Base {
	Datatype_Bool(): Datatype_Base(DATA_BOOL) {
		size = 8;
	}
};

struct Datatype_Void : public Datatype_Base {
	Datatype_Void(): Datatype_Base(DATA_VOID) {
		size = 0;
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
	Datatype_Base* eval = nullptr;

	Expression(Expression_Type t): type(t) {}
	virtual ~Expression() {}
	virtual Datatype_Base* typecheck(Parser*) {};
	void print(int) const;
	bool isBinaryType(Expression_Binary_Type) const;
	bool isUnaryType(Expression_Unary_Type) const;
	bool isAssignment() const;
};

struct Expression_Integer : public Expression {
	Expression_Integer(): Expression(EXP_INTEGER) {}
	Datatype_Base* typecheck(Parser*);

	int64_t value;
};

struct Expression_Float : public Expression {
	Expression_Float(): Expression(EXP_FLOAT) {}
	Datatype_Base* typecheck(Parser*);

	double value;
};	

struct Expression_Identifier : public Expression {
	Expression_Identifier(): Expression(EXP_IDENTIFIER) {}
	Datatype_Base* typecheck(Parser*);

	std::string value;
	Var_Declaration* var = nullptr;
};

struct Expression_String : public Expression {
	Expression_String(): Expression(EXP_STRING) {}
	Datatype_Base* typecheck(Parser*);

	std::string value;
};

struct Expression_Operator : public Expression {
	Expression_Operator(Expression_Type t): Expression(t) {}
	virtual Datatype_Base* typecheck(Parser*) {};

	const Operator_Descriptor* desc = nullptr;	
};

struct Expression_Binary : public Expression_Operator {
	Expression_Binary(): Expression_Operator(EXP_BINARY) {}
	static Expression_Binary_Type wordToBinaryType(const std::string&);
	Datatype_Base* typecheck(Parser*);
	void assertMatch(Parser*);
	
	Expression_Binary_Type optype;	
	Expression* left = nullptr;
	Expression* right = nullptr;
};

struct Expression_Unary : public Expression_Operator {
	Expression_Unary(): Expression_Operator(EXP_UNARY) {}
	static Expression_Unary_Type wordToUnaryType(const std::string&);
	Datatype_Base* typecheck(Parser*);
	
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
	Datatype_Base* typecheck(Parser*);

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

struct Ast_Return : public Ast_Node {
	Ast_Return(): Ast_Node(AST_RETURN) {}

	Expression* expression = nullptr;
};

class Parser {
	
	public:
		int num_tokens;
		int token_index;
		int marked;
		Ast_Block* root;
		Ast_Block* current_node;
		Ast_Block* current_block;
		Ast_Procedure* current_procedure = nullptr;
		Ast_Node* append_target = nullptr;
		Token* focus;
		Token* prev_focus;
		std::vector<Token>* tokens;
		std::vector<Var_Declaration*> defined_types;
		std::vector<Ast_Procedure*> defined_procedures;
		Datatype_Base* type_int;
		Datatype_Base* type_float;
		Datatype_Base* type_byte;
		Datatype_Base* type_string;
		Datatype_Base* type_bool;
		Datatype_Base* type_void;

		Parser();
		static Ast_Node* generateSyntaxTree(std::vector<Token>&);
		void handleIf();
		void handleWhile();
		void handleFor();
		void handleBlock();
		void handleCloseBlock();
		void handleStatement();
		void handleDeclaration();
		void handleReturn();
		void handleStruct();
		void handleDefine();
		void registerLocal(Var_Declaration*);
		void die(const std::string&);
		void undeclaredIdentifier(const std::string&);
		void focusTokens(std::vector<Token>&);
		void setIndex(int);
		void next();
		Token* peek();
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
		Expression* parseAndTypecheckExpression();
		uint32_t parseModifiers();
		bool matchesDeclaration();
		Datatype_Base* parseDatatype();
		Var_Declaration* parseDeclaration();
		Datatype_Base* getTypeInfo(const std::string&);
		Var_Declaration* getLocal(const std::string&);
};

#endif
