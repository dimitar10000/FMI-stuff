#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
#include "lexer.h"
#include "parser.h"
#include "utility_funcs.h"

int main() {
	std::string file_name;
	std::cin >> file_name;
	
	std::ifstream reader(file_name);
	
	std::string word;
	std::vector<std::string> file_content;	

	while (std::getline(reader,word)) {
		file_content.push_back(word);
	}

	std::string content_as_str = vec_to_str(file_content);

	Lexer lexer(content_as_str);
	lexer.produce_tokens();

	lexer.print_tokens();

	Parser parser(lexer.tokens);

	AST* tree = new AST("Program");

	parser.parse_code(tree);
	tree->print();

	return 0;
}