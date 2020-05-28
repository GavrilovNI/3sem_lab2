#pragma once
#include<string>
#include<list>
#include "TextSplitter.h"
#include "Postfix.h"
#include "Var.h"
#include <map>
#include <algorithm>
#include "Part.h"
#include "Function.h"
#include "CompilerExceptions.h"
#include <variant>
#include "../HeaderFiles/TableHash.h"

using TAG = bool;
using TAGMAP = std::map<std::string, TAG>;

class CompilerUtility
{
protected:

	static void Clear(Part* p);

	static bool IsEndWordFor(std::string start, std::string end, TAGMAP tags);

	static Part* GoToNextPart(Part* part, std::list<std::string>* words, TAGMAP& tags);

	static bool GetTagState(TAGMAP& tags, std::string name);

	static void MakeFine(std::list<std::string>& words);

public:

	static Part* SplitStr(std::string str);

};

class Compiler: public CompilerUtility
{
private:
	
	static bool CanBeAVarName(std::string name);

	static bool IsPartEqual(Part* part, std::string str);

	static std::list<std::pair<Part*, Part*>> GetArgumentsOfFuncCall(Part* bracket);

	static void CheckForErrors(Part* _first, TAGMAP& tags, std::map<std::string, std::pair<Var::_Type, bool>>& varTypes);

	


	static void Run(Part* _first, TAGMAP tags, TableHash& vars);

	Part* first = nullptr;
public:

	~Compiler()
	{
		Clear();
	}
	
	void Clear();

	void Compile(std::string str);

	void Run();



};