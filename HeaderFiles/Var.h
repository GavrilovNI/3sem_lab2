#pragma once
#include <string>

class Var
{
public:
	enum class _Type
	{
		_int, _double, _bool, _string, const_int, const_double, const_bool, const_string, _void
	};

	static Var* Assign(std::string v);
	_Type GetType();
	static _Type GetTypeByString(std::string);

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
		return _Double(v.value + this->value);
	}

	_Double operator- (_Double v)
	{
		return _Double(this->value - v.value);
	}

	_Double operator- (_Int v)
	{
		return _Double(v.value - this->value);
	}

	_Double operator* (_Double v)
	{
		return _Double(this->value * v.value);
	}

	_Double operator* (_Int v)
	{
		return _Double(v.value * this->value);
	}

	_Double operator/ (_Double v)
	{
		return _Double(this->value / v.value);
	}

	_Double operator/ (_Int v)
	{
		return _Double(v.value / this->value);
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
}*/

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

	explicit operator std::string() const
	{
		return value;
	}
};
