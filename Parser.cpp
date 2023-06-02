#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include "Parser.h"
#include "Scanner.h"
#include "Calc.h"
#include "Node.h"
#include "Exception.h"
#include "DebugNew.h"

Parser::Parser(Scanner& scanner, Calc& calc)
	: scanner_(scanner), calc_(calc), status_(STATUS_OK)
{
}

Parser::~Parser()
{
}

STATUS Parser::Parse()
{
	expr_ = Expr();				// 解析之前已经读取到了一位非空字符
	if (!scanner_.IsDone())
	{
		status_ = STATUS_ERROR;
	}

	return status_;
}

std::unique_ptr<Node> Parser::Expr()
{
	std::unique_ptr<Node> node = Term();
	EToken token = scanner_.Token();

	if (token == TOKEN_PLUS || token == TOKEN_MINUS)
	{
		// Expr ::= Term { ('+' | '-') Term }
		std::unique_ptr<MultipleNode> multipleNode = std::unique_ptr<SumNode>(new SumNode(node));
		do 
		{
			scanner_.Accept();	// 更新token 准备解析 Term
			std::unique_ptr<Node> nextNode = Term();
			multipleNode->AppendChild(nextNode, (token == TOKEN_PLUS));
			token = scanner_.Token();
		} while (token == TOKEN_PLUS || token == TOKEN_MINUS);
		node = std::move(multipleNode);
	}
	else if (token == TOKEN_ASSIGN)
	{
		// Expr ::= Term = Expr
		scanner_.Accept();	// 更新token 准备解析 Expr
		std::unique_ptr<Node> nodeRight = Expr();

		if (node->IsLvalue())
		{
			node = std::unique_ptr<Node>(new AssignNode(node, nodeRight));
		}
		else
		{
			status_ = STATUS_ERROR;
			throw SyntaxError("The left-hand side of an assignment must be a variable.");
		}
	}

	return node;
}

std::unique_ptr<Node> Parser::Term()
{
	std::unique_ptr<Node> node = Factor();
	EToken token = scanner_.Token();

	if (token == TOKEN_MULTIPLY || token == TOKEN_DIVIDE)
	{
		// Term ::= Factor { ('*' | '/') Factor }
		std::unique_ptr<MultipleNode> multipleNode = std::unique_ptr<ProductNode>(new ProductNode(node));
		do 
		{
			scanner_.Accept();
			std::unique_ptr<Node> nextNode = Factor();
			multipleNode->AppendChild(nextNode, (token == TOKEN_MULTIPLY));
			token = scanner_.Token();
		} while (token == TOKEN_MULTIPLY || token == TOKEN_DIVIDE);
		node = std::move(multipleNode);
	}

	return node;
}

std::unique_ptr<Node> Parser::Factor()
{
	std::unique_ptr<Node> node;
	EToken token = scanner_.Token();
	if (token == TOKEN_LPARENTHESIS)	// '('Expr ')'	这种情况
	{
		scanner_.Accept();		// 解析之前(需要)已经读取到了一位非空字符
		node = Expr();
		if (scanner_.Token() == TOKEN_RPARENTHESIS)
		{
			scanner_.Accept();	// 更新token，为下一次解析做准备
		}
		else
		{
			status_ = STATUS_ERROR;
			throw SyntaxError("Missing parenthesis.");
		}
	}
	else if (token == TOKEN_NUMBER)		 // Number 
	{
		node = std::unique_ptr<Node>(new NumberNode(scanner_.Number()));
		scanner_.Accept();		// 解析完之后，会再读取一位非空字符，更新token
	}
	else if (token == TOKEN_IDENTIFIER)		// IDENTIFIER有两种情况，1.function 2.variable
	{
		std::string symbol = scanner_.GetSymbol();
		unsigned int id = calc_.FindSymbol(symbol);
		scanner_.Accept();

		if (scanner_.Token() == TOKEN_LPARENTHESIS)		// function call
		{
			scanner_.Accept();		// 解析之前(需要)已经读取到了一位非空字符
			node = Expr();
			if (scanner_.Token() == TOKEN_RPARENTHESIS)
			{
				scanner_.Accept();	// 更新token，为下一次解析做准备
				if (id != SymbolTable::IDNOTFOUND && calc_.IsFunction(id))
				{
					node = std::unique_ptr<Node>(new FunctionNode(node, calc_.GetFunction(id)));
				}
				else
				{
					status_ = STATUS_ERROR;
					std::ostringstream oss;
					oss << "Unknown function \"" << symbol << "\".";
					throw SyntaxError(oss.str());
				}
			}
			else
			{
				status_ = STATUS_ERROR;
				throw SyntaxError("Missing parenthesis in a function call.");
			}
		}
		else
		{
			// 解析成为 variable
			// 前面已经有一个scanner_.Accept()了，所以，解析完之后，也会再读取一位非空字符，更新token
			if (id == SymbolTable::IDNOTFOUND)
			{
				id = calc_.AddSymbol(symbol);	// 只把变量符号添加到符号表中
			}
			node = std::unique_ptr<Node>(new VariableNode(id, calc_.GetStorage()));
		}		
	}
	else if (token == TOKEN_MINUS)		// '-'Factor
	{
		scanner_.Accept();		// 更新token
		std::unique_ptr<Node> tmp = Factor();
		node = std::unique_ptr<Node>(new UMinusNode(tmp));
	}
	else
	{
		status_ = STATUS_ERROR;
		throw SyntaxError("Not a valid expression.");
	}
	return node;
}

double Parser::Calculate() const
{
	assert(expr_ != nullptr);
	return expr_->Calc();
}