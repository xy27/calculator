#ifndef _SCANNER_H_
#define _SCANNER_H_
#include <string>

enum EToken
{
	TOKEN_COMMAND,
	TOKEN_END,
	TOKEN_ERROR,
	TOKEN_NUMBER,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_MULTIPLY,
	TOKEN_DIVIDE,
	TOKEN_LPARENTHESIS,
	TOKEN_RPARENTHESIS,
	TOKEN_IDENTIFIER,
	TOKEN_ASSIGN
};

class Scanner
{
public:
	explicit Scanner(std::istream& in);
	~Scanner() = default;

	void Accept();
	void AcceptCommand();
	bool IsEmpty() const { return isEmpty_; }
	bool IsDone() const { return token_ == TOKEN_END; }
	bool IsCommand() const { return token_ == TOKEN_COMMAND; }
	double Number() const { return number_; }
	std::string GetSymbol() const { return symbol_; }
	EToken Token() const { return token_; }
private:
	void ReadChar();
	
	std::istream& in_;
	int look_;
	EToken token_;
	double number_;
	std::string symbol_;
	bool isEmpty_;
};

#endif // _SCANNER_H_
