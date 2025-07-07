#pragma once
#include <string>
#include <vector>
#include "token.h"
#include "ast-builder.h"

class Parser {
	std::vector<Token> tokens;
	unsigned current_token;

public:

	Parser(std::vector<Token> tokens_array);

	bool match_type(Token token,TokenType type);
	bool match_one_of(Token token, std::vector<TokenType> types);

	bool finished_parsing();

	Token peek();
	Token next_token();
	Token prev_token();

	void parse_code(AST* tree);

	AST* parse_include_expr();
	AST* parse_library_expr();
	AST* parse_header_expr();

	AST* parse_using_expr();

	AST* parse_decl_assignment_expr();
	AST* parse_simple_assignment_expr();
	AST* parse_var_declaration_expr();

	AST* parse_func_definition_expr();
	std::vector<AST*> parse_func_args_expr();
	AST* parse_func_call_expr();

	AST* parse_arithmetic_expr();
	AST* build_arithmetic_tree(std::vector<Token> tokens);

	AST* parse_boolean_expr(std::vector<AST*>& parentheses);
	AST* chain_negations();
	AST* get_last_node(AST* tree);

	AST* parse_logical_expr();
	AST* build_logical_tree(std::vector<AST*> trees);

	AST* parse_string_expr();
	AST* build_string_tree(std::vector<Token> tokens);

	AST* parse_input_expr();
	AST* parse_output_expr();
	AST* build_io_tree(std::vector<AST*> trees);

	AST* parse_for_expr();
	AST* parse_incr_decr_expr();

	AST* parse_while_expr();
	AST* parse_if_else_expr();
	AST* parse_else_if();

	AST* parse_return_expr();

	AST* parse_class_definition_expr();
	AST* parse_class_body();
	AST* parse_access_specifier_expr();
	AST* parse_class_constructor_expr();
	AST* parse_class_destructor_expr();
	AST* parse_delete_expr();

	AST* parse_line_comment();
	AST* parse_multiline_comment();

	AST* parse_braces_body(const std::string& s);

};
