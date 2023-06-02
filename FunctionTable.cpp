#include <cmath>
#include <cassert>
#include <iostream>
#include "FunctionTable.h"
#include "SymbolTable.h"
//#include "DebugNew.h"

struct FunctionEntry
{
	Func func_;
	const char* funName_;
};

FunctionEntry Entrys[] =
{
	log,	"log",
	log10,	"log10",
	exp,	"exp",
	sqrt,	"sqrt",
	sin,	"sin",
	cos,	"cos",
	tan,	"tan",
	sinh,	"sinh",
	cosh,	"cosh",
	tanh,	"tanh",
	asin,	"asin",
	acos,	"acos",
	atan,	"atan"
};

FunctionTable::FunctionTable(SymbolTable& tbl)
{
	Init(tbl);
}

void FunctionTable::Init(SymbolTable& tbl)
{
	std::cout << "function list:" << std::endl;
	for (unsigned int i=0; i<FUNC_SIZE; ++i)
	{
		funcTbl_[i] = Entrys[i].func_;
		unsigned int j = tbl.Add(Entrys[i].funName_);
		assert(i == j);
		std::cout << Entrys[i].funName_ << std::endl;
	}
}