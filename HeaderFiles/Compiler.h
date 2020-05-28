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


class Compiler
{
private:
	
	using TAG = bool;
	using TAGMAP = std::map<std::string, TAG>;



	static bool GetTagState(TAGMAP& tags, std::string name)
	{
		if (tags.count(name) == 0)
		{
			tags[name] = false;
		}

		return tags[name];


		/*if (tags.count(name) == 0)
		{
			if (std::is_same<T, int>::value)
				tags[name] = 0;
			else
				tags[name] = false;
		}
		return std::get<T>(tags[name]);*/
	}
	static bool CanBeAVarName(std::string name);

	static bool IsPartEqual(Part* part, std::string str);

	static void MakeFine(std::list<std::string>& words);

	static Part* SplitStr(std::string str);

	static bool IsEndWordFor(std::string start, std::string end, TAGMAP tags);

	static Part* GoToNextPart(Part* part, std::list<std::string>* words, TAGMAP& tags);

	static std::list<std::pair<Part*, Part*>> GetArgumentsOfFuncCall(Part* bracket);

	static void CheckForErrors(Part* _first, TAGMAP& tags, std::map<std::string, std::pair<Var::_Type, bool>>& varTypes);

	static void Clear(Part* p);


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