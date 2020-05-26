#include "../HeaderFiles/Var.h"

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
			throw "unpair apostrophe";
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

	int count = 0;
	for (int i = 0; i < v.length(); i++)
	{
		//if ((v[i] == '\'') && (v.substr(i, 1) == "'"))
		if ((v[i] == '\'') && (v.substr(i, 1) == "'") || (v[i] == '.') && (v.substr(i, 1) == "."))
		{
			count++;
		}
	}

	//if ((count == 1) && (v.back() != '\''))
	if ((count == 1) && (v.find('.', 1) != -1))
	{
		_Double* temp = new _Double(stod(v));
		return (Var*)temp;
	}

	for (int i = 0; i < v.length(); i++)
	{
		if ((v[i] < '0') || (v[i] > '9'))
			throw "unidentified symbol - '" + v.substr(i, 1) + "'";
	}
	_Int* temp = new _Int(stoi(v));
	return (Var*)temp;
}

Var::_Type Var::GetType()
{
	return _type;
}

Var::_Type Var::GetTypeByString(std::string varName)
{
	if (varName == "integer")
		return _Type::_int;
	if (varName == "double")
		return _Type::_double;
	if (varName == "boolean")
		return _Type::_bool;
	if (varName == "string")
		return _Type::_string;

	throw "unidentified type";
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

_Double::_Double(int v)
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