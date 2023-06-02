#ifndef _FUNCTION_TABLE_H_
#define _FUNCTION_TABLE_H_

#include <functional>
#include <array>

class SymbolTable;

const int FUNC_SIZE = 13;
typedef std::function<double(double)> Func;

class FunctionTable
{
public:
	FunctionTable(SymbolTable& tbl);
	~FunctionTable() = default;

	unsigned int Size() const { return FUNC_SIZE; }
	Func GetFunction(unsigned int id) const { return funcTbl_[id]; }
private:
	void Init(SymbolTable& tbl);

	std::array<Func, FUNC_SIZE> funcTbl_;
};

#endif // _FUNCTION_TABLE_H_