#include <stdlib.h>
#include <iostream>
#include "tree.h"

/*
static const OpEntry prec[127] = {
	[',']				= {1, ASSOC_LEFT, OP_BINARY},
	['=']				= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_INC_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_DEC_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_MUL_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_DIV_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_MOD_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_SHL_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_SHR_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_AND_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_OR_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_XOR_BY]		= {2, ASSOC_RIGHT, OP_BINARY},
	[SPEC_LOG_AND]		= {3, ASSOC_LEFT, OP_BINARY},
	[SPEC_LOG_OR]		= {3, ASSOC_LEFT, OP_BINARY},
	[SPEC_EQ]			= {4, ASSOC_LEFT, OP_BINARY},
	[SPEC_NEQ]			= {4, ASSOC_LEFT, OP_BINARY},
	['>']				= {6, ASSOC_LEFT, OP_BINARY},
	[SPEC_GE]			= {6, ASSOC_LEFT, OP_BINARY},
	['<']				= {6, ASSOC_LEFT, OP_BINARY},
	[SPEC_LE]			= {6, ASSOC_LEFT, OP_BINARY},
	['|']				= {7, ASSOC_LEFT, OP_BINARY},
	[SPEC_SHL]			= {7, ASSOC_LEFT, OP_BINARY},
	[SPEC_SHR]			= {7, ASSOC_LEFT, OP_BINARY},
	['+']				= {8, ASSOC_LEFT, OP_BINARY},
	['-']				= {8, ASSOC_LEFT, OP_BINARY},
	['*']				= {9, ASSOC_LEFT, OP_BINARY},
	['%']				= {9, ASSOC_LEFT, OP_BINARY},
	['/']				= {9, ASSOC_LEFT, OP_BINARY},
	['@']				= {10, ASSOC_RIGHT, OP_UNARY},
	['$']				= {10, ASSOC_RIGHT, OP_UNARY},
	['!']				= {10, ASSOC_RIGHT, OP_UNARY},
	[SPEC_TYPENAME]		= {10, ASSOC_RIGHT, OP_UNARY},
	[SPEC_CAST]			= {10, ASSOC_RIGHT, OP_UNARY},
	['.']				= {11, ASSOC_LEFT, OP_BINARY},
	[SPEC_INC_ONE]		= {11, ASSOC_LEFT, OP_UNARY},
	[SPEC_DEC_ONE]		= {11, ASSOC_LEFT, OP_UNARY},
	[SPEC_CALL]			= {11, ASSOC_LEFT, OP_UNARY},
	[SPEC_INDEX]		= {11, ASSOC_LEFT, OP_UNARY}
};
*/

static const std::vector<Operator_Descriptor> operator_table {
	Operator_Descriptor(">",  6, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor(">=", 6, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("<",  6, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("<=", 6, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("+",  8, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("-",  8, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("*",  9, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("/",  9, ASSOC_LEFT, OP_BINARY),
};

static const Operator_Descriptor*
get_operator_descriptor(const std::string& word) {
	for (const Operator_Descriptor& op: operator_table) {
		if (op.str == word) {
			return &op;
		}
	}
	return nullptr;
}

/* ------ EXPRESSION IMPL ------ */
bool
Expression::isBinaryType(Expression_Binary_Type type) const {
	const Expression_Binary* b = dynamic_cast<const Expression_Binary *>(this);
	if (!b) {
		return false;
	}
	return b->optype == type;
}

bool
Expression::isUnaryType(Expression_Unary_Type type) const {
	const Expression_Unary* u = dynamic_cast<const Expression_Unary *>(this);
	if (!u) {
		return false;
	}
	return u->optype == type;
}

Expression_Binary_Type
Expression_Binary::wordToBinaryType(const std::string& word) {
	if (word == "+")
		return BINARY_ADDITION;
	else if (word == "-")
		return BINARY_SUBTRACTION;
	else if (word == "*")
		return BINARY_MULTIPLICATION;
	else if (word == "/")
		return BINARY_DIVISION;
	else if (word == "&")
		return BINARY_MODULUS;
	else if (word == "&")
		return BINARY_BITWISE_AND;
	else if (word == "<<")
		return BINARY_SHIFT_LEFT;
	else if (word == ">>")
		return BINARY_SHIFT_RIGHT;
	else if (word == "|")
		return BINARY_BITWISE_OR;
	else if (word == "^")
		return BINARY_BITWISE_XOR;
	else if (word == "&&")
		return BINARY_LOGICAL_AND;
	else if (word == "||")
		return BINARY_LOGICAL_OR;
	else if (word == "+=")
		return BINARY_INCREMENT_BY;
	else if (word == "-=")
		return BINARY_DECREMENT_BY;
	else if (word == "*=")
		return BINARY_MULTIPLY_BY;
	else if (word == "/=")
		return BINARY_DIVIDE_BY;
	else if (word == "%=")
		return BINARY_MODULUS_BY;
	else if (word == "&=")
		return BINARY_AND_BY;
	else if (word == "|=")
		return BINARY_OR_BY;
	else if (word == "^=")
		return BINARY_XOR_BY;
	else if (word == "<<=")
		return BINARY_SHIFT_LEFT_BY;
	else if (word == ">>=")
		return BINARY_SHIFT_RIGHT_BY;
}

Expression_Unary_Type
Expression_Unary::wordToUnaryType(const std::string& word) {

}

/* ------ TREE_NODE IMPL ------ */

Tree_Node::Tree_Node(Tree_Node_Type type) {
	this->type = type;	
}

Tree_Node::~Tree_Node() {

}

/* ------ PARSER IMPL ------ */

Parser::Parser() {
	num_tokens = 0; // TBI
	token_index = 0;
	marked = 0;
	root = new Tree_Node(NODE_BLOCK);
}

void
Parser::die(const std::string& message) {
	Token* f = focus ?: prev_focus;
	std::cout << "!!! PARSE ERROR !!!\n";
	std::cout << "\tmessage: " << message << std::endl;
	std::cout << "\tline: " << (f ? f->line : 0) << std::endl;
	exit(1);
}

void 
Parser::focusTokens(std::vector<Token>& toks) {
	tokens = &toks;
	num_tokens = toks.size();
	token_index = 0;	
	focus = &toks[0];
	prev_focus = &toks[0];
}

void
Parser::peek() {
	if (!focus) {
		die("unexpected EOF");
	}
	focus = &((*tokens)[token_index + 1]);
}

void
Parser::next() {
	if (!focus) {
		die("unexpected EOF");
	}
	prev_focus = focus;
	token_index++;
	if (token_index >= num_tokens) {
		focus = nullptr;
	} else {
		focus = &((*tokens)[token_index]);
	}
}

void
Parser::setIndex(int index) {
	token_index = index;
	if (token_index >= num_tokens) {
		focus = nullptr;
	} else {
		focus = &((*tokens)[token_index]);
	}
}

Token*
Parser::atIndex(int index) {
	return &((*tokens)[index]);
}

bool 
Parser::isOperator(int op_type) {
	return focus->type == TOK_OP && focus->i == op_type;
}

bool
Parser::isIdentifier(const std::string& identifier) {
	return focus->type == TOK_ID && focus->id == identifier;
}

void 
Parser::eatOperator(int op_type) {
	if (!isOperator(op_type)) {
		die("expected operator (" + std::to_string(op_type) + ")");
	}
	next();
}

void
Parser::eatIdentifier(const std::string& expected) {
	if (!isIdentifier(expected)) {
		die("expected identifier '" + expected + "'");
	}
	next();
}

void
Parser::printOn() {
	std::cout << "ON: (word: " << focus->word << ", line: " << focus->line << ")\n";
}

void
Parser::markOperator(int inc, int dec) {
	int start = token_index;
	int count = 1;
	while (true) {
		if (isOperator(inc)) {
			count++;
		} else if (isOperator(dec)) {
			count--;
		} 
		if (count == 0) {
			marked = token_index;
			setIndex(start);
			return;
		}
		next();	
	}
}

Expression*
Parser::parseExpression() {

	std::vector<Token> raw;
	std::vector<Expression*> ops;
	std::vector<Expression*> postfix;
	Expression* exp;

	if (token_index == marked) {
		return nullptr;
	}
	
	// first, gather the raw expression
	for (int i = token_index; i < marked; i++) {
		raw.push_back(*atIndex(i));
	}

	// now scan the expression and use shunting yard
	for (Token& tok: raw) {
		switch (tok.type) {
			case TOK_INT:
			case TOK_FLOAT:
			case TOK_STRING:
			case TOK_ID:
				break;
			case TOK_OP: {
				switch (tok.i) {
					case '(': { 
						Expression_Unary* push = new Expression_Unary();
						push->optype = UNARY_OPEN_PARENTHESIS;
						push->word = "(";
						ops.push_back(push);
						break;
					}
					case ')': {
						Expression* back;
						while (true) {
							if (ops.size() == 0) {
								die("unexpected closing parenthesis");
							}
							back = ops.back();
							if (back->isUnaryType(UNARY_OPEN_PARENTHESIS)) {
								break;
							}
							postfix.push_back(back);
							ops.pop_back();
						}
						ops.pop_back();
						break;
					}
					default: {
						const Operator_Descriptor* desc = get_operator_descriptor(tok.word);
						Expression* push;
						if (!desc) {
							std::string err = "unknown operator '";
							err += tok.word;
							err += "'";
							die(err);
						}
						if (desc->operand_type == OP_BINARY) {
							Expression_Binary* op = new Expression_Binary();
							op->optype = Expression_Binary::wordToBinaryType(tok.word);
							push = op;
						} else if (desc->operand_type == OP_UNARY) {
							Expression_Unary* op = new Expression_Unary();
							op->optype = Expression_Unary::wordToBinaryType(tok.word);
							push = op;
						}
						break;
					}
				}
				break;
			}
		}
	}

	return exp;
}

void
Parser::handleIf() {
	next(); // skip IF 
	eatOperator('(');
	markOperator('(', ')');
	Expression* exp = parseExpression();
}

Tree_Node*
Parser::generateSyntaxTree(std::vector<Token>& tokens) {

	Parser parse;
	parse.focusTokens(tokens);
	
	while (parse.focus) {
		switch (parse.focus->type) {
			case TOK_ID:
				if (parse.focus->id == "if") {
					parse.handleIf();	
				}
				break;
			case TOK_INT:
			case TOK_FLOAT:
			case TOK_STRING:
			case TOK_OP:
				break;	
			default:
				parse.next();
				break;
		}
		parse.next();
	}
			

	return parse.root;

}
