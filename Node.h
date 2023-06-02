#ifndef _NODE_H_
#define _NODE_H_

#include <cassert>
#include <vector>
#include <memory>
#include "FunctionTable.h"

class Storage;

class Noncopyable
{
protected:
	Noncopyable() = default;
	~Noncopyable() = default;
private:
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};

// abstract class
class Node : private Noncopyable
{
public:
	virtual ~Node() = default;

	virtual double Calc() const = 0;

	// 这两个虚函数有必要吗？
	virtual bool IsLvalue() const { return false; }	// 提供默认实现
	virtual void Assign(double)                     // 提供默认实现
	{
		assert(!"Assign called incorrectlly.");
	}
};

class NumberNode : public Node
{
public:
	NumberNode(double number) : number_(number) {}
	~NumberNode() = default;

	double Calc() const override { return number_; }
private:
	const double number_;
};

// abstract class
class UnaryNode : public Node
{
public:
	UnaryNode(std::unique_ptr<Node>& child)
		: child_(std::move(child)) {}
	~UnaryNode() = default;
protected:
	std::unique_ptr<Node> child_;
};

// abstract class
class BinaryNode : public Node
{
public:
	BinaryNode(std::unique_ptr<Node>& left, std::unique_ptr<Node>& right)
		: left_(std::move(left)), right_(std::move(right)) {}
	~BinaryNode() = default;
protected:
	std::unique_ptr<Node> left_;
	std::unique_ptr<Node> right_;
};

class FunctionNode : public UnaryNode
{
public:
	FunctionNode(std::unique_ptr<Node>& child, const Func& pFun)
		: UnaryNode(child), func_(pFun) {}
	~FunctionNode() = default;

	double Calc() const override { return func_(child_->Calc()); }
private:
	Func func_;
};

class UMinusNode : public UnaryNode
{
public:
	UMinusNode(std::unique_ptr<Node>& child)
		: UnaryNode(child) {}
	~UMinusNode() = default;

	double Calc() const override { return -child_->Calc(); }
};

// abstract class
class MultipleNode : public Node
{
public:
	MultipleNode(std::unique_ptr<Node>& node) { AppendChild(node, true); }
	~MultipleNode() = default;

	void AppendChild(std::unique_ptr<Node>& node, bool positive)
	{
		childs_.emplace_back(std::move(node));
		positives_.push_back(positive);
	}

protected:
	std::vector<std::unique_ptr<Node>> childs_;
	std::vector<bool> positives_;
};

class SumNode : public MultipleNode
{
public:
	SumNode(std::unique_ptr<Node>& node)
		: MultipleNode(node) {}
	~SumNode() = default;

	double Calc() const override;
};

class ProductNode : public MultipleNode
{
public:
	ProductNode(std::unique_ptr<Node>& node)
		: MultipleNode(node) {}
	~ProductNode() = default;

	double Calc() const override;
};

// 变量节点在使用之前，是必须先赋值的(除常量 e, pi除外)
// 因此肯定是先有个赋值节点，然后才可能有变量节点
class VariableNode : public Node
{
public:
	VariableNode(unsigned int id, Storage& storage)
		: id_(id), storage_(storage) {}
	~VariableNode() = default;

	double Calc() const override;
	bool IsLvalue() const override { return true; }
	//void Assign(double val) override { storage_.SetValue(id_, val); }  // 还没有storage_的完整定义
	void Assign(double val) override;
private:
	const unsigned int id_;
	Storage& storage_;
};

class AssignNode : public BinaryNode
{
public:
	AssignNode(std::unique_ptr<Node>& left, std::unique_ptr<Node>& right)
		: BinaryNode(left, right)
	{
		assert(left_->IsLvalue());		// left_一定是变量节点
	}
	~AssignNode() = default;

	double Calc() const override;
};
#endif // _NODE_H_