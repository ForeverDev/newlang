#include <iostream>
#include "lex.h"
#include "tree.h"

int main() {

	std::vector<Token> tokens = Lexer::generateTokens("demo.spy");
	Ast_Node* tree = Parse_Context::generateSyntaxTree(tokens);

}
