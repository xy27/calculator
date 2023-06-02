#include "Exception.h"

#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

void Exception::FillStackTrace(bool demangle)
{
/*
	const int len = 200;
	void* buffer[len];
	int nptrs = ::backtrace(buffer, len);
	char** strings = ::backtrace_symbols(buffer, nptrs);
	if (strings)
	{
		for (int i = 0; i < nptrs; ++i)
		{
			stackTrace_.append(strings[i]);
			stackTrace_.push_back('\n');
		}
		free(strings);
	}
*/

	// 好像没什么效果？
	const int max_frames = 200;
	void* frame[max_frames];
	int nptrs = ::backtrace(frame, max_frames);
	char** strings = ::backtrace_symbols(frame, nptrs);
	if (strings)
	{
		size_t len = 256;
		char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
		for (int i = 1; i < nptrs; ++i)  // skipping the 0-th, which is this function
		{
			if (demangle)
			{
				// https://panthema.net/2008/0901-stacktrace-demangled/
				// bin/exception_test(_ZN3Bar4testEv+0x79) [0x401909]
				char* left_par = nullptr;
				char* plus = nullptr;
				for (char* p = strings[i]; *p; ++p)
				{
					if (*p == '(')
						left_par = p;
					else if (*p == '+')
						plus = p;
				}

				if (left_par && plus)
				{
					*plus = '\0';
					int status = 0;
					char* ret = abi::__cxa_demangle(left_par+1, demangled, &len, &status);
					*plus = '+';
					if (status == 0)
					{
						demangled = ret;  // ret could be realloc()
						stackTrace_.append(strings[i], left_par+1);
						stackTrace_.append(demangled);
						stackTrace_.append(plus);
						stackTrace_.push_back('\n');
						continue;
					}
				}
			}
			// Fallback to mangled names
			stackTrace_.append(strings[i]);
			stackTrace_.push_back('\n');
		}
		free(demangled);
		free(strings);
	}
}