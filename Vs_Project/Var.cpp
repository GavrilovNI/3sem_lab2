#include "Var.h"

using namespace std;

Var* Var::Assign(string v)
{
	if (v[0] == '\'')
	{
		v.erase(0, 1);
		int f = 0;
		if (((f = v.find("'", f)) != string::npos) && (v[f] == v.back()))
		{
			v.erase(f, 1);
		}
		else
		{
			throw "what a twist!";
		}

		_String* temp = new _String(v);
		return (Var*)temp;
	}

	if (v == "false")
	{
		_Bool* temp = new _Bool(false);
		return (Var*)temp;
	}
	if (v == "true")
	{
		_Bool* temp = new _Bool(true);
		return (Var*)temp;
	}

	try
	{
		if (v.find('.', 1) != -1)
		{
			_Double* temp = new _Double(stod(v));
			return (Var*)temp;
		}

		_Int* temp = new _Int(stoi(v));
		return (Var*)temp;
	}
	catch (int)
	{
		throw "what a twist!";
	}
}

_Type Var::GetType()
{
	return _type;
}

_Int::_Int(int v)
{
	value = v;
	_type = _Type::_int;
}

_Int::_Int()
{
	value = 0;
	_type = _Type::_int;
}

_Double::_Double(double v)
{
	value = v;
	_type = _Type::_double;
}

_Double::_Double()
{
	value = .0;
	_type = _Type::_double;
}

_Bool::_Bool(bool v)
{
	value = v;
	_type = _Type::_bool;
}

_Bool::_Bool(int v)
{
	value = v;
	_type = _Type::_bool;
}

_Bool::_Bool()
{
	value = false;
	_type = _Type::_bool;
}

_String::_String(std::string v)
{
	value = v;
	_type = _Type::_string;
}

_String::_String()
{
	value = ' ';
	_type = _Type::_string;
}