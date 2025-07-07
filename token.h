#pragma once
#include <string>
#include <sstream>

enum class TokenType {
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
	Comma,
	DotOperator,
	Tilde,
	BracketOperator,
	Minus,
	Plus,
	Semicolon,
	Star,
	Division,
	Modulo,
	BitwiseNot,
	BitwiseAnd,
	BitwiseXor,
	BitwiseOr,
	QuestionMark,
	Colon,
	LeftSquareBracket,
	RightSquareBracket,
	ScopeOperator,
	Arrow,

	Equal,
	NotEqual,
	EqualEqual,
	Greater,
	GreaterEqual,
	Less,
	LessEqual,
	MinusMinus,
	PlusPlus,
	LeftShift,
	RightShift,
	LogicalAnd,
	LogicalOr,
	StarEqual,
	DivideEqual,
	ModuloEqual,
	PlusEqual,
	MinusEqual,
	LeftShiftEqual,
	RightShiftEqual,
	BitwiseAndEqual,
	BitwiseXorEqual,
	BitwiseOrEqual,
	Backslash,

	Identifier,
	StringConst,
	CharConst,
	IntConst,
	UnsignedConst,
	FloatConst,

	Break,
	Case,
	Catch,
	Class,
	Const,
	Continue,
	Default,
	Delete,
	Do,
	Else,
	Finally,
	For,
	If,
	New,
	Return,
	Struct,
	Switch,
	This,
	Throw,
	Try,
	Void,
	While,
	True,
	False,

	// char, double, float, int, unsigned, string, signed, short, bool, long
	Char,
	FloatType,
	IntegerType,
	Unsigned,
	String,
	Bool,

	NewLine,
	LineComment,
	MultilineCommentStart,
	MultilineCommentEnd,

	AccessSpecifier,
	Using,
	Namespace,

	IncludeDirective,
	Header,

	Cin,
	Cout,

	EndOfTokens
};

struct Token {
	TokenType type;
	std::string value;
	unsigned line;
	unsigned column;
	unsigned length;

	Token() {};
	Token(TokenType type, std::string value = "",unsigned line=0,unsigned column = 0,unsigned length = 0);

	Token(const Token& other);
	
};

std::ostream& operator<<(std::ostream& os, const Token& token);