#include <iostream>
#include "lex.h"
#include "tree.h"

int main() {

	std::vector<Token> tokens = Lexer::generateTokens("demo.spy");
	Tree_Node* tree = Parser::generateSyntaxTree(tokens);

}
