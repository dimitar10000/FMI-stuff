#include "parser.h"
#include "ast-builder.h"
#include <iostream>
#include <stack>

Parser::Parser(std::vector<Token> tokens_array) {
	tokens = tokens_array;
	current_token = 0;
}

bool Parser::match_type(Token token, TokenType type) {
	return token.type == type;
}

bool Parser::match_one_of(Token token, std::vector<TokenType> types) {
	for (TokenType token_type : types) {
		if (token.type == token_type) {
			return true;
		}
	}

	return false;
}

bool Parser::finished_parsing() {
	return tokens[current_token].type == TokenType::EndOfTokens;
}

Token Parser::next_token() {
	current_token++;


	if (finished_parsing()) {
		return Token(TokenType::EndOfTokens);
	}

	return tokens[current_token];
}
Token Parser::prev_token() {
	current_token--;

	return tokens[current_token];
}

Token Parser::peek() {
	return tokens[current_token];
}

AST* Parser::parse_include_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	if (!match_type(curr, TokenType::IncludeDirective)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));

	curr = next_token();
	AST* library = nullptr;
	try { library = parse_library_expr(); }
	catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

	if (library != nullptr) {
		nodes.push_back(library);

		AST* result = new AST("IncludeExpr");
		result->add_children(nodes);

		return result;
	}


	AST* header = nullptr;
	try { header = parse_header_expr(); }
	catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

	if (header != nullptr) {
		nodes.push_back(header);

		AST* result = new AST("IncludeExpr");
		result->add_children(nodes);

		return result;
	}
}

AST* Parser::parse_library_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	if (!match_type(curr, TokenType::Less)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));

	curr = next_token();

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));

	curr = next_token();

	if (!match_type(curr, TokenType::Greater)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));

	AST* result = new AST("LibraryExpr");
	result->add_children(nodes);

	return result;
}

AST* Parser::parse_header_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	if (!match_type(curr, TokenType::Header)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));

	AST* result = new AST("HeaderExpr");
	result->add_children(nodes);

	return result;
}

AST* Parser::parse_using_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	if (!match_type(curr, TokenType::Using)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));

	curr = next_token();

	if (!match_type(curr, TokenType::Namespace)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));

	curr = next_token();

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));

	curr = next_token();

	if (!match_type(curr, TokenType::Semicolon)) {
		throw curr;
	}

	AST* result = new AST("UsingExpr");
	result->add_children(nodes);

	return result;
}

AST* Parser::parse_var_declaration_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	// the expression isn't a declaration
	if (current_token + 2 <= tokens.size() - 1
		&& tokens[current_token + 2].type != TokenType::Semicolon) {
		return nullptr;
	}

	std::vector<TokenType> accepted_types{ TokenType::IntegerType,TokenType::FloatType,
	TokenType::String,TokenType::Unsigned,TokenType::Bool,TokenType::Char, TokenType::Class,
	TokenType::Struct};

	if (!match_one_of(curr, accepted_types)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::Semicolon)) {
		throw curr;
	}

	curr = next_token();

	AST* result = new AST("VarDeclExpr");
	result->add_children(nodes);

	return result;
}

int precedence(Token token) {
	if (token.type == TokenType::BitwiseXor) {
		return 3;
	}

	if (token.type == TokenType::Star || token.type == TokenType::Division || token.type == TokenType::Modulo) {
		return 2;
	}

	if (token.type == TokenType::Plus || token.type == TokenType::Minus) {
		return 1;
	}

	if (token.type == TokenType::BitwiseNot) {
		return 3;
	}

	if (token.type == TokenType::LogicalAnd) {
		return 2;
	}

	if (token.type == TokenType::LogicalOr) {
		return 1;
	}
}

AST* Parser::build_arithmetic_tree(std::vector<Token> tokens) {
	std::stack<Token> ops;
	std::stack<AST*> trees;

	std::vector<TokenType> accepted_types{ TokenType::IntConst,TokenType::UnsignedConst,TokenType::FloatConst,TokenType::Identifier };

	for (int i = 0; i < tokens.size(); i++) {
		if (match_one_of(tokens[i], accepted_types)) {
			AST* tree = new AST(tokens[i]);
			trees.push(tree);
		}
		else if (ops.empty() || tokens[i].type == TokenType::LeftParen) {
			ops.push(tokens[i]);
		}
		else {
			Token top_token = ops.top();

			if (top_token.type == TokenType::LeftParen) {
				ops.push(tokens[i]);
				continue;
			}

			if (tokens[i].type == TokenType::RightParen) {
				while (top_token.type != TokenType::LeftParen) {
					ops.pop();

					std::vector<AST*> children;
					AST* child1 = new AST(*trees.top());
					trees.pop();
					AST* child2 = new AST(*trees.top());
					trees.pop();

					children.push_back(child2);
					children.push_back(child1);

					AST* new_tree = new AST(top_token);
					new_tree->add_children(children);

					trees.push(new_tree);

					top_token = ops.top();
				}
				ops.pop(); // removing the (
				continue;
			}
			else if (precedence(tokens[i]) >= precedence(top_token)) {
				ops.push(tokens[i]);
			}
			else {
				ops.pop();

				std::vector<AST*> children;
				AST* child1 = new AST(*trees.top());
				trees.pop();
				AST* child2 = new AST(*trees.top());
				trees.pop();

				children.push_back(child2);
				children.push_back(child1);

				AST* new_tree = new AST(top_token);
				new_tree->add_children(children);

				trees.push(new_tree);

				i--; // we will examine the same token from input again
			}
		}
	}

	AST* result;

	while (!ops.empty()) {
		Token op = ops.top();
		ops.pop();

		std::vector<AST*> children;
		AST* child1 = new AST(*trees.top());
		trees.pop();
		AST* child2 = new AST(*trees.top());
		trees.pop();

		children.push_back(child2);
		children.push_back(child1);

		AST* new_tree = new AST(op);
		new_tree->add_children(children);

		trees.push(new_tree);
	}

	result = trees.top();

	return result;
}

AST* Parser::parse_arithmetic_expr() {
	Token curr = peek();
	std::vector<Token> tokens;

	bool matched_one = false;

	int open_paren = 0;
	int close_paren = 0;

	while (match_type(curr, TokenType::LeftParen)) {
		tokens.push_back(curr);
		curr = next_token();
		open_paren++;
	}

	std::vector<TokenType> accepted_types{ TokenType::IntConst,TokenType::UnsignedConst,TokenType::FloatConst, TokenType::Identifier };

	std::vector<TokenType> accepted_ops{ TokenType::Plus,TokenType::Minus,TokenType::Star,
		TokenType::Division,TokenType::Modulo,TokenType::BitwiseXor };

	if (!match_one_of(curr, accepted_types)) {
		return nullptr;
	}
	matched_one = true;
	tokens.push_back(curr);
	curr = next_token();

	while (match_one_of(curr, accepted_ops)) {
		tokens.push_back(curr);
		curr = next_token();

		while (match_type(curr, TokenType::LeftParen)) {
			tokens.push_back(curr);
			curr = next_token();
			open_paren++;
		}

		if (!match_one_of(curr, accepted_types)) {
			throw curr;
		}
		tokens.push_back(curr);
		curr = next_token();

		while (match_type(curr, TokenType::RightParen)) {
			tokens.push_back(curr);
			curr = next_token();
			close_paren++;
		}
	}

	if (open_paren != close_paren) {
		throw "parentheses error";
	}

	AST* child = build_arithmetic_tree(tokens);
	std::vector<AST*> children = { child };

	AST* result = new AST("ArithmExpr");
	result->add_children(children);

	return result;
}

AST* Parser::parse_func_definition_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	// the expression isn't a func definition
	if (current_token + 2 > tokens.size() - 1
		|| tokens[current_token + 1].type != TokenType::Identifier
		|| tokens[current_token + 2].type != TokenType::LeftParen) {
		return nullptr;
	}

	std::vector<TokenType> accepted_types{ TokenType::IntegerType,TokenType::FloatType,
	TokenType::String,TokenType::Unsigned,TokenType::Bool,TokenType::Char, TokenType::Void };

	if (!match_one_of(curr, accepted_types)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	AST* args = new AST("Arguments");

	if (match_type(curr, TokenType::RightParen)) {
		curr = next_token();
	}
	else {
		std::vector<AST*> args_list = parse_func_args_expr();
		args->add_children(args_list);

		curr = peek();

		if (!match_type(curr, TokenType::RightParen)) {
			throw curr;
		}
		curr = next_token();
	}

	nodes.push_back(args);

	if (match_type(curr, TokenType::Semicolon)) {
		AST* result = new AST("FuncDeclExpr");
		result->add_children(nodes);

		next_token();

		return result;
	}
	else if (match_type(curr, TokenType::LeftBrace)) {
		next_token();
		AST* body = parse_braces_body("FuncBody");

		if (body != nullptr) {
			nodes.push_back(body);
		}
		else {
			curr = peek();
			throw curr;
		}

		AST* result = new AST("FuncDefExpr");
		result->add_children(nodes);

		return result;
	}
	
	throw curr;
}

std::vector<AST*> Parser::parse_func_args_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	std::vector<TokenType> accepted_types{ TokenType::IntegerType,TokenType::FloatType,
	TokenType::String,TokenType::Unsigned,TokenType::Bool,TokenType::Char,TokenType::Identifier };

	bool matched_one = false;

	while (match_one_of(curr, accepted_types)) {
		AST* new_node = new AST("DeclExpr");
		std::vector<AST*> node_children;

		node_children.push_back(new AST(Token(curr)));

		curr = next_token();

		if (!match_type(curr, TokenType::Identifier)) {
			throw curr;
			break;
		}
		node_children.push_back(new AST(Token(curr)));
		matched_one = true;

		new_node->add_children(node_children);
		nodes.push_back(new_node);

		curr = next_token();
		if (match_type(curr, TokenType::Comma)) {
			curr = next_token();
		}
	}

	if (!matched_one) {
		exit(-1);
	}

	return nodes;
}

AST* Parser::parse_func_call_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;
	
	// the expression isn't a func call
	if (current_token + 1 > tokens.size() - 1
		|| tokens[current_token].type != TokenType::Identifier
		|| tokens[current_token + 1].type != TokenType::LeftParen) {
		return nullptr;
	}

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}

	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	AST* args = new AST("Arguments");

	if (match_type(curr, TokenType::RightParen)) {
		curr = next_token();
	}
	else {
		std::vector<AST*> args_list = parse_func_args_expr();
		args->add_children(args_list);

		curr = peek();

		if (!match_type(curr, TokenType::RightParen)) {
			throw curr;
		}
		curr = next_token();
	}

	nodes.push_back(args);

	if (match_type(curr, TokenType::Semicolon)) {
		AST* result = new AST("FuncCallExpr");
		result->add_children(nodes);

		return result;
	}

	throw curr;
}


AST* Parser::build_string_tree(std::vector<Token> tokens) {
	std::stack<Token> ops;
	std::stack<AST*> trees;

	// special case, string expression without ops
	if (tokens.size() == 1) {
		return new AST(tokens[0]);
	}

	std::vector<TokenType> accepted_types{ TokenType::StringConst,TokenType::Identifier };

	for (int i = 0; i < tokens.size(); i++) {
		if (match_one_of(tokens[i], accepted_types)) {
			AST* tree = new AST(tokens[i]);
			trees.push(tree);
		}
		else {
			ops.push(tokens[i]);
		}
	}

	AST* result;

	while (!ops.empty()) {
		Token op = ops.top();
		ops.pop();

		std::vector<AST*> children;
		AST* child1 = new AST(*trees.top());
		trees.pop();
		AST* child2 = new AST(*trees.top());
		trees.pop();

		children.push_back(child2);
		children.push_back(child1);

		AST* new_tree = new AST(op);
		new_tree->add_children(children);

		trees.push(new_tree);
	}

	result = trees.top();

	return result;
}

AST* Parser::parse_string_expr() {
	Token curr = peek();
	std::vector<Token> tokens;

	bool matched_one = false;

	std::vector<TokenType> accepted_types{ TokenType::StringConst, TokenType::Identifier };

	if (!match_one_of(curr, accepted_types)) {
		return nullptr;
	}
	matched_one = true;
	tokens.push_back(curr);
	curr = next_token();

	while (match_type(curr, TokenType::Plus)) {
		tokens.push_back(curr);
		curr = next_token();

		if (!match_one_of(curr, accepted_types)) {
			throw curr;
		}
		tokens.push_back(curr);
		curr = next_token();
	}

	AST* child = build_string_tree(tokens);
	std::vector<AST*> children = { child };

	AST* result = new AST("StringExpr");
	result->add_children(children);

	return result;
}

AST* Parser::build_logical_tree(std::vector<AST*> trees) {
	std::stack<AST*> ops;
	std::stack<AST*> exprs;

	// special case, logical expression without ops and <=
	if (trees.size() == 1) {
		return trees[0];
	}

	std::vector<TokenType> accepted_ops{ TokenType::LogicalAnd,TokenType::LogicalOr,
		TokenType::LeftParen,TokenType::RightParen };

	for (int i = 0; i < trees.size(); i++) {
		auto root = trees[i]->get_root();

		if (root.index() == 0) {
			exprs.push(trees[i]);
			continue;
		}

		Token root_value = std::get<Token>(root);

		std::cout << root_value << '\n';

		if (match_one_of(root_value, accepted_ops)) {
			if (ops.empty() || root_value.type == TokenType::LeftParen) {
				ops.push(trees[i]);
			}
			else {
				Token top_token = std::get<Token>(ops.top()->get_root());

				if (root_value.type != TokenType::RightParen && top_token.type == TokenType::LeftParen) {
					ops.push(trees[i]);
					continue;
				}

				if (root_value.type == TokenType::RightParen) {
					while (top_token.type != TokenType::LeftParen) {
						ops.pop();

						std::vector<AST*> children;
						AST* child1 = exprs.top();
						exprs.pop();
						AST* child2 = exprs.top();
						exprs.pop();

						children.push_back(child2);
						children.push_back(child1);

						AST* new_tree = new AST(top_token);
						new_tree->add_children(children);

						exprs.push(new_tree);

						top_token = std::get<Token>(ops.top()->get_root());
					}
					ops.pop(); // removing the (
					continue;
				}
				else if (precedence(root_value) >= precedence(top_token)) {
					ops.push(trees[i]);
				}
				else {
					AST* ops_top = ops.top();
					ops.pop();

					std::vector<AST*> children;
					AST* child1 = exprs.top();
					exprs.pop();
					AST* child2 = exprs.top();
					exprs.pop();

					children.push_back(child1);
					children.push_back(child2);
					ops_top->add_children(children);

					exprs.push(ops_top);

					i--; // we will examine the same token from input again
				}
			}
		}
		else {
			exprs.push(trees[i]);
		}
	}

	while (!ops.empty()) {
		AST* ops_top = ops.top();
		Token root_value = std::get<Token>(ops_top->get_root());

		ops.pop();

		std::vector<AST*> children;

		AST* child1 = exprs.top();
		exprs.pop();
		AST* child2 = exprs.top();
		exprs.pop();

		children.push_back(child2);
		children.push_back(child1);
		ops_top->add_children(children);

		exprs.push(ops_top);
	}

	AST* result = exprs.top();

	return result;
}

AST* Parser::parse_logical_expr() {
	Token curr = peek();
	std::vector<AST*> children;
	std::vector<AST*> paren;

	std::cout << "parsing logical\n";

	AST* bool_expr = parse_boolean_expr(paren);
	bool parsed_one = false;

	while (bool_expr != nullptr) {
		parsed_one = true;

		int i = 0;

		while (!paren.empty() && i <= paren.size() - 1 && std::get<Token>(paren[i]->get_root()).type == TokenType::LeftParen) {
			children.push_back(paren[i]);
			i++;
		}
		children.push_back(bool_expr);
		while (!paren.empty() && i <= paren.size() - 1 && std::get<Token>(paren[i]->get_root()).type == TokenType::RightParen) {
			children.push_back(paren[i]);
			i++;
		}
		paren.clear();

		curr = peek();

		std::vector<TokenType> accepted_ops{ TokenType::LogicalAnd,TokenType::LogicalOr };

		if (match_one_of(curr, accepted_ops)) {
			AST* op = new AST(curr);
			children.push_back(op);
			curr = next_token();
		}
		else {
			break;
		}

		bool_expr = parse_boolean_expr(paren);
	}

	if (parsed_one) {

		AST* new_tree = new AST("LogicalExpr");
		AST* result = build_logical_tree(children);

		std::vector<AST*> child = { result };
		new_tree->add_children(child);

		return new_tree;
	}

	return nullptr;
}

AST* Parser::chain_negations() {
	AST* negation = nullptr;
	std::vector<AST*> negations;

	Token curr = peek();

	while (curr.type == TokenType::BitwiseNot) {
		negations.push_back(new AST(curr));
		curr = next_token();
	}

	if (negations.size() > 1) {
		for (int i = 0; i < negations.size() - 1; i++) {
			std::vector<AST*> new_child = { negations[i + 1] };
			negations[i]->add_children(new_child);
		}
		negation = negations[0];

		return negation;
	}
	else if (negations.size() == 1) {
		negation = negations[0];

		return negation;
	}
	else {
		return nullptr;
	}
}

AST* Parser::get_last_node(AST* tree) {
	while (!tree->get_children().empty()) {
		tree = tree->get_children()[0];
	}

	return tree;
}

AST* Parser::parse_boolean_expr(std::vector<AST*>& paren) {
	Token curr = peek();
	std::vector<AST*> children;

	int open_paren = 0;
	int close_paren = 0;

	if (match_type(curr, TokenType::LeftParen)) {
		paren.push_back(new AST(curr));
		curr = next_token();
		open_paren++;
	}

	AST* negation_tree = chain_negations();
	curr = peek();

	std::vector<TokenType> accepted_types = {TokenType::True,TokenType::False };

	AST* lhs = parse_arithmetic_expr();

	if (lhs == nullptr) {
		if (!match_one_of(curr, accepted_types)) {
			return nullptr;
		}
		else {
			lhs = new AST(curr);
			curr = next_token();
		}
	}

	curr = peek();

	AST* last_node1 = nullptr;
	if (negation_tree != nullptr) {
		std::vector<AST*> new_child = { lhs };

		last_node1 = get_last_node(negation_tree);
		last_node1->add_children(std::vector<AST*>{ new_child});
	}

	std::vector<TokenType> accepted_ops = { TokenType::EqualEqual, TokenType::NotEqual,TokenType::Less,
	TokenType::LessEqual,TokenType::Greater,TokenType::GreaterEqual,TokenType::LeftShiftEqual };

	AST* op = nullptr;
	if (match_one_of(curr, accepted_ops)) {
		op = new AST(curr);
		curr = next_token();
	}

	AST* negation_tree2 = chain_negations();
	curr = peek();

	AST* rhs = parse_arithmetic_expr();

	curr = peek();

	if (op != nullptr) {
		if (rhs == nullptr && !match_one_of(curr, accepted_types)) {
			throw curr;
		}
		else if (match_one_of(curr,accepted_types)) {
			rhs = new AST(curr);
			curr = next_token();
		}
	}

	AST* last_node2 = nullptr;
	if (negation_tree2 != nullptr) {
		std::vector<AST*> new_child = { rhs };

		last_node2 = get_last_node(negation_tree2);
		last_node2->add_children(std::vector<AST*>{ new_child});
	}

	if (match_type(curr, TokenType::RightParen)) {
		if (close_paren == open_paren) {
			paren.clear(); // mismatched parens, discard all
		}
		else {
			paren.push_back(new AST(curr));
			next_token();
		}
	}

	if (op == nullptr) {
		if (negation_tree != nullptr) {
			return negation_tree;
		}
		return lhs;
	}

	if (negation_tree != nullptr) {
		children.push_back(negation_tree);
	}
	else {
		children.push_back(lhs);
	}

	if (negation_tree2 != nullptr) {
		children.push_back(negation_tree2);
	}
	else {
		children.push_back(rhs);
	}

	op->add_children(children);
	return op;
}

AST* Parser::parse_simple_assignment_expr() {
	Token curr = peek();
	std::vector<AST*> children;

	std::vector<TokenType> accepted_ops = { TokenType::Equal, TokenType::StarEqual,TokenType::DivideEqual,
	TokenType::ModuloEqual,TokenType::PlusEqual,TokenType::MinusEqual,TokenType::LeftShiftEqual,
	TokenType::RightShiftEqual,TokenType::BitwiseAndEqual,TokenType::BitwiseXorEqual, TokenType::BitwiseOrEqual };

	// the expression isn't an assignment
	if (current_token + 1 > tokens.size() - 1
		|| !match_one_of(tokens[current_token + 1], accepted_ops)) {
		return nullptr;
	}

	if (!match_type(curr, TokenType::Identifier)) {
		return nullptr;
	}

	AST* lhs = new AST(curr);
	curr = next_token();

	if (!match_one_of(curr, accepted_ops)) {
		throw curr;
	}

	AST* op = new AST(curr);
	curr = next_token();

	AST* rhs = nullptr;
	rhs = parse_arithmetic_expr();

	if (rhs == nullptr) {
		rhs = parse_string_expr();
	}
	if (rhs == nullptr) {
		rhs = parse_logical_expr();
	}
	if (rhs == nullptr && match_type(curr, TokenType::CharConst)) {
		AST* rhs = new AST(curr);
	}

	if (rhs != nullptr) {
		children.push_back(lhs);
		children.push_back(rhs);
		op->add_children(children);

		AST* result = new AST("AssignExpr");
		std::vector<AST*> child = { op };
		result->add_children(child);

		return result;
	}
	else {
		throw curr;
	}
}

AST* Parser::parse_decl_assignment_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;
	std::vector<AST*> lhs_children;

	TokenType matched_type;

	std::vector<TokenType> accepted_types{ TokenType::IntegerType,TokenType::FloatType,
	TokenType::String,TokenType::Unsigned,TokenType::Bool,TokenType::Char };

	// the expression isn't an assignment
	if (current_token + 2 > tokens.size() - 1
		|| tokens[current_token+2].type != TokenType::Equal) {
		return nullptr;
	}

	if (!match_one_of(curr, accepted_types)) {
		return nullptr;
	}
	else {
		matched_type = curr.type;
		lhs_children.push_back(new AST(curr));
		curr = next_token();
	}

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	lhs_children.push_back(new AST(curr));
	curr = next_token();

	if (!match_type(curr, TokenType::Equal)) {
		throw curr;
	}
	AST* op = new AST(curr);

	curr = next_token();
	AST* rhs = nullptr;

	if (matched_type == TokenType::IntegerType || matched_type == TokenType::FloatType
		|| matched_type == TokenType::Unsigned) {
		rhs = parse_arithmetic_expr();
	}
	else if (matched_type == TokenType::String) {
		rhs = parse_string_expr();
	}
	else if (matched_type == TokenType::Char && curr.type == TokenType::CharConst) {
		rhs = new AST(Token(curr));
		next_token();
	}
	else if (matched_type == TokenType::Bool) {
		rhs = parse_logical_expr();
	}

	if (rhs == nullptr) {
		throw curr;
	}

	AST* result = new AST("AssignExpr");

	AST* lhs = new AST("LHS");
	lhs->add_children(lhs_children);

	nodes.push_back(lhs);
	nodes.push_back(rhs);

	op->add_children(nodes);
	std::vector<AST*> child = { op };
	result->add_children(child);

	return result;
}

AST* Parser::parse_input_expr() {
	Token curr = peek();
	std::vector<AST*> children;

	if (!match_type(curr, TokenType::Cin)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	bool parsed_one = false;

	while (match_type(curr, TokenType::RightShift)) {

		AST* op = new AST(curr);
		children.push_back(op);
		curr = next_token();

		if (!match_type(curr, TokenType::Identifier)) {
			throw curr;
			break;
		}
		children.push_back(new AST(curr));
		curr = next_token();

		parsed_one = true;
	}

	if (parsed_one) {
		AST* new_tree = new AST("InputExpr");
		AST* result = build_io_tree(children);

		std::vector<AST*> child = { result };
		new_tree->add_children(child);

		return new_tree;
	}

	return nullptr;
}

AST* Parser::parse_output_expr() {
	Token curr = peek();
	std::vector<AST*> children;

	if (!match_type(curr, TokenType::Cout)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	bool parsed_one = false;

	while (match_type(curr, TokenType::LeftShift)) {

		AST* op = new AST(curr);
		children.push_back(op);
		curr = next_token();

		AST* str_expr = parse_string_expr();

		if (str_expr != nullptr) {
			children.push_back(str_expr);
			curr = peek();
		}
		else if(match_type(curr,TokenType::CharConst)) {
			children.push_back(new AST(Token(curr)));
			curr = next_token();
		}
		else if (match_type(curr, TokenType::Identifier)) {
			children.push_back(new AST(Token(curr)));
			curr = next_token();
		}
		else {
			throw curr;
			break;
		}

		parsed_one = true;
	}

	if (parsed_one) {
		AST* new_tree = new AST("OutputExpr");
		AST* result = build_io_tree(children);

		std::vector<AST*> child = { result };
		new_tree->add_children(child);

		std::cout << peek() << '\n';

		return new_tree;
	}

	return nullptr;
}

AST* Parser::build_io_tree(std::vector<AST*> trees) {
	std::stack<AST*> ops;
	std::stack<AST*> exprs;

	std::vector<TokenType> accepted_ops{ TokenType::RightShift,TokenType::LeftShift };

	for (int i = 0; i < trees.size(); i++) {
		auto root = trees[i]->get_root();

		if (root.index() == 1) {
			Token root_value = std::get<Token>(root);

			if (match_one_of(root_value, accepted_ops)) {
				ops.push(trees[i]);
			}
			else {
				exprs.push(trees[i]);
			}
		}
		else {
			exprs.push(trees[i]);
		}
	}

	while (!ops.empty()) {
		AST* ops_top = ops.top();
		Token root_value = std::get<Token>(ops_top->get_root());

		ops.pop();

		std::vector<AST*> children;

		AST* child1 = exprs.top();
		exprs.pop();
		AST* child2 = exprs.top();
		exprs.pop();

		children.push_back(child2);
		children.push_back(child1);
		ops_top->add_children(children);

		exprs.push(ops_top);
	}

	AST* result = exprs.top();

	return result;
}

AST* Parser::parse_for_expr() {
	Token curr = peek();
	std::vector<AST*> children;

	if (!match_type(curr, TokenType::For)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	if (!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	AST* init_expr = parse_decl_assignment_expr();

	if (init_expr == nullptr) {
		init_expr = parse_simple_assignment_expr();

		if (init_expr != nullptr) {
			children.push_back(init_expr);
		}
	}
	else {
		children.push_back(init_expr);
	}

	curr = peek();
	if (!match_type(curr, TokenType::Semicolon)) {
		throw curr;
	}
	curr = next_token();

	AST* cond_expr = parse_logical_expr();

	if (cond_expr == nullptr) {
		throw curr;
	}
	else {
		children.push_back(cond_expr);
	}

	curr = peek();
	if (!match_type(curr, TokenType::Semicolon)) {
		throw curr;
	}
	curr = next_token();

	AST* incr_expr = parse_incr_decr_expr();
	if (incr_expr == nullptr) {
		incr_expr = parse_simple_assignment_expr();

		if (incr_expr != nullptr) {
			children.push_back(incr_expr);
		}
	}
	else {
		children.push_back(incr_expr);
	}

	curr = peek();

	if (!match_type(curr, TokenType::RightParen)) {
		throw curr;
	}
	curr = next_token();

	if (!match_type(curr, TokenType::LeftBrace)) {
		throw curr;
	}
	curr = next_token();

	AST* body = parse_braces_body("ForBody");

	if (body != nullptr) {
		children.push_back(body);
	}
	else {
		throw curr;
	}

	AST* result = new AST("ForExpr");
	result->add_children(children);

	return result;
}

AST* Parser::parse_class_definition_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	std::vector<TokenType> accepted_types{TokenType::Class,TokenType::Struct };

	if (!match_one_of(curr, accepted_types)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (match_type(curr, TokenType::Semicolon)) {
		AST* result = new AST("ClassDeclExpr");
		result->add_children(nodes);

		return result;
	}
	else if (match_type(curr, TokenType::LeftBrace)) {
		next_token();
		AST* body = parse_class_body();

		if (body != nullptr) {
			nodes.push_back(body);
		}
		else {
			curr = peek();
			throw curr;
		}

		AST* result = new AST("ClassDefExpr");
		result->add_children(nodes);

		return result;
	}

	throw curr;
}

// after parsed body token is the one after }
AST* Parser::parse_class_body() {
	Token curr = peek();
	std::vector<AST*> children;

	while (!match_type(curr, TokenType::RightBrace)) {
		AST* node = nullptr;

		curr = peek();

		if (current_token == tokens.size() - 1) {
			throw curr;
			break;
		}

		if (match_type(curr, TokenType::NewLine)) {
			next_token();
			continue;
		}
		
		try { node = parse_access_specifier_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			//next_token();
		}

		try { node = parse_class_definition_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			//next_token();
		}

		try { node = parse_var_declaration_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			//next_token();
		}

		try { node = parse_func_definition_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			//next_token();
		}

		try { node = parse_class_constructor_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			next_token();
		}

		try { node = parse_class_destructor_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			next_token();
		}
	}

	if (match_type(curr, TokenType::RightBrace)) {
		next_token();
	}

	AST* result = new AST("ClassBody");
	result->add_children(children);

	return result;
}

AST* Parser::parse_access_specifier_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	if (!match_type(curr, TokenType::AccessSpecifier)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::Colon)) {
		throw curr;
	}
	curr = next_token();

	AST* result = new AST("AccessSpecExpr");
	result->add_children(nodes);

	return result;
}

AST* Parser::parse_class_constructor_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	// the expression isn't a constructor
	if (current_token + 1 > tokens.size() - 1
		|| tokens[current_token].type != TokenType::Identifier
		|| tokens[current_token + 1].type != TokenType::LeftParen) {
		return nullptr;
	}

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	AST* args = new AST("Arguments");

	if (match_type(curr, TokenType::RightParen)) {
		curr = next_token();
	}
	else {
		std::vector<AST*> args_list = parse_func_args_expr();
		args->add_children(args_list);

		curr = peek();

		if (!match_type(curr, TokenType::RightParen)) {
			throw curr;
		}
		curr = next_token();
	}

	nodes.push_back(args);

	if (match_type(curr, TokenType::LeftBrace)) {
		curr = next_token();
		
		while (curr.type != TokenType::RightBrace) {

			if (match_type(curr, TokenType::NewLine)) {
				curr = next_token();
				continue;
			}

			AST* node = parse_simple_assignment_expr();

			if (node != nullptr) {
				curr = peek();
				if (match_type(curr, TokenType::Semicolon)) {
					nodes.push_back(node);
					curr = next_token();
				}
				else {
					throw curr;
					exit(-1);
				}
			}
		}

		AST* result = new AST("ClassConstrExpr");
		result->add_children(nodes);

		return result;
	}

	throw curr;
}

AST* Parser::parse_class_destructor_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	if (!match_type(curr, TokenType::Tilde)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	if (!match_type(curr, TokenType::RightParen)) {
		throw curr;
	}
	curr = next_token();

	if (!match_type(curr, TokenType::LeftBrace)) {
		throw curr;
	}
	curr = next_token();

	while (curr.type != TokenType::RightBrace) {
		if (match_type(curr, TokenType::NewLine)) {
			curr = next_token();
			continue;
		}

		AST* node = parse_delete_expr();

		if (node == nullptr) {
			throw curr;
			exit(-1);
		}
		else {
			nodes.push_back(node);
			curr = next_token();
		}
	}

	AST* result = new AST("ClassDestrExpr");
	result->add_children(nodes);

	return result;
}

AST* Parser::parse_delete_expr() {
	Token curr = peek();
	std::vector<AST*> nodes;

	if (!match_type(curr, TokenType::Delete)) {
		return nullptr;
	}
	nodes.push_back(new AST(Token(curr)));
	curr = next_token();

	if (match_type(curr, TokenType::BracketOperator)) {
		nodes.push_back(new AST(Token(curr)));
		curr = next_token();
	}

	if (!match_type(curr, TokenType::Identifier)) {
		throw curr;
	}
	curr = next_token();

	if (!match_type(curr, TokenType::Semicolon)) {
		throw curr;
	}
	curr = next_token();

	AST* result = new AST("DeleteExpr");
	result->add_children(nodes);

	return result;
}

// after parsed body token is the one after }
AST* Parser::parse_braces_body(const std::string& str) {
	Token curr = peek();
	std::vector<AST*> children;

	std::cout << "in braces " << peek() << '\n';

	while (!match_type(curr, TokenType::RightBrace)) {
		AST* node = nullptr;
		
		curr = peek();

		if (current_token == tokens.size() - 1) {
			throw curr;
			break;
		}

		if (match_type(curr, TokenType::NewLine)) {
			next_token();
			continue;
		}

		try { node = parse_output_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			children.push_back(node);
			next_token();

			std::cout << peek();
		}

		try { node = parse_input_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			children.push_back(node);
			next_token();
		}

		try { node = parse_return_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			children.push_back(node);
			next_token();
		}

		try { node = parse_var_declaration_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
		}

		try { node = parse_decl_assignment_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			children.push_back(node);
			next_token();
		}

		try { node = parse_simple_assignment_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			children.push_back(node);
			next_token();
		}

		try { node = parse_if_else_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			//next_token();
		}

		try { node = parse_for_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			//next_token();
		}

		try { node = parse_func_call_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			next_token();
		}

		try { node = parse_func_definition_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			//next_token();
		}

		try { node = parse_class_definition_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (node != nullptr) {
			children.push_back(node);
			next_token();
		}
	}

	curr = peek();

	std::cout << peek() << '\n';

	if (curr.type == TokenType::RightBrace) {
		next_token();
	}
	else {
		throw curr;
		exit(-1);
	}

	AST* result = new AST(str);
	result->add_children(children);

	return result;
}

AST* Parser::parse_incr_decr_expr() {
	Token curr = peek();
	std::vector<AST*> children;

	bool prefix = false;
	bool plus = false;

	if (match_type(curr, TokenType::Identifier)) {
		children.push_back(new AST(Token(curr)));
	}
	else if (match_type(curr, TokenType::PlusPlus)) {
		children.push_back(new AST(Token(curr)));
		prefix = true;
		plus = true;
	}
	else if (match_type(curr, TokenType::MinusMinus)) {
		children.push_back(new AST(Token(curr)));
		prefix = true;
	}
	else {
		return nullptr;
	}

	curr = next_token();

	if (prefix) {
		if (!match_type(curr, TokenType::Identifier)) {
			throw curr;
		}
		else {
			children.push_back(new AST(Token(curr)));
		}
	}
	else if (match_type(curr, TokenType::PlusPlus)) {
		children.push_back(new AST(Token(curr)));
		plus = true;
		next_token();
	}
	else if (match_type(curr, TokenType::MinusMinus)) {
		children.push_back(new AST(Token(curr)));
		next_token();
	}
	else {
		throw curr;
	}

	AST* result;
	if (plus) {
		result = new AST("IncrExpr");
	}
	else {
		result = new AST("DecrExpr");
	}

	result->add_children(children);

	return result;
}

AST* Parser::parse_while_expr() {
	Token curr = peek();
	std::vector<AST*> children;

	if (!match_type(curr, TokenType::While)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	std::cout << "parsing while\n";

	if (!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	AST* cond_expr = parse_logical_expr();

	if (cond_expr == nullptr) {
		throw curr;
	}
	else {
		children.push_back(cond_expr);
	}

	curr = peek();

	if (!match_type(curr, TokenType::RightParen)) {
		throw curr;
	}
	curr = next_token();

	if (!match_type(curr, TokenType::LeftBrace)) {
		throw curr;
	}
	curr = next_token();

	AST* body = parse_braces_body("WhileBody");

	if (body != nullptr) {
		children.push_back(body);
	}
	else {
		throw curr;
	}

	AST* result = new AST("WhileExpr");
	result->add_children(children);

	return result;
}

AST* Parser::parse_if_else_expr() {
	Token curr = peek();

	std::vector<AST*> children;
	std::vector<AST*> if_children;
	std::vector<AST*> else_if_children;
	std::vector<AST*> else_children;

	bool has_else_if = false;
	bool has_else = false;

	if (!match_type(curr, TokenType::If)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	if (!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	AST* cond_expr = parse_logical_expr();

	if (cond_expr == nullptr) {
		throw curr;
	}
	else {
		if_children.push_back(cond_expr);
	}

	curr = peek();
	if (!match_type(curr, TokenType::RightParen)) {
		throw curr;
	}
	curr = next_token();

	if (!match_type(curr, TokenType::LeftBrace)) {
		throw curr;
	}
	curr = next_token();

	AST* body = parse_braces_body("IfBody");

	if (body != nullptr) {
		if_children.push_back(body);
	}
	else {
		throw curr;
	}

	curr = peek();
	if(match_type(curr, TokenType::NewLine)) {
		curr = next_token();
	}

	AST* else_if = parse_else_if();
	bool parsed_one = false;
	
	while (else_if != nullptr) {
		curr = peek();

		if (current_token == tokens.size() - 1) {
			throw curr;
			break;
		}

		if (match_type(curr, TokenType::NewLine)) {
			next_token();
			continue;
		}

		parsed_one = true;
		has_else_if = true;
		else_if_children.push_back(else_if);

		else_if = parse_else_if();
	}

	if (parsed_one) {
		children.push_back(new AST("ElseIfExprs"));
	}

	curr = peek();

	if (match_type(curr, TokenType::NewLine)) {
		curr = next_token();
	}

	if (match_type(curr, TokenType::Else)) {
		has_else = true;

		children.push_back(new AST(curr));
		curr = next_token();

		if (match_type(curr, TokenType::LeftBrace)) {
			curr = next_token();

			AST* body2 = parse_braces_body("ElseBody");

			if (body2 != nullptr) {
				else_children.push_back(body2);
			}
			else {
				throw curr;
			}
		}
		else {
			throw curr;
		}
	}

	AST* result = new AST("IfElseExpr");
	result->add_children(children);

	result->get_children()[0]->add_children(if_children);
	if (has_else_if && has_else) {
		result->get_children()[1]->add_children(else_if_children);
		result->get_children()[2]->add_children(else_children);
	}
	else if (!has_else_if && has_else) {
		result->get_children()[1]->add_children(else_children);
	}

	return result;
}

AST* Parser::parse_else_if() {
	Token curr = peek();
	std::vector<AST*> children;

	// check if its an else if expression
	if (current_token + 1 > tokens.size() - 1 ||
		tokens[current_token].type != TokenType::Else ||
		tokens[current_token + 1].type != TokenType::If) {
		return nullptr;
	}

	if (!match_type(curr, TokenType::Else)) {
		throw curr;
	}
	children.push_back(new AST(Token(curr)));
	curr = next_token();

	if (!match_type(curr, TokenType::If)) {
		throw curr;
	}
	children.push_back(new AST(Token(curr)));
	curr = next_token();

	if(!match_type(curr, TokenType::LeftParen)) {
		throw curr;
	}
	curr = next_token();

	AST* cond_expr = parse_logical_expr();

	if (cond_expr == nullptr) {
		throw curr;
	}
	else {
		children.push_back(cond_expr);
	}

	curr = peek();
	if (!match_type(curr, TokenType::RightParen)) {
		throw curr;
	}
	curr = next_token();

	if (!match_type(curr, TokenType::LeftBrace)) {
		throw curr;
	}
	curr = next_token();

	AST* body = parse_braces_body("ElseIfBody");

	if (body != nullptr) {
		children.push_back(body);
	}
	else {
		throw curr;
	}

	AST* result = new AST("ElseIfExpr");
	result->add_children(children);

	return result;
}

AST* Parser::parse_return_expr() {
	Token curr = peek();
	std::vector<AST*> children;

	if (!match_type(curr, TokenType::Return)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	if (match_type(curr, TokenType::Identifier)) {
		children.push_back(new AST(curr));
		curr = next_token();
	}
	else {
		AST* expr = parse_arithmetic_expr();

		if (expr == nullptr) {
			expr = parse_logical_expr();
		}

		if (expr == nullptr) {
			expr = parse_string_expr();
		}

		if (expr != nullptr) {
			children.push_back(expr);
		}
		else {
			throw curr;
		}
	}

	AST* result = new AST("ReturnExpr");
	result->add_children(children);

	return result;
}

AST* Parser::parse_line_comment() {
	Token curr = peek();
	std::vector<AST*> children;

	if (!match_type(curr, TokenType::LineComment)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	while (!match_type(curr, TokenType::NewLine)) {
		children.push_back(new AST(curr));
		curr = next_token();
	}

	AST* result = new AST("LineComment");
	result->add_children(children);

	return result;
}

AST* Parser::parse_multiline_comment() {
	Token curr = peek();
	std::vector<AST*> children;

	if (!match_type(curr, TokenType::MultilineCommentStart)) {
		return nullptr;
	}
	children.push_back(new AST(curr));
	curr = next_token();

	while (!match_type(curr, TokenType::MultilineCommentEnd)) {
		children.push_back(new AST(curr));
		curr = next_token();
	}

	AST* result = new AST("MultilineComment");
	result->add_children(children);

	return result;
}

void Parser::parse_code(AST* tree) {
	while (!finished_parsing()) {
		AST* new_node = nullptr;

		Token curr = peek();

		std::cout << curr << '\n';

		if (curr.type == TokenType::NewLine) {
			next_token();
			continue;
		}

		try { new_node = parse_line_comment(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_multiline_comment(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_include_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_using_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_var_declaration_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			//next_token();
		}

		try { new_node = parse_decl_assignment_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_simple_assignment_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_func_definition_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
		}

		try { new_node = parse_func_call_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_input_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_output_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_for_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			//next_token();
		}

		try { new_node = parse_while_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			//next_token();
		}

		try { new_node = parse_if_else_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
		}

		try { new_node = parse_return_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			curr = peek();
			if (curr.type != TokenType::Semicolon) {
				std::cout << "error on " << curr.line << ' ' << curr.column; exit(-1);
			}

			tree->add_children(std::vector<AST*> {new_node});
			next_token();
		}

		try { new_node = parse_class_definition_expr(); }
		catch (Token token) { std::cout << "error on " << token.line << ' ' << token.column; exit(-1); }

		if (new_node != nullptr) {
			tree->add_children(std::vector<AST*> {new_node});
			//next_token();
		}
	}
}