#include <stdlib.h>
#include <iostream>
#include "tree.h"

TreeNode::TreeNode(TreeNodeType type) {
	this->type = type;	
}

TreeNode::~TreeNode() {

}

Parser::Parser() {
	num_tokens = 0; // TBI
	token_index = 0;
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
}

void
Parser::peek() {
	boundsCheck();
	prev_focus = focus;
	focus = &((*tokens)[token_index]);
}

void
Parser::next() {
	boundsCheck();
	prev_focus = focus;
	focus = &((*tokens)[token_index++]);
}

void
Parser::boundsCheck() {
	if (token_index >= tokens->size()) {
		die("unexpected EOF");
	}
}

TreeNode*
Parser::generateSyntaxTree(std::vector<Token>& tokens) {

	Parser parse;
	parse.focusTokens(tokens);
	parse.focus = &((*parse.tokens)[0]);
	parse.prev_focus = &((*parse.tokens)tokens[0]);
	
	while (parse.next()) {
		switch (parse.focus->type) {
			default:
				std::cout << "unknown token " << parse.focus->word << std::endl;
				break;
		}
	}
			

	return parse.root;

}
