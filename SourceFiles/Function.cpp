#include "../HeaderFiles/Function.h"

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

void Function::Calculate(std::string FuncName, std::list<std::pair<Var*, bool>>& args)
{
    if (FuncName == "read" || FuncName == "readln")
    {
        for (auto it = args.begin(); it != args.end(); it++)
        {
            std::cin.clear();
            std::cin.sync();
            std::string s;
            std::getline(std::cin, s);
            try {
                if (it->first->GetType() == Var::_Type::_string)
                {
                    ((_String*)(it->first))->value = s;
                    //std::getline(std::cin, ((_String*)(it->first))->value);
                    //std::cin >> ((_String*)(it->first))->value;
                }
                else if (it->first->GetType() == Var::_Type::_int)
                {
                    ((_Int*)(it->first))->value = std::stoi(s);
                    //std::cin >> ((_Int*)(it->first))->value;
                }
                else if (it->first->GetType() == Var::_Type::_double)
                {
                    ((_Double*)(it->first))->value = std::stod(s);
                    //std::cin >> ((_Double*)(it->first))->value;
                }
                else if (it->first->GetType() == Var::_Type::_bool)
                {
                    if (s == "true")
                    {
                        ((_Double*)(it->first))->value = true;
                    }
                    else if (s == "false")
                    {
                        ((_Double*)(it->first))->value = false;
                    }
                    else
                    {
                        throw CompilerExc("cant convert to bool");
                    }

                    //std::cin >> ((_Bool*)(it->first))->value;
                }
                else
                {
                    throw CompilerExc("exception, which should not be");
                }
            }
            catch (...)
            {
                throw CompilerExc("cant convert '" + s + "' to " + Var::GetTypeName(it->first->GetType()));
            }
            std::cin.clear();
            std::cin.sync();
        }

        if (FuncName == "readln")
            std::cout << std::endl;
    }

    if (FuncName == "write" || FuncName == "writeln")
    {

        for (auto it = args.begin(); it != args.end(); it++)
        {
            std::cout << it->first->ToString();
        }

        if (FuncName == "writeln")
            std::cout << std::endl;
    }
}

bool Function::IsFuncName(std::string name)
{
	return !(std::find(funcNameList.begin(), funcNameList.end(), name) == funcNameList.end());
}
