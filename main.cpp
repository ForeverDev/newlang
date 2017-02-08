#include <iostream>
#include "lex.h"
#include "tree.h"

int main() {

	std::vector<Token> tokens = Lexer::generateTokens("demo.spy");
	TreeNode* tree = Parser::generateSyntaxTree(tokens);

}
