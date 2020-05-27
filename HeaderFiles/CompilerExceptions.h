#pragma once

class CompilerExc : public std::exception
{
protected:
	std::string str;
public:
	CompilerExc(std::string str)
	{
		this->str = str;
	}

	const char* what() const throw ()
	{
		return str.c_str();
	}
};

class NotExpectedExc : public CompilerExc
{
public:
	NotExpectedExc(std::string str) :CompilerExc(str + " isn't expected here.")
	{

	}
};
class ExpectedExc : public CompilerExc
{
public:
	ExpectedExc(std::string str) :CompilerExc(str + " is expected here.")
	{

	}
};