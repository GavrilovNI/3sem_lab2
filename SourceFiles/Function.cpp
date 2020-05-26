#include "Function.h"

void Function::CheckOnCompile(std::string FuncName, std::list<std::pair<Var::_Type, bool>> argTypes)
{
	if(!IsFuncName(FuncName))
		throw (FuncName + " is undefined");
	if (FuncName == "read" || FuncName == "readln")
	{
		for (auto itType = argTypes.begin(); itType != argTypes.end(); ++itType)
		{
			if (itType->second == true)
			{
				throw "error input";
			}
		}
	}
}

Var* Function::Calculate(std::string FuncName, std::list<Var*> args)
{
	return nullptr;
}

bool Function::IsFuncName(std::string name)
{
	return !(std::find(funcNameList.begin(), funcNameList.end(), name) == funcNameList.end());
}
