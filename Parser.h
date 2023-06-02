#ifndef _PARSER_H_
#define _PARSER_H_

#include <memory>

class Scanner;
class Node;
class Calc;

enum STATUS
{
	STATUS_OK,
	STATUS_ERROR,
	STATUS_QUIT
};

class Parser
{
public:
	Parser(Scanner& scanner, Calc& calc);
	~Parser();
	// ~Parser() = default;		// expr_析构时，delete p, Node的完整定义还不知道

	STATUS Parse();
	double Calculate() const;
private:
	std::unique_ptr<Node> Expr();
	std::unique_ptr<Node> Term();
	std::unique_ptr<Node> Factor();

	Scanner& scanner_;
	Calc& calc_;
	std::unique_ptr<Node> expr_;
	STATUS status_;
};

#endif // _PARSER_H_