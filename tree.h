#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include "lex.h"

enum TreeNodeType {
	NODE_IF,
	NODE_WHILE,
	NODE_FOR,
	NODE_BLOCK,
	NODE_STATEMENT,
	NODE_DECL
};

class TreeNode {
	
	public:
		TreeNode(TreeNodeType);
		~TreeNode();
	
		int line = 0;
		TreeNodeType type;

	private:

};

class Parser {
	
	public:
		Parser();
		static TreeNode* generateSyntaxTree(std::vector<Token>&);
		void next();
		void peek();
		void focusTokens(std::vector<Token>&);

	private:
		TreeNode* root;
		Token* focus;
		Token* prev_focus;
		std::vector<Token>* tokens;
		int num_tokens;
		int token_index;

		void boundsCheck();
		void die(const std::string&);
};

#endif
