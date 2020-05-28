#pragma once
#include<string>
#include <list>
#include "Var.h"
#include <iostream>
#include "CompilerExceptions.h"

const std::list<std::string> funcNameList{
   "readln",
   "read",
   "writeln",
   "write"
};

class Function
{
public:
    static void CheckOnCompile(std::string FuncName, std::list<std::pair<Var::_Type, bool>> argTypes);
    static void Calculate(std::string FuncName, std::list<std::pair<Var*, bool>>& args)
    {
        if (FuncName == "read" || FuncName == "readln")
        {
            for (auto it = args.begin(); it != args.end(); it++)
            {
                if (it->first->GetType() == Var::_Type::_string)
                {
                    std::cin >> ((_String*)(it->first))->value;
                }
                else if (it->first->GetType() == Var::_Type::_int)
                {
                    std::cin >> ((_Int*)(it->first))->value;
                }
                else if (it->first->GetType() == Var::_Type::_double)
                {
                    std::cin >> ((_Double*)(it->first))->value;
                }
                else if (it->first->GetType() == Var::_Type::_bool)
                {
                    std::cin >> ((_Bool*)(it->first))->value;
                }
                else
                {
                    throw CompilerExc("exception, which should not be");
                }
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
    static bool IsFuncName(std::string name);
private:
};

