#pragma once
#include <string>

enum class _Type
{
	_int, _double, _bool, _string, const_int, const_double, const_bool, const_string, count
};

class Var
{
public:
	static Var* Assign(std::string v);
	_Type GetType();

protected:
	_Type _type;
};

class _Int : Var
{
public:
	int value;

	_Int(int v);
	_Int();

	explicit operator int() const
	{
		return value;
	}
	explicit operator double() const
	{
		return value;
	}
	explicit operator bool() const
	{
		if (value != 0)
			return true;
		return false;
	}
};

class _Double : Var
{
public:
	double value;

	_Double(double v);
	_Double();

	explicit operator double() const
	{
		return value;
	}
};

class _Bool : Var
{
public:
	bool value;

	_Bool(bool v);
	_Bool(int v);
	_Bool();

	explicit operator bool() const
	{
		return value;
	}
	explicit operator int() const
	{
		if (value)
			return 1;
		return 0;
	}
};

class _String : Var
{
public:
	std::string value;

	_String(std::string v);
	_String();

	explicit operator std::string() const
	{
		return value;
	}
};
