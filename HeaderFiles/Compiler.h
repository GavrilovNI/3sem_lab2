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

namespace CompilerExceptions
{
	class ComplierExc : public std::exception
	{
		virtual const char* what() const throw ()
		{
			return "unknown compiler exception";
		}
	};

	class NotExpectedExc : public ComplierExc
	{
	private:
		std::string str;
	public:
		NotExpectedExc(std::string str)
		{
			this->str = str;
		}

		const char* what() const throw () override
		{
			return string(str + " isn't expected here.").c_str();
		}
	};
	class ExpectedExc : public ComplierExc
	{
	private:
		std::string str;
	public:
		ExpectedExc(std::string str)
		{
			this->str = str;
		}

		const char* what() const throw () override
		{
			return string(str + " is expected here.").c_str();
		}
	};
}


using namespace CompilerExceptions;

class Compiler
{
private:
	

	bool GetTagState(std::map<std::string, bool> &tags, std::string name)
	{
		return (tags.find(name) != tags.end() && tags.find(name)->second);
	}
	bool CanBeAVarName(std::string name)
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

		for (int i = 0; i < sizeof(notAbleStrs)/sizeof(string); i++)
		{
			if (name == notAbleStrs[i])
			{
				return false;
			}
		}


		return true;
	}
	bool IsPartEqual(Part* part, std::string str)
	{
		if (part == nullptr)
			return false;
		else
			return part->str == str;
	}

	void MakeFine(std::list<std::string>* words)
	{
		bool quoteOpened = false;
		std::string inQuotesStr = "";
		for (auto it = words->begin(); it != words->end();)
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
				
				if (it == words->begin())
				{
					words->erase(it);
					it = words->begin();
				}
				else
				{
					auto removeIt = it;
					it--;
					words->erase(removeIt);
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
			throw "Bad Quotes";
		}
	}

	Part* SplitStr(std::string str)
	{
		std::string chars[21]{ " ","\n","\t",";",",","'","\"",":=","(",")","+","-","*","/","=","<>","<=","<",">=",">",":" };

		std::list<std::string> words = TextSplitter::Split(str, chars, sizeof(chars) / sizeof(std::string));
		MakeFine(&words);

		if (words.empty())
		{
			//program is empty
			return nullptr;
		}

		Part* first = new Part({ words.front(), nullptr, nullptr });
		words.pop_front();

		std::map<std::string, bool> tags;

		Part* curr = first;
		while (curr != nullptr)
		{
			curr = GoToNextPart(curr, &words, &tags);
		}

		return first;
	}

	bool IsEndWordFor(std::string start, std::string end, std::map<std::string, bool> tags)
	{
		if (GetTagState(tags,"equalParts"))
		{
			return false;
		}

		if (start=="program")
		{
			return end == ";";
		}
		else if(start == "var" || start == "const")
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
			return end == ";" || end == "end" || end == "end.";
		}
		else if (start == "if")
		{
			return end == "then";
		}

		return true;
	}

	Part* GoToNextPart(Part* part, std::list<std::string>* words, std::map<std::string, bool>* tags)
	{
		if (part == nullptr)
		{
			return nullptr;
		}

		if (part->str == "const")
		{
			(*tags)["const"] = true;
		}
		else if (part->str=="var" || part->str == "begin")
		{
			(*tags)["const"] = false;
		}

		if (part->next == nullptr && !words->empty())
		{
			Part* newPart = new Part({ words->front() });
			words->pop_front();
			//std::string nextStr = *(words->begin());

			if (newPart != nullptr && !IsEndWordFor(part->str, newPart->str, *tags))
			{
				part->nextInside = newPart;
				newPart->prev = part;
			}

			while (newPart != nullptr && !IsEndWordFor(part->str, newPart->str, *tags))
			{
				(*tags)["equalParts"] = false;
				Part* prevPart = newPart;
				newPart = GoToNextPart(newPart, words, tags);
				(*tags)["equalParts"] = part->str == prevPart->str;
			}
			if (newPart == nullptr)
			{
				throw "compilation error";//not found next part for this one
				}
			else
			{
				part->next = newPart;
				if (newPart->prev == nullptr)
				{
					newPart->prev = part;
				}
				else
				{
					newPart->prevInside = newPart->prev;
					newPart->prev = part;
				}
			}


		}
		return part->next;
	}

	std::list<std::pair<Part*, Part*>> GetArgumentsOfFuncCall(Part* bracket)
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
			throw "compilation error";
		}

		if (argStart != nullptr)
		{
			result.push_back(std::pair<Part*, Part*>(argStart, curr));
		}

		return result;
	}

	void CheckForErrors(Part* first, std::map<std::string, bool>* tags, std::map<std::string, std::pair<Var::_Type, bool>> &varTypes)
	{
		if (first == nullptr)
		{
			return;
		}
		
		if (first->str == "program")
		{
			if (GetTagState(*tags, "const") ||
				GetTagState(*tags, "var") ||
				GetTagState(*tags, "program"))
			{
				throw NotExpectedExc(first->str);
			}

			if (first->nextInside == nullptr)
			{
				throw ExpectedExc("program name");
			}

			if(!IsPartEqual( first->nextInside->next, ";"))
			{
				throw ExpectedExc(";");
			}

			CheckForErrors(first->next, tags, varTypes);
			return;
		}
		if (first->str == "const")
		{
			if (GetTagState(*tags, "program"))
			{
				throw NotExpectedExc(first->str);
			}
			if (first->nextInside == nullptr)
			{
				throw ExpectedExc("consts creation");
			}
			(*tags)["const"] = true;
			(*tags)["var"] = false;
			CheckForErrors(first->nextInside, tags, varTypes);
			return;
		}
		else if (first->str == "var")
		{
			if (GetTagState(*tags, "program"))
			{
				throw NotExpectedExc(first->str);
			}
			if (first->nextInside == nullptr)
			{
				throw ExpectedExc("vars creation");
			}

			(*tags)["const"] = false;
			(*tags)["var"] = true;
			CheckForErrors(first->nextInside, tags, varTypes);
			return;
		}
		else if (first->str == "begin" && tags->count("begin")==0)
		{
			(*tags)["const"] = false;
			(*tags)["var"] = false;
			(*tags)["program"] = true;

			if (!IsPartEqual(first->next, "end."))
			{
				throw ExpectedExc("end.");
			}

			CheckForErrors(first->nextInside, tags, varTypes);
			return;
		}

		if (first->str == ";")
		{
			CheckForErrors(first->next, tags, varTypes);
			return;
		}

		/*if (first->str=="(")
		{
			if (first->next == nullptr || first->next->str != ")")
			{
				throw "compilation error";
			}
		}
		else */if (first->str == ")")
		{
			if (!IsPartEqual(first->prev, "("))
			{
				throw NotExpectedExc(first->str);
			}
		}
		/*else if (first->str=="begin")
		{
			if (first->next == nullptr || (first->next->str != "end" && first->next->str!="end."))
			{
				throw "compilation error";
			}
		}*/
		else if (first->str == "end")
		{
			if (!IsPartEqual(first->prev, "begin"))
			{
				throw NotExpectedExc(first->str);
			}
		}
		else if (first->str == "end.")
		{
			if (!IsPartEqual(first->prev, "begin") || 
				first->next != nullptr)
			{
				throw NotExpectedExc(first->str);
			}
		}
		else if (Function::IsFuncName(first->str))
		{
			if (!IsPartEqual(first->next,"("))
			{
				throw ExpectedExc(first->next->str);
			}

			auto argParts = GetArgumentsOfFuncCall(first->next);

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

			Function::CheckOnCompile(first->str, argTypes);

			CheckForErrors(first->next->next, tags, varTypes);
			return;
		}
		/*else if (first->str == "="  && GetTagState(*tags, "const"))
		{
			if (first->nextInside == nullptr)
			{
				throw "compilation error";
			}

			Part* varPart = first->prev;
			if (varPart == nullptr)
			{
				throw "compilation error";
			}
			if (varTypes.count(varPart->str) != 0)
			{
				throw "compilation error";
			}

			if (varTypes[varPart->str].first != Postfix::CheckOnCompile(first, first->next, varTypes))
			{
				throw "compilation error";
			}
		}*/
		else if (first->str == "if")
		{
			/*if (!IsPartEqual(first->next, "then"))
			{
				throw "compilation error";
			}*/
			if (first->nextInside == nullptr)
			{
				throw "compilation error";
			}

			if (Postfix::CheckOnCompile(first->nextInside, first->next, varTypes)!=Var::_Type::_bool)
			{
				throw "compilation error";
			}

			CheckForErrors(first->next, tags, varTypes);
			return;
		}
		else if (first->str == "then")
		{
			if (!IsPartEqual(first->prev, "if"))
			{
				throw "compilation error";
			}

			if (IsPartEqual(first->next, "end") || 
				IsPartEqual(first->next, "end."))
			{
				throw NotExpectedExc(first->next->str);
			}
		}
		else if (CanBeAVarName(first->str))
		{
			if (GetTagState(*tags, "const"))
			{
				if (first->next->str != "=")
				{
					throw "compilation error";
				}

				if (first->next->nextInside == nullptr)
				{
					throw "compilation error";
				}
				if (varTypes.count(first->str) != 0)
				{
					throw "compilation error";
				}

				varTypes[first->str].first = Postfix::CheckOnCompile(first->next->nextInside, first->next->next, varTypes);
				varTypes[first->str].second = true;

				CheckForErrors(first->next->next, tags, varTypes);
				return;
			}
			else if (GetTagState(*tags, "var"))
			{
				std::list<std::string> newVarNames;

				Part* curr = first;

				while (curr->str != ":")
				{
					if (curr->str != ",")
					{
						if (!CanBeAVarName(curr->str))
						{
							throw "compilation error";
						}
						newVarNames.push_back(curr->str);
					}
					curr = curr->next;
					if (curr == nullptr)
					{
						throw "compilation error";
					}
				}

				
				if (curr->next == nullptr)
				{
					throw "compilation error";//var type not present;
				}
				else if (curr->next->str == ";")
				{
					throw ExpectedExc("var type");
				}


				if (!IsPartEqual(curr->next->next, ";"))
				{
					throw "compilation error";//; not present;
				}

				Var::_Type varsType = Var::GetTypeByString(curr->next->str);

				for (auto it = newVarNames.begin(); it != newVarNames.end(); it++)
				{
					if (varTypes.count(*it) != 0)
					{
						throw "compilation error";
					}

					varTypes[*it].first = varsType;
					varTypes[*it].second = false;
				}

				CheckForErrors(curr->next->next, tags, varTypes);
				return;
			}
			else if (GetTagState(*tags, "program"))
			{
				if (IsPartEqual(first->next, ":="))
				{
					Part* equalSign = first->next;
					if (equalSign->nextInside == nullptr)
					{
						throw ExpectedExc("expression");
					}

					if (varTypes.count(first->str) == 0)
					{
						throw "var '" + first->str + "' unidentified";
					}
					if (varTypes[first->str].second)
					{
						throw "const can't be changed";//its const
					}

					Var::_Type t = Postfix::CheckOnCompile(equalSign->nextInside, equalSign->next, varTypes);
					if (varTypes[first->str].first != t)
					{
						//std::string s = "can't convert '"+ss+"'";
						throw "cant convert";
					}
					CheckForErrors(equalSign->next, tags, varTypes);
					return;
				}
				else
				{
					throw NotExpectedExc(first->str);
				}
			}
			else
			{
				throw "unknown error";
			}
		}
		else
		{
			throw "compilation error";
		}

		
		if (first->nextInside == nullptr)
		{
			CheckForErrors(first->next, tags, varTypes);
		}
		else
		{
			CheckForErrors(first->nextInside, tags, varTypes);
		}
	}

public:

	

	void Compile(std::string str)
	{
		//name:(type,isconst)
		std::map<std::string, std::pair<Var::_Type, bool>> varTypes = std::map<std::string, std::pair<Var::_Type, bool>>();

		
		Part* first = SplitStr(str);

		std::map<std::string, bool> tags;
		CheckForErrors(first, &tags, varTypes);


		while (first != nullptr)
		{
			Part* delPart = first;
			
			if (first->nextInside == nullptr)
			{
				first = first->next;
			}
			else
			{
				first = first->nextInside;
			}
			delete delPart;
		}

		/*for (auto const& [key, val] : vars)
		{
			delete val;
		}*/
	}



};