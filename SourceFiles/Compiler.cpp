#include "../HeaderFiles/Compiler.h"


#pragma region StaticRegion

bool CompilerUtility::GetTagState(TAGMAP& tags, std::string name)
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

bool Compiler::CanBeAVarName(std::string name)
{
	if (name.empty())
		return false;

	std::string avaliableFirstChars = "abcdefghijklmnopqrstuvwxyz_";
	std::string avaliableOtherChars = "1234567890";

	if (avaliableFirstChars.find(name[0], 0) == avaliableFirstChars.npos)
		return false;
	for (auto it = name.begin(); it != name.end(); it++)
	{
		if (avaliableFirstChars.find_first_of(*it, 0) == avaliableFirstChars.npos &&
			avaliableOtherChars.find_first_of(*it, 0) == avaliableOtherChars.npos)
		{
			return false;
		}
	}

	std::string notAbleStrs[]{ "program","const","var","begin" };

	for (int i = 0; i < sizeof(notAbleStrs) / sizeof(std::string); i++)
	{
		if (name == notAbleStrs[i])
		{
			return false;
		}
	}


	return true;
}

bool Compiler::IsPartEqual(Part* part, std::string str)
{
	if (part == nullptr)
		return false;
	else
		return part->str == str;
}

void CompilerUtility::MakeFine(std::list<std::string>& words)
{
	bool quoteOpened = false;
	std::string inQuotesStr = "";
	for (auto it = words.begin(); it != words.end();)
	{
		std::transform((*it).begin(), (*it).end(), (*it).begin(), [](unsigned char c) { return std::tolower(c); });

		if (*it == "'")
		{
			quoteOpened = !quoteOpened;
			if (!quoteOpened)
			{
				*it = inQuotesStr + "'";
				inQuotesStr = "";
			}

		}

		if (quoteOpened)
		{
			inQuotesStr += *it;
		}


		if (quoteOpened || *it == " " || *it == "\n" || *it == "\t")
		{

			if (it == words.begin())
			{
				words.erase(it);
				it = words.begin();
			}
			else
			{
				auto removeIt = it;
				it--;
				words.erase(removeIt);
				it++;
			}

		}
		else
		{
			it++;
		}
	}

	if (quoteOpened)
	{
		throw CompilerExc("Bad Quotes");
	}
}

Part* CompilerUtility::SplitStr(std::string str)
{
	std::string chars[21]{ " ","\n","\t",";",",","'","\"",":=","(",")","+","-","*","/","=","<>","<=","<",">=",">",":" };

	std::list<std::string> words = TextSplitter::Split(str, chars, sizeof(chars) / sizeof(std::string));

	MakeFine(words);

	if (words.empty())
	{
		//program is empty
		return nullptr;
	}

	Part* _first = new Part({ words.front(), nullptr, nullptr });
	words.pop_front();

	TAGMAP tags;

	try
	{
		Part* curr = _first;
		while (curr != nullptr)
		{
			curr = GoToNextPart(curr, &words, tags);
		}
	}
	catch (CompilerExc ex)
	{
		Clear(_first);
		throw ex;

	}


	return _first;
}

bool CompilerUtility::IsEndWordFor(std::string start, std::string end, TAGMAP tags)
{
	//std::string possibleEqualParts[]{ "then","else" };

	if (GetTagState(tags, "equalParts")) // && std::find(std::begin(possibleEqualParts), std::end(possibleEqualParts), start)== std::end(possibleEqualParts)
	{
		if (start == "then" || start == "else")
		{
			if (end == ";")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if (start == "program")
	{
		return end == ";";
	}
	else if (start == "var" || start == "const")
	{
		return end == "var" || end == "const" || end == "begin";
	}
	else if (start == "=")
	{
		if (GetTagState(tags, "const"))
		{
			return end == ";";
		}
		else
		{
			return true;
		}
	}
	else if (start == "(")
	{
		return end == ")";
	}
	else if (start == "begin")
	{
		return end == "end" || end == "end.";
	}
	else if (start == ":=")
	{
		return end == ";" || end == "end" || end == "end." || end == "else";
	}
	else if (start == "if")
	{
		return end == "then";
	}
	else if (start == "then")
	{
		return end == "else" || end == ";";
	}
	else if (start == "else")
	{
		return end == "else" || end == ";" || end == "if";
	}

	return true;
}

Part* CompilerUtility::GoToNextPart(Part* part, std::list<std::string>* words, TAGMAP& tags)
{
	if (part == nullptr)
	{
		return nullptr;
	}

	if (part->str == "const")
	{
		tags["const"] = true;
	}
	else if (part->str == "var" || part->str == "begin")
	{
		tags["const"] = false;
	}

	if (part->next == nullptr && !words->empty())
	{
		Part* newPart = new Part({ words->front() });
		words->pop_front();
		//std::string nextStr = *(words->begin());

		if (newPart != nullptr && !IsEndWordFor(part->str, newPart->str, tags))
		{
			part->nextInside = newPart;
			newPart->prev = part;
		}

		while (newPart != nullptr && !IsEndWordFor(part->str, newPart->str, tags))
		{
			tags["equalParts"] = false;
			Part* prevPart = newPart;
			newPart = GoToNextPart(newPart, words, tags);
			tags["equalParts"] = part->str == prevPart->str;
		}
		if (newPart == nullptr)
		{
			throw ExpectedExc("closing part of '" + part->str + "'");
		}
		else
		{
			part->next = newPart;

			newPart->prev = part;
			/*if (newPart->prev == nullptr)
			{
				newPart->prev = part;
			}
			else
			{
				newPart->prevInside = newPart->prev;
				newPart->prev = part;
			}*/
		}


	}
	return part->next;
}

std::list<std::pair<Part*, Part*>> Compiler::GetArgumentsOfFuncCall(Part* bracket)
{
	std::list<std::pair<Part*, Part*>> result;

	Part* end = bracket->next;
	Part* curr = bracket;

	Part* prev = nullptr;

	Part* argStart = nullptr;

	while (curr != end)
	{
		if (argStart == nullptr)
		{
			if (curr->str != "(" && curr->str != ",")
			{
				argStart = curr;
			}
		}
		else
		{
			if (curr->str == ",")
			{
				result.push_back(std::pair<Part*, Part*>(argStart, curr));
				argStart = nullptr;
			}
		}

		prev = curr;
		if (curr->nextInside != nullptr)
		{
			curr = curr->nextInside;
		}
		else
		{
			curr = curr->next;
		}
	}

	if (prev->str == ",")
	{
		throw NotExpectedExc(prev->str);
	}

	if (argStart != nullptr)
	{
		result.push_back(std::pair<Part*, Part*>(argStart, curr));
	}

	return result;
}

void Compiler::CheckForErrors(Part* _first, TAGMAP& tags, std::map<std::string, std::pair<Var::_Type, bool>>& varTypes)
{
	if (_first == nullptr)
	{
		return;
	}



	if (_first->str == "program")
	{
		if (CompilerUtility::GetTagState(tags, "const") ||
			CompilerUtility::GetTagState(tags, "var") ||
			CompilerUtility::GetTagState(tags, "program"))
		{
			throw NotExpectedExc(_first->str);
		}

		if (_first->nextInside == nullptr)
		{
			throw ExpectedExc("program name");
		}

		if (!IsPartEqual(_first->nextInside->next, ";"))
		{
			throw ExpectedExc(";");
		}

		CheckForErrors(_first->next, tags, varTypes);
		return;
	}
	if (_first->str == "const")
	{
		if (CompilerUtility::GetTagState(tags, "program"))
		{
			throw NotExpectedExc(_first->str);
		}
		if (_first->nextInside == nullptr)
		{
			throw ExpectedExc("consts creation");
		}
		tags["const"] = true;
		tags["var"] = false;
		CheckForErrors(_first->nextInside, tags, varTypes);
		return;
	}
	else if (_first->str == "var")
	{
		if (CompilerUtility::GetTagState(tags, "program"))
		{
			throw NotExpectedExc(_first->str);
		}
		if (_first->nextInside == nullptr)
		{
			throw ExpectedExc("vars creation");
		}

		tags["const"] = false;
		tags["var"] = true;
		CheckForErrors(_first->nextInside, tags, varTypes);
		return;
	}
	else if (_first->str == "begin" && !CompilerUtility::GetTagState(tags, "program"))
	{
		tags["const"] = false;
		tags["var"] = false;
		tags["program"] = true;

		if (!IsPartEqual(_first->next, "end."))
		{
			throw ExpectedExc("end.");
		}
		
		if (_first->nextInside == nullptr)
		{
			CheckForErrors(_first->next, tags, varTypes);
		}
		else
		{
			CheckForErrors(_first->nextInside, tags, varTypes);
		}
		return;
	}

	if (_first->str == ";")
	{
		CheckForErrors(_first->next, tags, varTypes);
		return;
	}

	if (_first->str == ")")
	{
		if (!IsPartEqual(_first->prev, "("))
		{
			throw NotExpectedExc(_first->str);
		}
	}
	else if (_first->str == "end")
	{
		if (!IsPartEqual(_first->prev, "begin"))
		{
			throw NotExpectedExc(_first->str);
		}
	}
	else if (_first->str == "end.")
	{
		if (!IsPartEqual(_first->prev, "begin") ||
			_first->next != nullptr)
		{
			throw NotExpectedExc(_first->str);
		}
	}
	else if (Function::IsFuncName(_first->str))
	{
		if (!IsPartEqual(_first->next, "("))
		{
			throw ExpectedExc(_first->next->str);
		}
		if (!IsPartEqual(_first->next->next->next, ";") && !IsPartEqual(_first->next->next->next, "else"))
		{
			throw ExpectedExc(";");
		}

		auto argParts = GetArgumentsOfFuncCall(_first->next);

		std::list<std::pair<Var::_Type, bool>> argTypes;
		for (auto it = argParts.begin(); it != argParts.end(); it++)
		{
			bool isConst = true;
			if (it->first->next == it->second && varTypes.count(it->first->str))
			{
				isConst = varTypes[it->first->str].second;
			}

			argTypes.push_back(std::pair<Var::_Type, bool>(Postfix::CheckOnCompile(it->first, it->second, varTypes), isConst));
		}

		Function::CheckOnCompile(_first->str, argTypes);

		CheckForErrors(_first->next->next, tags, varTypes);
		return;
	}
	else if (_first->str == "if")
	{
		/*if (!IsPartEqual(_first->next, "then"))
		{
			throw "compilation error";
		}*/
		if (_first->nextInside == nullptr)
		{
			throw ExpectedExc("condition");
		}

		if (!Var::CanCast(Postfix::CheckOnCompile(_first->nextInside, _first->next, varTypes), Var::_Type::_bool))
		{
			throw CompilerExc("condition is not a bool");
		}


		CheckForErrors(_first->next, tags, varTypes);
		return;
	}
	else if (_first->str == "then")
	{
		if (!IsPartEqual(_first->prev, "if"))
		{
			throw NotExpectedExc(_first->str);
		}

		TAGMAP tmp;
		if (_first->nextInside == nullptr || CompilerUtility::IsEndWordFor("then", _first->nextInside->str, tmp))
		{
			throw ExpectedExc("body of if");
		}
	}
	else if (_first->str == "else")
	{
		if (!IsPartEqual(_first->prev, "then"))
		{
			throw NotExpectedExc(_first->str);
		}

		if (!IsPartEqual(_first->next,"if") && _first->nextInside == nullptr)
		{
			throw ExpectedExc("body of else");
		}

		CheckForErrors(_first->next, tags, varTypes);
		return;
	}
	else if (CanBeAVarName(_first->str))
	{
		if (CompilerUtility::GetTagState(tags, "const"))
		{
			if (_first->next->str != "=")
			{
				throw NotExpectedExc(_first->next->str);
			}

			if (_first->next->nextInside == nullptr)
			{
				throw ExpectedExc("const initialization");
			}
			if (varTypes.count(_first->str) != 0)
			{
				throw CompilerExc("var or const with same name already exists");
			}

			varTypes[_first->str].first = Postfix::CheckOnCompile(_first->next->nextInside, _first->next->next, varTypes);
			varTypes[_first->str].second = true;

			CheckForErrors(_first->next->next, tags, varTypes);
			return;
		}
		else if (CompilerUtility::GetTagState(tags, "var"))
		{
			std::list<std::string> newVarNames;

			Part* curr = _first;

			bool wasComma = true;

			while (curr->str != ":")
			{
				if (curr->str != ",")
				{
					if (!wasComma)
					{
						throw NotExpectedExc(curr->str);
					}
					if (!CanBeAVarName(curr->str))
					{
						throw CompilerExc(curr->str + " can't be a var name");
					}
					newVarNames.push_back(curr->str);
					wasComma = false;
				}
				else
				{
					wasComma = true;
				}

				curr = curr->next;
				if (curr == nullptr)
				{
					throw ExpectedExc(":");
				}
			}

			if (wasComma)
			{
				throw NotExpectedExc(",");
			}

			if (curr->next == nullptr || curr->next->str == ";")
			{
				throw ExpectedExc("var type");
			}


			if (!IsPartEqual(curr->next->next, ";"))
			{
				throw ExpectedExc(";");
			}

			Var::_Type varsType = Var::GetTypeByString(curr->next->str);

			for (auto it = newVarNames.begin(); it != newVarNames.end(); it++)
			{
				if (varTypes.count(*it) != 0)
				{
					throw CompilerExc("var or const with same name already exists");
				}

				varTypes[*it].first = varsType;
				varTypes[*it].second = false;
			}

			CheckForErrors(curr->next->next, tags, varTypes);
			return;
		}
		else if (CompilerUtility::GetTagState(tags, "program"))
		{
			if (IsPartEqual(_first->next, ":="))
			{
				Part* equalSign = _first->next;
				if (equalSign->nextInside == nullptr)
				{
					throw ExpectedExc("expression");
				}

				if (varTypes.count(_first->str) == 0)
				{
					throw CompilerExc("var with same name '" + _first->str + "' unidentified");
				}
				if (varTypes[_first->str].second)
				{
					throw CompilerExc("consts can't be changed");
				}

				Var::_Type t = Postfix::CheckOnCompile(equalSign->nextInside, equalSign->next, varTypes);
				if (!Var::CanCast(t, varTypes[_first->str].first))
				{
					throw CompilerExc("cant convert from '" + Var::GetTypeName(t) + "' to '" + Var::GetTypeName(varTypes[_first->str].first) + "'");
				}
				CheckForErrors(equalSign->next, tags, varTypes);
				return;
			}
			else
			{
				throw NotExpectedExc(_first->str);
			}
		}
		else
		{
			throw NotExpectedExc(_first->str);
		}
	}
	else
	{
		//go through
		if (_first->str == "begin")
		{
			CheckForErrors(_first->nextInside, tags, varTypes);
			return;
		}

		throw NotExpectedExc(_first->str);
	}


	if (_first->nextInside == nullptr)
	{
		CheckForErrors(_first->next, tags, varTypes);
	}
	else
	{
		CheckForErrors(_first->nextInside, tags, varTypes);
	}
}

void CompilerUtility::Clear(Part* p)
{
	while (p != nullptr)
	{
		Part* delPart = p;

		if (p->nextInside == nullptr)
		{
			p = p->next;
		}
		else
		{
			p = p->nextInside;
		}
		delete delPart;
	}
}

void Compiler::Run(Part* _first, TAGMAP tags, TableHash& vars)
{
	if (_first == nullptr)
	{
		return;
	}

	if (_first->str == "program")
	{
		//TODO: set program name
		Run(_first->next, tags, vars);
		return;
	}
	if (_first->str == "const")
	{
		tags["const"] = true;
		tags["var"] = false;
		Run(_first->nextInside, tags, vars);
		return;
	}
	else if (_first->str == "var")
	{
		tags["const"] = false;
		tags["var"] = true;
		Run(_first->nextInside, tags, vars);
		return;
	}
	else if (_first->str == "begin" && !CompilerUtility::GetTagState(tags, "program"))
	{
		tags["const"] = false;
		tags["var"] = false;
		tags["program"] = true;

		Run(_first->nextInside, tags, vars);
		return;
	}



	if (_first->str == ";")
	{
		tags["gotoelse"] = false;
	}

	std::string passthrough[]{ ";",")","end","end.","then" };
	auto lastPT = passthrough + sizeof(passthrough) / sizeof(std::string);
	if (std::find(passthrough, lastPT, _first->str) != lastPT)
	{
		Run(_first->next, tags, vars);
		return;
	}



	if (Function::IsFuncName(_first->str))
	{
		auto argParts = GetArgumentsOfFuncCall(_first->next);

		std::list<std::pair<Var*, bool>> args;
		std::list<Var*> toDelete;
		for (auto it = argParts.begin(); it != argParts.end(); it++)
		{
			bool isConst = true;
			if (it->first->next == it->second && vars.Find(it->first->str)!=nullptr)
			{
				args.push_back(*vars.Find(it->first->str));
			}
			else
			{
				Var* nv = Postfix::Calculate(it->first, it->second, vars);
				toDelete.push_back(nv);
				args.push_back(std::pair<Var*, bool>(nv, true));
			}

		}

		Function::Calculate(_first->str, args);

		for (auto it = toDelete.begin(); it != toDelete.end(); it++)
		{
			delete (*it);
		}

		Run(_first->next->next, tags, vars);
		return;
	}
	else if (_first->str == "if")
	{
		_Bool* b = (_Bool*)Postfix::Calculate(_first->nextInside, _first->next, vars);

		if (b->value)
		{
			Run(_first->next->nextInside, tags, vars);

		}
		else
		{
			tags["gotoelse"] = true;
			Run(_first->next->next, tags, vars);
		}
		return;
	}
	else if (_first->str == "else")
	{
		if (!CompilerUtility::GetTagState(tags, "gotoelse"))
		{
			if (_first->next->str == "if")
			{
				Part* nextPart = _first->next->next;
				while (nextPart->str != ";")
				{
					nextPart = nextPart->next;
				}
				Run(nextPart, tags, vars);
			}
			else
			{
				Run(_first->next, tags, vars);
			}
			return;
		}
		tags["gotoelse"] = false;

	}
	else if (CanBeAVarName(_first->str))
	{
		if (CompilerUtility::GetTagState(tags, "const"))
		{
			Var* tmp = Postfix::Calculate(_first->next->nextInside, _first->next->next, vars);
			std::pair<Var*, bool> p = std::make_pair(tmp, true);
			vars.Insert(_first->str, p);
			//vars.Find(_first->str)->first = 
			//vars.Find(_first->str)->second = true;

			Run(_first->next->next, tags, vars);
			return;
		}
		else if (CompilerUtility::GetTagState(tags, "var"))
		{
			std::list<std::string> newVarNames;

			Part* curr = _first;

			while (curr->str != ":")
			{
				if (curr->str != ",")
				{
					newVarNames.push_back(curr->str);
				}
				curr = curr->next;
			}

			Var::_Type varsType = Var::GetTypeByString(curr->next->str);

			for (auto it = newVarNames.begin(); it != newVarNames.end(); it++)
			{
				Var* tmp = Var::CreateVarByType(varsType);
				std::pair<Var*, bool> p = std::make_pair(tmp, false);
				vars.Insert(*it, p);
				//vars.Find(*it)->first = Var::CreateVarByType(varsType);
				//vars.Find(*it)->second = false;
			}

			Run(curr->next->next, tags, vars);
			return;
		}
		else if (CompilerUtility::GetTagState(tags, "program"))
		{
			if (IsPartEqual(_first->next, ":="))
			{
				Part* equalSign = _first->next;

				Var* newVar = Postfix::Calculate(equalSign->nextInside, equalSign->next, vars);

				Var::Cast(vars.Find(_first->str)->first, newVar);

				delete newVar;

				//vars[_first->str] = pair<Var*, bool>(newVar, false);

				Run(equalSign->next, tags, vars);
				return;
			}
		}
	}


	if (_first->nextInside == nullptr)
	{
		Run(_first->next, tags, vars);
	}
	else
	{
		Run(_first->nextInside, tags, vars);
	}
}

#pragma endregion

void Compiler::Clear()
{
	CompilerUtility::Clear(first);
	first = nullptr;
}

void Compiler::Compile(std::string str)
{
	Clear();
	std::map<std::string, std::pair<Var::_Type, bool>> varTypes;


	first = CompilerUtility::SplitStr(str);

	TAGMAP tags;

	
	CheckForErrors(first, tags, varTypes);


}

void Compiler::Run()
{
	if (first == nullptr)
	{
		throw CompilerExc("Program is not compiled.");
	}

	TAGMAP tags;
	TableHash vars;
	//std::map<std::string, std::pair<Var*, bool>> vars;

	Run(first, tags, vars);

	for (auto it = vars.begin(); it != vars.end(); it++)
	{
		delete (*it).polynomial.first;
	}
}


