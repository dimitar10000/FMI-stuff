#pragma once
#include <string>
#include <vector>
#include "token.h"


class Lexer {
	std::string raw_content;
	unsigned line;
	unsigned column;
	
	void match_token(std::string& content);

	void remove_whitespaces_at_start(std::string& str);

	bool match_plus_equal(std::string& content);
	bool match_minus_equal(std::string& content);
	bool match_star_equal(std::string& content);
	bool match_divide_equal(std::string& content);
	bool match_modulo_equal(std::string& content);

	bool match_left_shift_equal(std::string& content);
	bool match_right_shift_equal(std::string& content);
	bool match_bitwise_and_equal(std::string& content);
	bool match_bitwise_or_equal(std::string& content);
	bool match_bitwise_xor_equal(std::string& content);
	bool match_bitwise_and(std::string& content);
	bool match_bitwise_or(std::string& content);
	bool match_bitwise_not(std::string& content);
	bool match_bitwise_xor(std::string& content);
	bool match_left_shift(std::string& content);
	bool match_right_shift(std::string& content);
	bool match_logical_and(std::string& content);
	bool match_logical_or(std::string& content);

	bool match_scope_operator(std::string& content);
	bool match_arrow(std::string& content);

	bool match_plus_plus(std::string& content);
	bool match_minus_minus(std::string& content);
	bool match_plus(std::string& content);
	bool match_star(std::string& content);
	bool match_minus(std::string& content);
	bool match_modulo(std::string& content);
	bool match_division(std::string& content);
	
	bool match_less_equal(std::string& content);
	bool match_greater_equal(std::string& content);
	bool match_equal_equal(std::string& content);
	bool match_not_equal(std::string& content);
	bool match_equal(std::string& content);
	bool match_less(std::string& content);
	bool match_greater(std::string& content);

	bool match_include_directive(std::string& content);
	bool match_header(std::string& content);

	bool match_left_paren(std::string& content);
	bool match_right_paren(std::string& content);
	bool match_left_brace(std::string& content);
	bool match_right_brace(std::string& content);
	bool match_colon(std::string& content);
	bool match_semicolon(std::string& content);
	bool match_comma(std::string& content);
	bool match_dot_operator(std::string& content);
	bool match_tilde(std::string& content);
	bool match_bracket_operator(std::string& content);

	bool match_int_constant(std::string& content);
	bool match_unsigned_constant(std::string& content);
	bool match_float_constant(std::string& content);
	bool match_char_constant(std::string& content);
	bool match_string_constant(std::string& content);
	bool match_identifier(std::string& content);

	bool match_newline(std::string& content);
	bool match_line_comment(std::string& content);
	bool match_multiline_comment_start(std::string& content);
	bool match_multiline_comment_end(std::string& content);

	bool match_break(std::string& content);
	bool match_case(std::string& content);
	bool match_class(std::string& content); // struct too
	bool match_delete(std::string& content);

	bool match_integer_type(std::string& content);
	bool match_float_type(std::string& content);
	bool match_string_type(std::string& content);
	bool match_char_type(std::string& content);
	bool match_unsigned_type(std::string& content);
	bool match_bool_type(std::string& content);

	bool match_new(std::string& content);
	bool match_access_specifier(std::string& content); // public private protected
	bool match_if(std::string& content);
	bool match_else(std::string& content);

	bool match_bool_value(std::string& content); // true false
	bool match_return(std::string& content);

	bool match_while(std::string& content);
	bool match_for(std::string& content);
	bool match_do(std::string& content);

	bool match_const(std::string& content);
	bool match_void(std::string& content);

	bool match_using(std::string& content);
	bool match_namespace(std::string& content);

	bool match_cin(std::string& content);
	bool match_cout(std::string& content);
	
public:

	std::vector<Token> tokens;
	Lexer(std::string content);

	void produce_tokens();
	void print_tokens() const;
};