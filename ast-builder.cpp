#include "ast-builder.h"
#include <iostream>

AST::AST(std::string str) {
	node_type = str;
	indentation = 0;
}

AST::AST(Token token) {
	node_type = token;
	indentation = 0;
}

AST::AST(const AST& other) {
	node_type = other.node_type;
	children = other.children;
}

std::variant< std::string, Token> AST::get_root() {
	return node_type;
}

std::vector<AST*> AST::get_children() {
	return children;
}

void AST::add_children(std::vector<AST*> nodes) {
	children.insert(children.end(),nodes.begin(),nodes.end());
}

void AST::print() {
	for (unsigned i = 0; i < indentation; i++) {
		std::cout << '\t';
	}

	if (node_type.index() == 0) {
		std::cout << std::get<std::string>(node_type) << '\n';
	}
	else {
		std::cout << std::get<Token>(node_type) << '\n';
	}

	for (int i = 0; i < children.size(); i++) {
		children[i]->indentation = indentation + 1;
		children[i]->print();
	}
}