#include "lexer.h"
#include "utility_funcs.h"
#include <regex>
#include <iostream>
#include <string>

Lexer::Lexer(std::string content) {
	raw_content = content;
	line = 1;
	column = 1;
}

void Lexer::print_tokens() const {
	for (Token t : tokens) {
		std::cout << t << '\n';
	}
}

void Lexer::remove_whitespaces_at_start(std::string& str) {
	int i = 0;
	
	while (!str.empty() && str[i] == ' ') {
		column++;
		i++;
	}

	str = str.substr(i, std::string::npos);
}

bool Lexer::match_using(std::string& content) {
	std::regex rgx("using");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();
		
		tokens.push_back(Token(TokenType::Using, it.str(),line,column,len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_namespace(std::string& content) {
	std::regex rgx("namespace");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Namespace, it.str(),line,column,len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_newline(std::string& content) {
	std::regex rgx("\n");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		/*
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::NewLine, it.str(),line,column,len));
		*/

		line++;
		column = 1;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_plus_equal(std::string& content) {
	std::regex rgx("\\+=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::PlusEqual,it.str(),line,column,len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_minus_equal(std::string& content) {
	std::regex rgx("-=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::MinusEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_star_equal(std::string& content) {
	std::regex rgx("\\*=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::StarEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_divide_equal(std::string& content) {
	std::regex rgx("/=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::DivideEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_modulo_equal(std::string& content) {
	std::regex rgx("%=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::ModuloEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_left_shift_equal(std::string& content) {
	std::regex rgx("<<=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LeftShiftEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_right_shift_equal(std::string& content) {
	std::regex rgx(">>=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::RightShiftEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_bitwise_and_equal(std::string& content) {
	std::regex rgx("&=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BitwiseAndEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_bitwise_or_equal(std::string& content) {
	std::regex rgx("\|=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BitwiseOrEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_bitwise_xor_equal(std::string& content) {
	std::regex rgx("\^=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BitwiseXorEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_scope_operator(std::string& content) {
	std::regex rgx("::");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::ScopeOperator, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_arrow(std::string& content) {
	std::regex rgx("->");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Arrow, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_plus_plus(std::string& content) {
	std::regex rgx("\\+\\+");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::PlusPlus, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_minus_minus(std::string& content) {
	std::regex rgx("--");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::MinusMinus, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_plus(std::string& content) {
	std::regex rgx("\\+");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Plus, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_star(std::string& content) {
	std::regex rgx("\\*");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Star, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_minus(std::string& content) {
	std::regex rgx("-");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Minus, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_modulo(std::string& content) {
	std::regex rgx("%");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Modulo, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_line_comment(std::string& content) {
	std::regex rgx("//");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LineComment, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_multiline_comment_start(std::string& content) {
	std::regex rgx("/\\*");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::MultilineCommentStart, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_multiline_comment_end(std::string& content) {
	std::regex rgx("\\*/");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::MultilineCommentEnd, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}


bool Lexer::match_division(std::string& content) {
	std::regex rgx("/");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Division, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_bitwise_xor(std::string& content) {
	std::regex rgx("\\^");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BitwiseXor, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_left_shift(std::string& content) {
	std::regex rgx("<<");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LeftShift, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_right_shift(std::string& content) {
	std::regex rgx(">>");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::RightShift, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_less_equal(std::string& content) {
	std::regex rgx("<=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LessEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_greater_equal(std::string& content) {
	std::regex rgx(">=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::GreaterEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_equal_equal(std::string& content) {
	std::regex rgx("==");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::EqualEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_not_equal(std::string& content) {
	std::regex rgx("!=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::NotEqual, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_bitwise_and(std::string& content) {
	std::regex rgx("&");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BitwiseAnd, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_bitwise_or(std::string& content) {
	std::regex rgx("\\|");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BitwiseOr, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_bitwise_not(std::string& content) {
	std::regex rgx("!");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BitwiseNot, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_logical_and(std::string& content) {
	std::regex rgx("&&");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LogicalAnd, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_logical_or(std::string& content) {
	std::regex rgx("\\|\\|");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LogicalOr, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}


bool Lexer::match_equal(std::string& content) {
	std::regex rgx("=");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Equal, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_less(std::string& content) {
	std::regex rgx("<");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Less, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_greater(std::string& content) {
	std::regex rgx(">");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Greater, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;

}

bool Lexer::match_include_directive(std::string& content) {
	std::regex rgx("#include");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::IncludeDirective, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_header(std::string& content) {
	std::regex rgx(R"(".+\.h")");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Header, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_left_paren(std::string& content) {
	std::regex rgx("\\(");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LeftParen, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_right_paren(std::string& content) {
	std::regex rgx("\\)");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::RightParen, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_left_brace(std::string& content) {
	std::regex rgx("\\{");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::LeftBrace, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_right_brace(std::string& content) {
	std::regex rgx("\\}");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::RightBrace, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_colon(std::string& content) {
	std::regex rgx(":");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Colon, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_semicolon(std::string& content) {
	std::regex rgx(";");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Semicolon, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_comma(std::string& content) {
	std::regex rgx(",");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Comma, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_dot_operator(std::string& content) {
	std::regex rgx("\\.");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::DotOperator, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_tilde(std::string& content) {
	std::regex rgx("~");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Tilde, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_bracket_operator(std::string& content) {
	std::regex rgx("\\[\\]|\\[(0|[1-9][0-9]*)\\]");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::BracketOperator, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}


bool Lexer::match_unsigned_constant(std::string& content) {
	std::regex rgx("[0]|[1-9][0-9]*");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::UnsignedConst, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_int_constant(std::string& content) {
	std::regex rgx("[0]|[-+]?[1-9][0-9]*");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::IntConst, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_float_constant(std::string& content) {
	std::regex rgx("[+-]?([0]|[1-9]*)?[.][0-9]+");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::FloatConst, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_char_constant(std::string& content) {
	std::regex rgx(R"('[^'\\]'|'\\a'|'\\b'|'\\f'|'\\n'|'\\r'|'\\t'|'\\v'|'\\''|'\\\\'|'\\"'|'\\?')");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::CharConst, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_string_constant(std::string& content) {
	std::regex rgx(R"("[^"]*")");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::StringConst, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_identifier(std::string& content) {
	std::regex rgx("[a-zA-Z_][a-zA-Z0-9_]*");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Identifier, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_break(std::string& content) {
	std::regex rgx("break");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Break, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_case(std::string& content) {
	std::regex rgx("case");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Case, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_class(std::string& content) {
	std::regex rgx("class|struct");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		if (it.str() == "class") {
			unsigned len = it.str().length();

			tokens.push_back(Token(TokenType::Class, it.str(), line, column, len));
			column = column + len;

		}
		else {
			unsigned len = it.str().length();

			tokens.push_back(Token(TokenType::Struct, it.str(), line, column, len));
			column = column + len;

		}
		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_delete(std::string& content) {
	std::regex rgx("delete");
	std::smatch it;


	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Delete, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_integer_type(std::string& content) {
	std::regex rgx("int|signed|short|long");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::IntegerType, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_float_type(std::string& content) {
	std::regex rgx("double|float");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::FloatType, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_string_type(std::string& content) {
	std::regex rgx("string");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::String, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_char_type(std::string& content) {
	std::regex rgx("char");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Char, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_unsigned_type(std::string& content) {
	std::regex rgx("unsigned");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Unsigned, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_bool_type(std::string& content) {
	std::regex rgx("bool");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Bool, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_new(std::string& content) {
	std::regex rgx("new");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::New, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_access_specifier(std::string& content) {
	std::regex rgx("public|private|protected");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::AccessSpecifier, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_if(std::string& content) {
	std::regex rgx("if");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::If, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_else(std::string& content) {
	std::regex rgx("else");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Else, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_bool_value(std::string& content) {
	std::regex rgx("true|false");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		if (it.str() == "true") {
			unsigned len = it.str().length();

			tokens.push_back(Token(TokenType::True, it.str(), line, column, len));
			column = column + len;

		}
		else {
			unsigned len = it.str().length();

			tokens.push_back(Token(TokenType::False, it.str(), line, column, len));
			column = column + len;

		}
		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_return(std::string& content) {
	std::regex rgx("return");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Return, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_while(std::string& content) {
	std::regex rgx("while");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::While, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_for(std::string& content) {
	std::regex rgx("for");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::For, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_do(std::string& content) {
	std::regex rgx("do");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Do, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_const(std::string& content) {
	std::regex rgx("const");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Const, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_cin(std::string& content) {
	std::regex rgx("cin");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Cin, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_cout(std::string& content) {
	std::regex rgx("cout");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Cout, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}

bool Lexer::match_void(std::string& content) {
	std::regex rgx("void");
	std::smatch it;

	if (std::regex_search(content, it, rgx, std::regex_constants::match_continuous)) {
		unsigned len = it.str().length();

		tokens.push_back(Token(TokenType::Void, it.str(), line, column, len));
		column = column + len;

		content = it.suffix();
		return true;
	}

	return false;
}


void Lexer::match_token(std::string& content) {
	if (match_newline(content)) {
		return;
	}

	if (match_header(content)) {
		return;
	}

	if (match_using(content)) {
		return;
	}

	if (match_namespace(content)) {
		return;
	}

	if (match_break(content)) {
		return;
	}

	if (match_case(content)) {
		return;
	}

	if (match_class(content)) {
		return;
	}

	if (match_integer_type(content)) {
		return;
	}

	if (match_float_type(content)) {
		return;
	}

	if (match_string_type(content)) {
		return;
	}

	if (match_char_type(content)) {
		return;
	}

	if (match_unsigned_type(content)) {
		return;
	}

	if (match_bool_type(content)) {
		return;
	}

	if (match_new(content)) {
		return;
	}

	if (match_access_specifier(content)) {
		return;
	}

	if (match_if(content)) {
		return;
	}

	if (match_else(content)) {
		return;
	}

	if (match_bool_value(content)) {
		return;
	}

	if (match_return(content)) {
		return;
	}

	if (match_while(content)) {
		return;
	}

	if (match_for(content)) {
		return;
	}
	
	if (match_float_constant(content)) {
		return;
	}

	if (match_do(content)) {
		return;
	}

	if (match_const(content)) {
		return;
	}

	if (match_void(content)) {
		return;
	}

	if (match_int_constant(content)) {
		return;
	}

	if (match_string_constant(content)) {
		return;
	}

	if (match_char_constant(content)) {
		return;
	}

	if (match_include_directive(content)) {
		return;
	}

	if (match_left_paren(content)) {
		return;
	}

	if (match_right_paren(content)) {
		return;
	}

	if (match_left_brace(content)) {
		return;
	}

	if (match_right_brace(content)) {
		return;
	}

	if (match_colon(content)) {
		return;
	}

	if (match_semicolon(content)) {
		return;
	}

	if (match_comma(content)) {
		return;
	}

	if (match_dot_operator(content)) {
		return;
	}

	if (match_bracket_operator(content)) {
		return;
	}

	if (match_equal_equal(content)) {
		return;
	}

	if (match_equal(content)) {
		return;
	}

	if (match_greater_equal(content)) {
		return;
	}

	if (match_less_equal(content)) {
		return;
	}

	if (match_not_equal(content)) {
		return;
	}

	if (match_left_shift(content)) {
		return;
	}

	if (match_right_shift(content)) {
		return;
	}

	if (match_less(content)) {
		return;
	}

	if (match_greater(content)) {
		return;
	}

	if (match_plus_plus(content)) {
		return;
	}

	if (match_minus_minus(content)) {
		return;
	}

	if (match_plus(content)) {
		return;
	}

	if (match_minus(content)) {
		return;
	}

	if (match_multiline_comment_start(content)) {
		return;
	}

	if (match_multiline_comment_end(content)) {
		return;
	}

	if (match_star(content)) {
		return;
	}

	if (match_line_comment(content)) {
		return;
	}

	if (match_division(content)) {
		return;
	}

	if (match_modulo(content)) {
		return;
	}

	if (match_bitwise_xor(content)) {
		return;
	}

	if (match_bitwise_not(content)) {
		return;
	}

	if (match_logical_and(content)) {
		return;
	}

	if (match_logical_or(content)) {
		return;
	}

	if (match_scope_operator(content)) {
		return;
	}

	if (match_arrow(content)) {
		return;
	}

	if (match_plus_equal(content)) {
		return;
	}

	if (match_minus_equal(content)) {
		return;
	}

	if (match_star_equal(content)) {
		return;
	}

	if (match_divide_equal(content)) {
		return;
	}

	if (match_modulo_equal(content)) {
		return;
	}

	if (match_cin(content)) {
		return;
	}

	if (match_cout(content)) {
		return;
	}

	if (match_identifier(content)) {
		return;
	}
}

void Lexer::produce_tokens() {

		while (raw_content != "") {
			remove_whitespaces_at_start(raw_content);

			std::string previous = raw_content;

			match_token(raw_content);

			if (raw_content == previous) {
				break;
			}
		}

		tokens.push_back(Token(TokenType::EndOfTokens));
}