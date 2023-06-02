#include <cmath>
#include <cassert>
#include <iostream>
#include "Node.h"
#include "Storage.h"
#include "DebugNew.h"

double SumNode::Calc() const
{
	double result = 0.0;

	auto childIt = childs_.begin();
	auto positiveIt = positives_.begin();

	for ( ; childIt != childs_.end(); ++childIt, ++positiveIt)
	{
		assert(positiveIt != positives_.end());
		double val = (*childIt)->Calc();
		if (*positiveIt)
			result += val;
		else
			result -= val;
	}
	assert(positiveIt == positives_.end());

	return result;
}

double ProductNode::Calc() const
{
	double result = 1.0;

	auto childIt = childs_.begin();
	auto positiveIt = positives_.begin();

	for (; childIt != childs_.end(); ++childIt, ++positiveIt)
	{
		assert(positiveIt != positives_.end());
		double val = (*childIt)->Calc();
		if (*positiveIt)
			result *= val;
		else if (val != 0.0)
			result /= val;
		else
		{
			std::cout << "Division by zero." << std::endl;
			return HUGE_VAL;
		}
			
	}
	assert(positiveIt == positives_.end());

	return result;
}

double VariableNode::Calc() const
{
	double x = 0.0;
	if (storage_.IsInit(id_))
	{
		x = storage_.GetValue(id_);
	}
	else
	{
		std::cout << "Use of uninitialized variable." << std::endl;
	}
	return x;
}

void VariableNode::Assign(double val)
{
	storage_.SetValue(id_, val);
}

double AssignNode::Calc() const
{
	double x = 0.0;
	x = right_->Calc();
	left_->Assign(x);
	return x;
}