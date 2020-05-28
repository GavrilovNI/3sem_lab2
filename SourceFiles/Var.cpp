#include "..\HeaderFiles\Var.h"

using namespace std;

const std::string Var::typeName[static_cast<int>(5)] = {"integer", "double", "boolean", "string" , "void"};

Var* Var::CreateCopy(Var* v)

{
	if (v->GetType() == _Type::_int)
		return new _Int(dynamic_cast<_Int*>(v)->value);
	if (v->GetType() == _Type::_double)
		return new _Double(dynamic_cast<_Double*>(v)->value);
	if (v->GetType() == _Type::_bool)
		return new _Bool(dynamic_cast<_Bool*>(v)->value);
	if (v->GetType() == _Type::_string)
		return new _String(dynamic_cast<_String*>(v)->value);

	throw "unidentified type";
}

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
		if (v[i] == '.')
		{
			count++;
		}
	}

	if ((count == 1) && (v.back() != '.'))
	{
		_Double* temp = new _Double(stod(v));
		return (Var*)temp;
	}

	for (int i = 0; i < v.length(); i++)
	{
		if (((v[i] < '0') || (v[i] > '9')) && (v[i] != '-'))
			throw ("unidentified symbol - '" + v.substr(i, 1) + "'");
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

std::string Var::GetTypeName(_Type t)
{
	return typeName[static_cast<int>(t)];
}

Var* Var::CreateVarByType(_Type t)
{
	switch (t)
	{
	case _Type::_int:
		return new _Int();
	case _Type::_double:
		return new _Double();
	case _Type::_bool:
		return new _Bool();
	case _Type::_string:
		return new _String();
	}
	throw "an error";
}

Var* _Int::operator+ (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Int(value + static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Double(value + static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator- (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Int(value - static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Double(value - static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator* (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Int(value * static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Double(value * static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator/ (Var* v)
{
	if (v->GetType() == _Type::_int)
	{
		if (static_cast<_Int*>(v)->value == 0)
			throw "Zero division error";
		return new _Int(value / static_cast<_Int*>(v)->value);
	}
	else if (v->GetType() == _Type::_double)
	{
		if (static_cast<_Double*>(v)->value == 0)
			throw "Zero division error";
		return new _Double(value / static_cast<_Double*>(v)->value);
	}
	else
		throw "Incompatible types";
}

Var* _Int::operator% (Var* v)
{
	if (v->GetType() == _Type::_int)
	{
		if (static_cast<_Int*>(v)->value == 0)
			throw "Zero division error";
		return new _Int(value % static_cast<_Int*>(v)->value);
	}
	else
		throw "Incompatible types";
}

Var* _Int::operator== (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value == static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value == static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator!= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value != static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value != static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}


Var* _Int::operator< (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value < static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value < static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator<= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value <= static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value <= static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator> (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value > static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value > static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator>= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value >= static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value >= static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Int::operator= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Int(static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_bool)
		return new _Int(static_cast<_Bool*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator+ (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Double(value + static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Double(value + static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator- (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Double(value - static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Double(value - static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator* (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Double(value * static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Double(value * static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator/ (Var* v)
{
	if (v->GetType() == _Type::_int)
	{
		if (static_cast<_Int*>(v)->value == 0)
			throw "Zero division error";
		return new _Double(value / static_cast<_Int*>(v)->value);
	}
	else if (v->GetType() == _Type::_double)
	{
		if (static_cast<_Double*>(v)->value == 0)
			throw "Zero division error";
		return new _Double(value / static_cast<_Double*>(v)->value);
	}
	else
		throw "Incompatible types";
}

Var* _Double::operator== (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value == static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value == static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator!= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value != static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value != static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator< (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value < static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value < static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator<= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value <= static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value <= static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator> (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value > static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value > static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator>= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(value >= static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_double)
		return new _Bool(value >= static_cast<_Double*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Double::operator= (Var* v)
{
	if (v->GetType() == _Type::_double)
		return new _Double(static_cast<_Double*>(v)->value);
	else if (v->GetType() == _Type::_int)
		return new _Double(static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_bool)
		return new _Double(static_cast<_Bool*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Bool::operator&& (Var* v)
{
	if (v->GetType() == _Type::_bool)
		return new _Bool(value && static_cast<_Bool*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Bool::operator|| (Var* v)
{
	if (v->GetType() == _Type::_bool)
		return new _Bool(value || static_cast<_Bool*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Bool::operator== (Var* v)
{
	if (v->GetType() == _Type::_bool)
		return new _Bool(value == static_cast<_Bool*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Bool::operator!= (Var* v)
{
	if (v->GetType() == _Type::_bool)
		return new _Bool(value != static_cast<_Bool*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _Bool::operator!()
{
	return new _Bool(!value);
}

Var* _Bool::operator= (Var* v)
{
	if (v->GetType() == _Type::_int)
		return new _Bool(static_cast<_Int*>(v)->value);
	else if (v->GetType() == _Type::_bool)
		return new _Bool(static_cast<_Bool*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _String::operator+ (Var* v)
{
	if (v->GetType() == _Type::_string)
		return new _String(value + static_cast<_String*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _String::operator== (Var* v)
{
	if (v->GetType() == _Type::_string)
		return new _Bool(value == static_cast<_String*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _String::operator!= (Var* v)
{
	if (v->GetType() == _Type::_string)
		return new _Bool(value != static_cast<_String*>(v)->value);
	else
		throw "Incompatible types";
}

Var* _String::operator= (Var* v)
{
	if (v->GetType() == _Type::_string)
		return new _String(static_cast<_String*>(v)->value);
	else
		throw "Incompatible types";
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
	value = "";
	_type = _Type::_string;
}