/* the lexer converts a spy file into a vector of tokens */

#ifndef LEX_H
#define LEX_H

#include <string>
#include <vector>
#include <fstream>

enum TokenType {
	TOK_NOTOK,
	TOK_INT,
	TOK_FLOAT,
	TOK_STRING,
	TOK_ID,
	TOK_OP
};

class Token {
	
	public:
		Token();
		~Token();
		std::string toString();

		TokenType type;
		int line;

		long long i; /* int, operator */
		double f;
		std::string id; /* identifier, string */
		std::string word; 
};

class Lexer {

	public:
		static std::vector<Token> generateTokens(const std::string&);

	private:
		Lexer(const std::string&);
		~Lexer();
		char get();
		char peek();
		void eat();
		void eat(char);
		void dispatchFromChar(char);
		void handleNumber();
		void handleIdentifier();
		void handleString();
		void handleOperator();
		void die(const std::string&);
		void printTokens();

		std::vector<Token> tokens;
		std::ifstream handle;
		int line;


};

#endif
