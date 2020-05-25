#pragma once
#include <string>

enum  _Type
{
	_int, _double, _bool, _string, const_int, const_double, const_bool, const_string, count
};

class Var
{
protected:
	_Type type;
public:
	//convert to constructor!
	void SetType(_Type t)
	{
		type = t;
	}
	static Var* Assign(std::string v);
	_Type GetType();
};

class _Int : public Var
{
public:
	int value;
	_Int(int v)
	{
		value = v;
		type = _int;
	}
	_Int()
	{
		value = 0;
		type = _int;
	}
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
	_Double(double v)
	{
		value = v;
		type = _double;
	}
	_Double()
	{
		value = .0;
		type = _double;
	}
	explicit operator double() const
	{
		return value;
	}
};

class _Bool : Var
{
public:
	bool value;
	_Bool(bool v)
	{
		value = v;
		type = _bool;
	}
	_Bool()
	{
		value = false;
		type = _bool;
	}
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
	_String(std::string v)
	{
		value = v;
		type = _string;
	}
	_String()
	{
		value = ' ';
		type = _string;
	}
	explicit operator std::string() const
	{
		return value;
	}
};