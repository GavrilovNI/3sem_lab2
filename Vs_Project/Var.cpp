#include "../HeaderFiles/Var.h"

Var* Var::Assign(std::string v)
{
	if (v[0] == '\'')
	{
		v.erase(0, 1);
		int f = 0;
		if (((f = v.find("'", f)) != std::string::npos) && (v[f] == v.back()))
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
	catch (...)
	{
		throw "what a twist!";
	}
}

_Type Var::GetType()
{
	return type;
}
