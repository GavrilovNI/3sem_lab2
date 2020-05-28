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
    static void Calculate(std::string FuncName, std::list<std::pair<Var*, bool>>& args);
    static bool IsFuncName(std::string name);
private:
};

