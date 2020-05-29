#pragma once
#include <string>
#include <ostream>
#include <sstream>


class Var
{
public:
	static const std::string typeName[static_cast<int>(5)];

	enum class _Type
	{
		_int, _double, _bool, _string, _void
	};

	
	static bool CanCast(_Type from, _Type to);
	static void Cast(Var* to, Var* from);
	static Var* CreateCopy(Var* v);
	static Var* Assign(std::string);
	_Type GetType();
	static _Type GetTypeByString(std::string);
	static std::string GetTypeName(_Type);
	static Var* CreateVarByType(_Type t);

	//virtual Var operator+ (Var& v);

	virtual std::string ToString() { throw "Error"; }
	virtual Var* operator+ (Var*) {throw "Error";}
	virtual Var* operator- (Var*) { throw "Error"; }
	virtual Var* operator* (Var*) { throw "Error"; }
	virtual Var* operator/ (Var*) { throw "Error"; }
	virtual Var* operator% (Var*) { throw "Error"; }
	virtual Var* operator&& (Var*) { throw "Error"; }
	virtual Var* operator|| (Var*) { throw "Error"; }
	virtual Var* operator== (Var* v) { throw "Error"; }
	virtual Var* operator!= (Var* v) { throw "Error"; }
	virtual Var* operator< (Var* v) { throw "Error"; }
	virtual Var* operator<= (Var* v) { throw "Error"; }
	virtual Var* operator> (Var* v) { throw "Error"; }
	virtual Var* operator>= (Var* v) { throw "Error"; }
	virtual Var* operator! () { throw "Error"; }
	virtual Var* operator= (Var* v) { throw "Error"; }

protected:
	_Type _type;
};

class _Int : public Var
{
public:
	int value;

	_Int(int v);
	_Int();

	std::string ToString() override
	{
		return std::to_string(value);
	}

	Var* operator+ (Var* v) override;
	Var* operator- (Var* v) override;
	Var* operator* (Var* v) override;
	Var* operator/ (Var* v) override;
	Var* operator% (Var* v) override;
	Var* operator== (Var* v) override;
	Var* operator!= (Var* v) override;
	Var* operator< (Var* v) override;
	Var* operator<= (Var* v) override;
	Var* operator> (Var* v) override;
	Var* operator>= (Var* v) override;
	Var* operator= (Var* v) override;


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

class _Double : public Var
{
public:
	double value;

	_Double(double v);
	_Double(int v);
	_Double();

	Var* operator+ (Var* v) override;
	Var* operator- (Var* v) override;
	Var* operator* (Var* v) override;
	Var* operator/ (Var* v) override;
	Var* operator== (Var* v) override;
	Var* operator!= (Var* v) override;
	Var* operator< (Var* v) override;
	Var* operator<= (Var* v) override;
	Var* operator> (Var* v) override;
	Var* operator>= (Var* v) override;
	Var* operator= (Var* v) override;
	
	std::string ToString() override
	{
		std::ostringstream out;
		out.precision(3);
		out << std::fixed << value;
		return out.str();
	}


	explicit operator double() const
	{
		return value;
	}
};


class _Bool : public Var
{
public:
	bool value;

	_Bool(bool v);
	_Bool(int v);
	_Bool();

	Var* operator&& (Var* v) override;
	Var* operator|| (Var* v) override;
	Var* operator== (Var* v) override;
	Var* operator!= (Var* v) override;
	Var* operator!() override;
	Var* operator= (Var* v) override;

	std::string ToString() override
	{
		if (value)
			return "true";
		else
			return "false";
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

class _String : public Var
{
public:
	std::string value;

	_String(std::string v);
	_String();


	Var* operator+ (Var* v) override;
	Var* operator== (Var* v) override;
	Var* operator!= (Var* v) override;
	Var* operator= (Var* v) override;

	std::string ToString() override
	{
		return value;
	}

	explicit operator std::string() const
	{
		return value;
	}
};

