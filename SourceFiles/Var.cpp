#include "..\Header Files\Var.h"

using namespace std;

const std::string Var::typeName[static_cast<int>(5)] = {"integer", "double", "boolean", "string" , "void"};

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

Var* Var::operator+(Var* v)
{
	switch (this->_type)
	{
	case _Type::_int:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_int;
			return (Var*)(&(*(_Int*)this + *(_Int*)v));
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&_Double((*(_Int*)this).value + (*(_Double*)v).value);
			break;
		default:
			throw "Incompatible types";
			break;
		}
		break;

	case _Type::_double:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this + *(_Int*)v);
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this + *(_Double*)v);
			break;
		default:
			throw "Incompatible types";
			break;
		}
	case _Type::_string:
		if (v->_type == _Type::_string)
			return (Var*)&(*(_String*)this + *(_String*)v);
		break;
	case _Type::_bool:
		throw "Type boolean doesn't have operator+";
		break;
	case _Type::_void:
		throw "Type void doesn't have operator+";
		break;
	}
}

Var* Var::operator-(Var* v)
{
	switch (this->_type)
	{
	case _Type::_int:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_int;
			return (Var*)(&(*(_Int*)this - *(_Int*)v));
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&_Double((*(_Int*)this).value - (*(_Double*)v).value);
			break;
		default:
			throw "Incompatible types";
			break;
		}
		break;

	case _Type::_double:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this - *(_Int*)v);
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this - *(_Double*)v);
			break;
		default:
			throw "Incompatible types";
			break;
		}
	case _Type::_bool:
		throw "Type boolean doesn't have operator-";
		break;
	case _Type::_string:
		throw "Type string doesn't have operator-";
		break;
	case _Type::_void:
		throw "Type void doesn't have operator-";
		break;
	}
}

Var* Var::operator*(Var* v)
{
	switch (this->_type)
	{
	case _Type::_int:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_int;
			return (Var*)(&(*(_Int*)this * *(_Int*)v));
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)v * *(_Int*)this);
			break;
		default:
			throw "Incompatible types";
			break;
		}
		break;

	case _Type::_double:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this * *(_Int*)v);
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this * *(_Double*)v);
			break;
		default:
			throw "Incompatible types";
			break;
		}
	case _Type::_bool:
		throw "Type boolean doesn't have operator*";
		break;
	case _Type::_string:
		throw "Type string doesn't have operator*";
		break;
	case _Type::_void:
		throw "Type void doesn't have operator*";
		break;
	}
}

Var* Var::operator/(Var* v)
{
	switch (this->_type)
	{
	case _Type::_int:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_int;
			return (Var*)&(*(_Int*)this / *(_Int*)v);
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&_Double(((_Int*)this)->value  / ((_Double*)v)->value);
			break;
		default:
			throw "Incompatible types";
			break;
		}
		break;

	case _Type::_double:
		switch (v->_type)
		{
		case _Type::_int:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this / *(_Int*)v);
			break;
		case _Type::_double:
			_type = _Type::_double;
			return (Var*)&(*(_Double*)this / *(_Double*)v);
			break;
		default:
			throw "Incompatible types";
			break;
		}
	case _Type::_bool:
		throw "Type boolean doesn't have operator+";
		break;
	case _Type::_string:
		throw "Type string doesn't have operator+";
		break;
	case _Type::_void:
		throw "Type void doesn't have operator+";
		break;
	}
}

Var* Var::operator%(Var* v)
{
	if ((this->_type == _Type::_int) && (v->_type == _Type::_int))
	{
		this->_type = _Type::_int;
		return (Var*)&(*(_Int*)this % *(_Int*)v);
	}
	else
	{
		throw "Incompatible types";
	}
}

Var* Var::operator&&(Var* v)
{
	if ((this->_type == _Type::_bool) && (v->_type == _Type::_bool))
	{
		this->_type = _Type::_bool;
		return (Var*)&(*(_Bool*)this && *(_Bool*)v);
	}
	else
	{
		throw "Incompatible types";
	}
}

Var* Var::operator||(Var* v)
{
	if ((this->_type == _Type::_bool) && (v->_type == _Type::_bool))
	{
		this->_type = _Type::_bool;
		return (Var*)&(*(_Bool*)this || *(_Bool*)v);
	}
	else
	{
		throw "Incompatible types";
	}
}

Var* Var::operator!()
{
	if (this->_type == _Type::_bool)
	{
		return (Var*)&(_Bool(!( ( (_Bool*) this) ->value) ) );
	}
	else
	{
		throw "Incompatible types";
	}
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