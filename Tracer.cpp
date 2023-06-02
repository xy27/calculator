#include <iostream>
#include "Tracer.h"

#ifndef NDEBUG

bool Tracer::Ready = false;

Tracer::Tracer() : lockCount_(0)
{
	Ready = true;
}

Tracer::~Tracer()
{
	Ready = false;
	Dump();
}

void Tracer::Add(void* p, const char* file, long line)
{
	if (lockCount_ > 0)		// 没有这个判断，mapEntry_[p] = Entry(file, line); 与operator new(size_t)会递归调用
		return;				// 最终导致栈溢出，有这个判断可以不跟踪标准库的map容器中的new操作

	Tracer::Lock lock(*this);

	// long long addr = reinterpret_cast<long long>(p);
	// std::cout << "ADD   0x" << std::hex << addr << file << " " << std::dec << line << std::endl;

	mapEntry_[p] = Entry(file, line);
}

void Tracer::Remove(void* p)
{
	if (lockCount_ > 0)
		return;

	Tracer::Lock lock(*this);

	auto it = mapEntry_.find(p);
	if (it != mapEntry_.end())
	{
		//int addr = reinterpret_cast<int>(p);
		//std::cout << "REMOVE   0x" << std::hex << addr << it->second.File() << " "
		//	<< std::dec << it->second.Line() << std::endl;

		mapEntry_.erase(it);
	}
}

void Tracer::Dump()
{
	if (mapEntry_.size() > 0)
	{
		std::cout << "!!! Memory leak(s):" << std::endl;

		for (auto it=mapEntry_.begin(); it!=mapEntry_.end(); ++it)
		{
			const char* file = it->second.File();
			long line = it->second.Line();
			long long addr = reinterpret_cast<long long>(it->first);
			std::cout << "0x" << std::hex << addr << ": "
					  << file << ", line " << std::dec << line << std::endl;
		}
		std::cout << std::endl;
	}
	else
	{
		std::cout << "No Memory Leak(s).\n";
	}
}

Tracer NewTrace;

void* operator new(size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, file, line);
	}
	return p;
}

void operator delete(void* p, const char*, long)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}

void* operator new(size_t size)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, "?", 0);
	}
	return p;
}

void operator delete(void* p)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}

void* operator new[](size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, file, line);
	}
	return p;
}

void operator delete[](void* p, const char*, long)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}

void* operator new[](size_t size)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, "?", 0);
	}
	return p;
}

void operator delete[](void* p)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}
#endif // #ifndef NDEBUG
