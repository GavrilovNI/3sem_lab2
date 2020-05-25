#pragma once
#include<string>
#include<list>
#include "TextSplitter.h"
#include "Postfix.h"
#include "Var.h"
#include <map>
#include <algorithm>
#include "Part.h"



class Compiler
{
private:
	std::map<std::string, Var*> vars;



	void MakeFine(std::list<std::string>* words)
	{
		bool quoteOpened = false;
		std::string inQuotesStr = "";
		for (auto it = words->begin(); it != words->end(); it++)
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
				auto removeIt = it;
				it--;
				words->erase(removeIt);
			}
		}

		if (quoteOpened)
		{
			throw "Bad Quotes";
		}
	}

	Part* SplitStr(std::string str)
	{
		std::string chars[21]{ " ","\n","\t",";",",","'","\"",":=","(",")","+","-","*","/","=","<>","<","<=",">",">=",":" };

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
		if (tags.find("equalParts") != tags.end() && tags.find("equalParts")->second)
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
			if (tags.find("const") != tags.end() && tags.find("const")->second)
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
			Part* newPart = new Part({ *(words->begin())});
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
				throw "compilation error";
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

	void CheckForErros(Part* first, std::map<std::string, bool>* tags)
	{
		if (first == nullptr)
		{
			return;
		}

		std::list<std::string> functionNames;
		functionNames.push_back("write");
		functionNames.push_back("writeln");
		functionNames.push_back("read");
		functionNames.push_back("readln");
		
		if (first->str == "const")
		{
			(*tags)["const"] = true;
		}
		else if (first->str == "var" || first->str == "begin")
		{
			(*tags)["const"] = false;
		}

		if (first->str=="(")
		{
			if (first->next == nullptr || first->next->str != ")")
			{
				throw "compilation error";
			}
		}
		else if (first->str == ")")
		{
			if (first->prev == nullptr || first->prev->str != "(")
			{
				throw "compilation error";
			}
		}
		else if (first->str=="begin")
		{
			if (first->next == nullptr || (first->next->str != "end" && first->next->str!="end."))
			{
				throw "compilation error";
			}
		}
		else if (first->str == "end")
		{
			if (first->prev == nullptr || first->prev->str != "begin")
			{
				throw "compilation error";
			}
		}
		else if (first->str == "end.")
		{
			if (first->prev == nullptr ||
				first->prev->str != "begin")
			{
				throw "compilation error";
			}
			if (first->next != nullptr)
			{
				throw "compilation error";
			}
		}
		else if (std::find(functionNames.begin(), functionNames.end(), first->str) != functionNames.end())
		{
			if (first->next->str != "(")
			{
				throw "compilation error";
			}
		}
		else if (first->str == ":=" ||
			(tags->find("const") != tags->end() && tags->find("const")->second) && first->str == "=")
		{
			if (first->nextInside == nullptr)
			{
				throw "compilation error";
			}
			else
			{
				//TODO
				Var* var = Postfix::CheckOnCompile(first, vars);
				int x = 5 - 6;
			}
		}
		else if (first->str == "if")
		{
			if (first->next->str != "then")
			{
				throw "compilation error";
			}
			if (first->nextInside == nullptr)
			{
				throw "compilation error";
			}
		}

		
		if (first->nextInside == nullptr)
		{
			CheckForErros(first->next, tags);
		}
		else
		{
			CheckForErros(first->nextInside, tags);
		}
	}

public:

	

	void Compile(std::string str)
	{
		

		vars = std::map<std::string, Var*>();

		
		Part* first = SplitStr(str);

		std::map<std::string, bool> tags;
		CheckForErros(first, &tags);


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

		for (auto const& [key, val] : vars)
		{
			delete val;
		}
	}



};