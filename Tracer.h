﻿#ifndef _TRACER_H_
#define _TRACER_H_

#include <map>

#ifndef NDEBUG

void* operator new(size_t size, const char* file, long line);
void operator delete(void* p, const char*, long);

void* operator new(size_t size);
void operator delete(void* p);

void* operator new[](size_t size, const char* file, long line);
void operator delete[](void* p, const char*, long);

void* operator new[](size_t size);
void operator delete[](void* p);

class Tracer
{
public:
	Tracer();
	~Tracer();
	static bool Ready;

	void Add(void* p, const char* file, long line);
	void Remove(void* p);
	void Dump();

private:
	class Entry
	{
	public:
		Entry(const char* file = nullptr, long line = 0)
			: file_(file), line_(line) {}
		const char* File() const { return file_; }
		long Line() const { return line_; }
	private:
		const char* file_;
		long line_;
	};

	class Lock
	{
	public:
		Lock(Tracer& tracer) : tracer_(tracer)
		{ tracer_.lock(); }
		~Lock()
		{ tracer_.unlock();	}
	private:
		Tracer& tracer_;
	};

	friend class Lock;
	
	void lock() { ++lockCount_; }
	void unlock() { --lockCount_; }

	std::map<void*, Entry> mapEntry_;
	int lockCount_;
};

extern Tracer NewTrace;
#endif // NDEBUG

#endif // _TRACER_H_