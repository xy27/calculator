#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	explicit Exception(const char* message)
		: message_(message)
	{
		FillStackTrace(false);
	}
	explicit Exception(const std::string& message)
		: message_(message)
	{
		FillStackTrace(false);
	}
	virtual ~Exception() noexcept override = default;

	virtual const char* what() const noexcept override
	{ return message_.c_str(); }

	const char* StackTrace() const noexcept
	{ return stackTrace_.c_str(); }

private:
	void FillStackTrace(bool demangle);
	std::string message_;
	std::string stackTrace_;
};

class SyntaxError : public Exception
{
public:
	explicit SyntaxError(const char* message)
		: Exception(message)
	{
	}
	explicit SyntaxError(const std::string& message)
		: Exception(message)
	{
	}
	virtual ~SyntaxError() noexcept override = default;
};

class FileStreamError : public Exception
{
public:
	explicit FileStreamError(const char* message)
		: Exception(message)
	{
	}
	explicit FileStreamError(const std::string& message)
		: Exception(message)
	{
	}
	virtual ~FileStreamError() noexcept override = default;
};

#endif // _EXCEPTION_H_
