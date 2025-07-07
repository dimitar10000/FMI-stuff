#include "token.h"

Token::Token(TokenType type, std::string value, unsigned line, unsigned column, unsigned length) {
	this->type = type;
	this->value = value;
	this->line = line;
	this->column = column;
	this->length = length;
}

Token::Token(const Token& other) {
	type = other.type;
	value = other.value;
	line = other.line;
	column = other.column;
	length = other.length;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
	std::string names[]{
	"LeftParen",
	"RightParen",
	"LeftBrace",
	"RightBrace",
	"Comma",
	"DotOperator",
	"Tilde",
	"BracketOperator",
	"Minus",
	"Plus",
	"Semicolon",
	"Star",
	"Division",
	"Modulo",
	"BitwiseNot",
	"BitwiseAnd",
	"BitwiseXor",
	"BitwiseOr",
	"QuestionMark",
	"Colon",
	"LeftSquareBracket",
	"RightSquareBracket",

	"Arrow",
	"ScopeOperator",

	"Equal",
	"NotEqual",
	"EqualEqual",
	"Greater",
	"GreaterEqual",
	"Less",
	"LessEqual",

	"MinusMinus",
	"PlusPlus",
	"LeftShift",
	"RightShift",
	"LogicalAnd",
	"LogicalOr",
	"StarEqual",
	"DivideEqual",
	"ModuloEqual",
	"PlusEqual",
	"MinusEqual",
	"LeftShiftEqual",
	"RightShiftEqual",
	"BitwiseAndEqual",
	"BitwiseXorEqual",
	"BitwiseOrEqual",
	"Backslash",

	"Identifier",
	"StringConst",
	"CharConst",
	"IntConst",
	"UnsignedConst",
	"FloatConstant",

	"Break",
	"Case",
	"Catch",
	"Class",
	"Const",
	"Continue",
	"Default",
	"Delete",
	"Do",
	"Else",
	"Finally",
	"For",
	"If",
	"New",
	"Return",
	"Struct",
	"Switch",
	"This",
	"Throw",
	"Try",
	"Void",
	"While",
	"True",
	"False",

	"Char",
	"FloatType",
	"IntegerType",
	"Unsigned",
	"String",
	"Bool",

	"NewLine",
	"LineComment",
	"MultilineCommentStart",
	"MultilineCommentEnd",

	"AccessSpecifier",
	"Using",
	"Namespace",

	"IncludeDirective",
	"Header",

	"Cin",
	"Cout",

	"EndOfTokens"
	};

	return os  << names[static_cast<int>(token.type)]  << " " << token.value << " "
		<< token.line << " " << token.column;
}