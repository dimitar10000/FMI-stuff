#pragma once
#include <string>
#include <vector>
#include "token.h"
#include <variant>

class AST {
	std::variant<std::string, Token> node_type;
	unsigned indentation;

	std::vector<AST*> children;

public:

	std::variant< std::string, Token> get_root();

	std::vector<AST*> get_children();

	AST(const AST& other);

	AST(std::string str);
	AST(Token token);

	void add_children(std::vector<AST*>);

	void print();
};
