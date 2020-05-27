#pragma once
#include <string>



class Var
{
public:
	static const std::string typeName[static_cast<int>(5)];

	enum class _Type
	{
		_int, _double, _bool, _string, _void
	};

	static Var* Assign(std::string);
	_Type GetType();
	static _Type GetTypeByString(std::string);
	static std::string GetTypeName(_Type);

	//virtual Var operator+ (Var& v);

	Var* operator+ (Var*);
	Var* operator- (Var*);
	Var* operator* (Var*);
	Var* operator/ (Var*);
	Var* operator% (Var*);
	Var* operator&& (Var*);
	Var* operator|| (Var*);
	Var* operator! ();

protected:
	_Type _type;
};

class _Int : Var
{
public:
	int value;

	_Int(int v);
	_Int();

	_Int operator+ (_Int v)
	{
		return _Int(this->value + v.value);
	}

	/*Var operator+ (Var& v) override
	{
		return (Var)&(*this + *v);
	}*/

	_Int operator- (_Int v)
	{
		return _Int(this->value - v.value);
	}

	_Int operator* (_Int v)
	{
		return _Int(this->value * v.value);
	}

	_Int operator/ (_Int v)
	{
		return _Int(this->value / v.value);
	}

	_Int operator% (_Int v)
	{
		return _Int(this->value % v.value);
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

	_Double(double v);
	_Double(int v);
	_Double();


	_Double operator+ (_Double v)
	{
		return _Double(this->value + v.value);
	}

	_Double operator+ (_Int v)
	{
		return _Double(this->value + v.value);
	}

	_Double operator- (_Double v)
	{
		return _Double(this->value - v.value);
	}

	_Double operator- (_Int v)
	{
		return _Double(this->value - v.value);
	}

	_Double operator* (_Double v)
	{
		return _Double(this->value * v.value);
	}

	_Double operator* (_Int v)
	{
		return _Double(this->value * v.value);
	}

	_Double operator/ (_Double v)
	{
		return _Double(this->value / v.value);
	}

	_Double operator/ (_Int v)
	{
		return _Double(this->value / v.value);
	}


	explicit operator double() const
	{
		return value;
	}
};

/*
_Double operator+ (_Int v1, _Double v2)
{
	return _Double(v1.value + v2.value);
}

_Double operator- (_Int v1, _Double v2)
{
	return _Double(v1.value - v2.value);
}

_Double operator* (_Int v1, _Double v2)
{
	return _Double(v1.value * v2.value);
}

_Double operator/ (_Int v1, _Double v2)
{
	return _Double(v1.value / v2.value);
}
*/

class _Bool : Var
{
public:
	bool value;

	_Bool(bool v);
	_Bool(int v);
	_Bool();


	_Bool operator&& (_Bool v)
	{
		return _Bool(this->value && v.value);
	}

	_Bool operator|| (_Bool v)
	{
		return _Bool(this->value || v.value);
	}

	_Bool operator! ()
	{
		return _Bool(!(this->value));
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

	_String(std::string v);
	_String();


	_String operator+ (_String v)
	{
		return _String(this->value + v.value);
	}


	explicit operator std::string() const
	{
		return value;
	}
};





/*
Var* operator+ (Var& v)
	{
		switch (this->_type)
		{
		case _Type::_int:
			switch (v._type)
			{
			case _Type::_int:
				return (Var*)&(*(_Int*)this + *(_Int*)&v);
				break;
			case _Type::_double:
				this->_type = _Type::_double;
				return (Var*)&(*(_Double*)&v + *(_Int*)this);
				break;
			default:
				throw "Incompatible types";
				break;
			}
			break;

		case _Type::_double:
			switch (v._type)
			{
			case _Type::_int:
				return (Var*)&(*(_Double*)this + *(_Int*)&v);
				break;
			case _Type::_double:
				return (Var*)&(*(_Double*)this + *(_Double*)&v);
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
*/