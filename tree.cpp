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
	Operator_Descriptor("=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("+=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("-=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("*=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("/=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("%=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("&=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("|=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("^=", 2, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("==", 4, ASSOC_LEFT, OP_BINARY),
	Operator_Descriptor("!=", 4, ASSOC_LEFT, OP_BINARY),
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
	else if (word == "==")
		return BINARY_COMPARE;
	else if (word == "!=")
		return BINARY_COMPARE_NOT;
	else if (word == "=")
		return BINARY_ASSIGNMENT;
}

Expression_Unary_Type
Expression_Unary::wordToUnaryType(const std::string& word) {
	if (word == "!")
		return UNARY_LOGICAL_NOT;
	else if (word == "~")
		return UNARY_BITWISE_NOT;
	else if (word == "(")
		return UNARY_OPEN_PARENTHESIS;
	else if (word == ")")
		return UNARY_CLOSE_PARENTHESIS;
}

void
Expression::print(int ind = 0) const {

	if (!this) {
		return;
	}

	auto indent = [](int amount) -> void {
		for (int i = 0; i < amount; i++) {
			std::cout << "  ";
		}
	};

	// ... is the another way to do this?
	const Expression_Integer*    a = dynamic_cast<const Expression_Integer *>(this);
	const Expression_Float*      b = dynamic_cast<const Expression_Float *>(this);
	const Expression_Identifier* c = dynamic_cast<const Expression_Identifier *>(this);
	const Expression_Binary*     d = dynamic_cast<const Expression_Binary *>(this);
	const Expression_Unary*      e = dynamic_cast<const Expression_Unary *>(this);
	const Expression_Cast*	     f = dynamic_cast<const Expression_Cast *>(this);

	indent(ind);

	if (a) {
		std::cout << a->value << std::endl;
	} else if (b) {
		std::cout << b->value << std::endl;
	} else if (c) {
		std::cout << c->value << std::endl;
	} else if (d) {
		std::cout << word << std::endl;
		d->left->print(ind + 1);
		d->right->print(ind + 1);
	} else if (e) {
		std::cout << word << std::endl;
		e->operand->print(ind + 1);
	} else if (f) {
		std::cout << "CAST" << std::endl;
		indent(ind + 1);
		std::cout << f->target->as_string << std::endl;
		f->operand->print(ind + 1);
	}

}

/* ------ TREE_NODE IMPL ------ */
void
Ast_Node::print(int ind = 0) const {

	if (!this) {
		return;
	}

	auto indent = [](int amount) -> void {
		for (int i = 0; i < amount; i++) {
			std::cout << "  ";
		}
	};
	
	// this is nasty but oh well
	const Ast_If*        a = dynamic_cast<const Ast_If *>(this);
	const Ast_Block*     b = dynamic_cast<const Ast_Block *>(this);
	const Ast_Statement* c = dynamic_cast<const Ast_Statement *>(this);
	const Ast_While*     d = dynamic_cast<const Ast_While *>(this);
	const Ast_For*       e = dynamic_cast<const Ast_For *>(this);
	const Ast_Procedure* f = dynamic_cast<const Ast_Procedure *>(this);

	indent(ind);
	
	if (a) {
		std::cout << "IF: [\n";
		indent(ind + 1);
		std::cout << "CONDITION: [\n";
		a->cond->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind + 1);
		std::cout << "CHILD: [\n";
		a->child->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind);
		std::cout << "]\n";
	} else if (b) {
		std::cout << "BLOCK: [\n";
		indent(ind + 1);
		std::cout << "LOCALS: [\n";
		for (const Var_Declaration* local: b->locals) {
			indent(ind + 2);
			std::cout << local->as_string << std::endl;
		}
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind + 1);
		std::cout << "CHILDREN: [\n";
		for (const Ast_Node* child: b->children) {
			child->print(ind + 2);
		}
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind);
		std::cout << "]\n";
	} else if (c) {
		std::cout << "STATEMENT: [\n";
		c->expression->print(ind + 1);
		indent(ind);
		std::cout << "]\n";
	} else if (d) {
		std::cout << "WHILE: [\n";
		indent(ind + 1);
		std::cout << "CONDITION: [\n";
		d->cond->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind + 1);
		std::cout << "CHILD: [\n";
		d->child->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind);
		std::cout << "]\n";
	} else if (e) {
		std::cout << "FOR: [\n";
		indent(ind + 1);
		std::cout << "INITIALIZER: [\n";
		e->init->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind + 1);
		std::cout << "CONDITION: [\n";
		e->cond->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind + 1);
		std::cout << "EACH: [\n";
		e->each->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind + 1);
		std::cout << "CHILD: [\n";
		e->child->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind);
		std::cout << "]\n";
	} else if (f) {
		std::cout << "PROCEDURE: [\n";
		indent(ind + 1);
		std::cout << "IDENTIFIER: " << f->identifier << std::endl;
		indent(ind + 1);
		std::cout << "ARGUMENTS: [\n";
		for (const Var_Declaration* arg: f->args) {
			indent(ind + 2);
			std::cout << arg->as_string << std::endl;
		}
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind + 1);
		std::cout << "CHILD: [\n";
		f->child->print(ind + 2);
		indent(ind + 1);
		std::cout << "]\n";
		indent(ind);
		std::cout << "]\n"; 	
	}

}

/* ------ DATATYPE IMPL ------ */

std::string
Datatype_Base::toString(const Datatype_Base& d) {
	std::string ret = "";		

	if (d.mods & MOD_STATIC) {
		ret += "static ";
	}
	if (d.mods & MOD_CONST) {
		ret += "const ";
	}
	if (d.mods & MOD_FOREIGN) {
		ret += "foreign ";
	}
	
	for (int i = 0; i < d.size_ptr; i++) {
		ret += "^";
	}

	for (int i = 0; i < d.size_array.size(); i++) {
		ret += "[]";
	}

	switch (d.type) {
		case DATA_PROCEDURE: {
			const Datatype_Procedure& proc = dynamic_cast<const Datatype_Procedure&>(d);
			size_t size = proc.args.size();
			ret += "(";
			for (int i = 0; i < size; i++) {
				ret += proc.args[i]->dt->as_string;
				if (i < size - 1) {
					ret += ", ";
				}	
			}
			ret += ") -> ";
			ret += proc.ret->as_string;
			break;
		}
		default:
			ret += d.type_name;
	}

	return ret;
}


/* ------ PARSER IMPL ------ */

Parser::Parser() {
	num_tokens = 0; // TBI
	token_index = 0;
	marked = 0;
	current_block = new Ast_Block();
	root = current_block;

	/* create default types */
	Datatype_Base* t_int = new Datatype_Integer();
	defined_types.push_back(t_int);

	Datatype_Base* t_float = new Datatype_Float();
	defined_types.push_back(t_float);

	Datatype_Base* t_byte = new Datatype_Byte();
	defined_types.push_back(t_byte);
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

Token*
Parser::peek() {
	if (!focus) {
		return nullptr;
	}
	return &((*tokens)[token_index + 1]);
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
Parser::isOperator() {
	return focus->type == TOK_OPERATOR;
}

bool 
Parser::isIdentifier() {
	return focus->type == TOK_IDENTIFIER;
}

bool 
Parser::onOperator(const std::string& word) {
	return isOperator() && focus->word == word;
}

bool
Parser::onIdentifier(const std::string& identifier) {
	return isIdentifier() && focus->id == identifier;
}

void 
Parser::eatOperator(const std::string& word) {
	if (!onOperator(word)) {
		die("expected operator (" + word + ")");
	}
	next();
}

void
Parser::eatIdentifier(const std::string& expected) {
	if (!onIdentifier(expected)) {
		die("expected identifier '" + expected + "'");
	}
	next();
}

void
Parser::printOn() {
	std::cout << "ON: (word: " << focus->word << ", line: " << focus->line << ")\n";
}

void
Parser::markOperator(const std::string& inc, const std::string& dec) {
	int start = token_index;
	int count = 1;
	while (true) {
		if (onOperator(inc)) {
			count++;
		} else if (onOperator(dec)) {
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

Datatype_Base*
Parser::getTypeInfo(const std::string& type_name) {
	for (Datatype_Base* d: defined_types) {
		if (d->type_name == type_name) {
			return d;
		}
	}
	return nullptr;
}

uint32_t
Parser::parseModifiers() {
	
	static const std::vector<std::string> modifiers {
		"static",
		"const",
		"foreign"
	};

	uint32_t mod = 0;

	while (isIdentifier()) {
		bool passed = false;
		for (int i = 0; i < modifiers.size(); i++) {
			if (onIdentifier(modifiers[i])) {
				passed = true;
				mod |= (0x1 << i);
				break;
			}
		}
		if (!passed) {
			break;
		}
		next();
	}

	return mod;
}

Datatype_Base*
Parser::parseDatatype() {

	Datatype_Base* ret;
	Datatype_Base* type_info;
	uint32_t mod;
	int ptr_dim = 0;
	int arr_dim = 0;
	
	mod = parseModifiers();	
	
	if (onIdentifier("int")) {
		ret = new Datatype_Integer();
	} else if (onIdentifier("float")) {
		ret = new Datatype_Float();
	} else if (onIdentifier("byte")) {
		ret = new Datatype_Byte();	
	} else if (onOperator("(")) {
		Datatype_Procedure* proc = new Datatype_Procedure();
		eatOperator("(");
		while (matchesDeclaration()) {
			Var_Declaration* arg = parseDeclaration();
			proc->args.push_back(arg);
			if (focus->word == ",") {
				next();
			}
		}
		if (!onOperator(")")) {
			die("expected ')' to close argument list, got token '" + focus->word + "'");
		}
		eatOperator(")");
		eatOperator("->");
		proc->ret = parseDatatype();
		ret = proc;
	} else {
		// check if its a defined struct
		Datatype_Base* found_type = nullptr;
		for (Datatype_Base* d: defined_types) {
			if (onIdentifier(d->type_name)) {
				found_type = d;
				break;	
			}
		}
		if (!found_type) {
			die("unknown typename '" + focus->word + "'");
		}
	}
	
	ret->mods = mod;
	ret->is_ptr = ptr_dim > 0;
	ret->size_ptr = ptr_dim;
	ret->is_array = arr_dim > 0;
	ret->as_string = Datatype_Base::toString(*ret);

	next();

	return ret;

}

bool
Parser::matchesDeclaration() {
	Token* p = peek();
	if (!p) {
		return false;
	}
	return isIdentifier() && p->word == ":"; 
}

Var_Declaration*
Parser::parseDeclaration() {
	
	Var_Declaration* decl = new Var_Declaration();	
	decl->identifier = focus->word;
	
	next(); // skip id
	eatOperator(":");

	decl->dt = parseDatatype();
	decl->as_string = decl->identifier + ": " + decl->dt->as_string;

	return decl;


}

Expression*
Parser::parseExpression() {

	std::vector<Token> raw;
	std::vector<Expression_Operator*> ops;
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
	for (int i = token_index; i < marked; i++) {
		Token& tok = *atIndex(i);
		switch (tok.type) {
			case TOK_INTEGER: {
				Expression_Integer* push = new Expression_Integer();
				push->value = tok.i;
				push->word = tok.word;
				postfix.push_back(push);
				break;
			}
			case TOK_FLOAT: {
				Expression_Float* push = new Expression_Float();
				push->value = tok.f;
				push->word = tok.word;
				postfix.push_back(push);
				break;
			}
			case TOK_STRING: {
				Expression_String* push = new Expression_String();
				push->value = tok.word;
				push->word = tok.word;
				postfix.push_back(push);
				break;
			}
			case TOK_IDENTIFIER: {
				Expression_Identifier* push = new Expression_Identifier();
				push->value = tok.word;
				push->word = tok.word;
				postfix.push_back(push);
				break;
			}
			case TOK_OPERATOR: {
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
						Expression_Operator* push;
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
							op->optype = Expression_Unary::wordToUnaryType(tok.word);
							push = op;
						}
						push->desc = desc;
						push->word = tok.word;
						Expression_Operator* back;
						while (true) {
							if (ops.size() == 0) {
								break;
							}
							back = ops.back();
							if (desc->assoc == ASSOC_LEFT) {
								if (desc->prec > back->desc->prec) break;
							} else {
								if (desc->prec >= back->desc->prec) break;
							}
							postfix.push_back(back);
							ops.pop_back();
						}
						ops.push_back(push);
						break;
					}
				}
				break;
			}
		}
	}
	while (ops.size() > 0) {
		postfix.push_back(ops.back());
		ops.pop_back();
	}	

	std::vector<Expression*> last;

	for (Expression* e: postfix) {
		switch (e->type) {
			case EXP_INTEGER:
			case EXP_FLOAT:
			case EXP_IDENTIFIER:
				last.push_back(e);
				break;	
			case EXP_BINARY: {
				Expression* operands[2];
				Expression_Binary* binary_op = dynamic_cast<Expression_Binary *>(e);
				for (int i = 0; i < 2; i++) {
					if (last.size() == 0) {
						// TODO make a better error message
						die("malformed expression");
					}
					Expression* op = last.back();
					op->parent = e;
					last.pop_back();
					operands[i] = op;
				}
				operands[0]->side = LEAF_LEFT;
				operands[1]->side = LEAF_RIGHT;
				binary_op->left = operands[0];
				binary_op->right = operands[1];
				last.push_back(e);
				break;
			}
			case EXP_UNARY: {
				Expression_Unary* unary_op = dynamic_cast<Expression_Unary *>(e);
				Expression* operand = last.back();
				last.pop_back();
				unary_op->operand = operand;
				operand->parent = e;
				last.push_back(e);
				break;
			}
		}
	}
	
	if (last.size() != 1) {
		die("an expression must have exactly one result");
	}

	setIndex(marked);

	return last.back();
}

void
Parser::handleIf() {
	Ast_If* node = new Ast_If();
	eatIdentifier("if");
	eatOperator("(");
	markOperator("(", ")");
	node->cond = parseExpression();
	eatOperator(")");
	appendNode(node);
}

void
Parser::handleWhile() {
	Ast_While* node = new Ast_While();
	eatIdentifier("while");
	eatOperator("(");
	markOperator("(", ")");
	node->cond = parseExpression();
	eatOperator(")");
	appendNode(node);
}

void
Parser::handleFor() {
	Ast_For* node = new Ast_For();
	eatIdentifier("for");
	eatOperator("(");
	markOperator("", ";");
	node->init = parseExpression();
	eatOperator(";");
	markOperator("", ";");
	node->cond = parseExpression();
	eatOperator(";");
	markOperator("(", ")");
	node->each = parseExpression();
	eatOperator(")");
	appendNode(node);
}

void
Parser::handleBlock() {
	Ast_Block* node = new Ast_Block();
	eatOperator("{");
	appendNode(node);
	current_block = node;
}

void
Parser::handleCloseBlock() {
	Ast_Node* parent = current_block->parent;
	while (parent && !dynamic_cast<Ast_Block *>(parent)) {
		parent = parent->parent;
	}
	current_block = dynamic_cast<Ast_Block *>(parent);
	if (!current_block) {
		die("unexpected closing bracket '}'");
	}
	next();
}

void
Parser::handleStatement() {
	Ast_Statement* node = new Ast_Statement();
	markOperator("", ";");
	node->expression = parseExpression();
	appendNode(node);
	eatOperator(";");
}

void
Parser::handleDeclaration() {
	
	Var_Declaration* decl = parseDeclaration();
	Datatype_Base* dt = decl->dt;
		
	// if its a procedure followed by {, it's an implementation */
	if (dt->type == DATA_PROCEDURE && onOperator("{")) {
		Datatype_Procedure* proc_desc = dynamic_cast<Datatype_Procedure *>(dt);

		for (const Var_Declaration* arg: proc_desc->args) {
			if (!arg->named) {
				die("all arguments must be named in a procedure implementation");
			}
		}

		/* wrap the procedure in a node */
		Ast_Procedure* node = new Ast_Procedure();
		node->implemented = true;
		node->identifier = decl->identifier;
		node->desc = proc_desc;
		
		for (const Ast_Procedure* p: defined_procedures) {
			if (p->identifier == node->identifier) {
				die("re-implementation of procedure '" + p->identifier + "'");
			}	
		}

		defined_procedures.push_back(node);
		appendNode(node);

	}

}

void
Parser::appendNode(Ast_Node* node) {
	if (append_target) {
		switch (append_target->type) {
			case AST_IF:
				dynamic_cast<Ast_If *>(append_target)->child = node;
				break;	
			case AST_WHILE:
				dynamic_cast<Ast_While *>(append_target)->child = node;
				break;	
			case AST_FOR:
				dynamic_cast<Ast_For *>(append_target)->child = node;
				break;
			case AST_PROCEDURE:
				dynamic_cast<Ast_Procedure *>(append_target)->child = node;
				break;
		}
		node->parent = append_target;
	} else {
		current_block->children.push_back(node);
		node->parent = current_block;
	}
	switch (node->type) {
		case AST_IF:
		case AST_WHILE:
		case AST_FOR:
		case AST_PROCEDURE:
			append_target = node;
			break;	
		default:
			append_target = nullptr;
	}
	if (node->type == AST_BLOCK) {
		current_block = dynamic_cast<Ast_Block *>(node);
	}
}

Ast_Node*
Parser::generateSyntaxTree(std::vector<Token>& tokens) {

	Parser parse;
	parse.focusTokens(tokens);

	Token* p;
	
	while (parse.focus) {
		p = parse.peek();
		std::string& word = parse.focus->word;
		if (word == "if") {
			parse.handleIf();	
		} else if (word == "while") {
			parse.handleWhile();
		} else if (word == "for") {
			parse.handleFor();
		} else if (word == "{") {
			parse.handleBlock();
		} else if (word == "}") {
			parse.handleCloseBlock();
		} else if (parse.matchesDeclaration()) {
			parse.handleDeclaration();
		} else {
			parse.handleStatement();
		}	
	}
			
	parse.root->print();

	return parse.root;

}
