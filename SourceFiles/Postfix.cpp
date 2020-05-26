#include "../HeaderFiles/Postfix.h"

#include <stack>
#include <iostream>

/*bool PostfixCheckOnCompile(Part* part, std::map<std::string, Var> vars);
part  = :=
Var* Postfix(...);*/

list<string> Postfix::ToList(Part* start, Part* end)
{
	list<string> tmp;
	auto it = start;
	bool F = true;
	if (it->str == "-")
	{
		tmp.emplace_back("0");
	}
	else if (it->str == "+")
	{
		++it;
	}
	while (F)
	{
		if (it == end)
		{
			F = false;
		}
		else if (it->nextInside == nullptr)
		{
			if ((it->str == "-") && (tmp.back() == "("))
			{
				tmp.emplace_back("0");
				tmp.emplace_back(it->str);
			}
			else if (!((it->str == "+") && (tmp.back() == "(")))
			{
				tmp.emplace_back(it->str);
			}
			it = it->next;
		}
		else
		{
			if ((it->str == "-") && (tmp.back() == "("))
			{
				tmp.emplace_back("0");
				tmp.emplace_back(it->str);
			}
			else if (!((it->str == "+") && (tmp.back() == "(")))
			{
				tmp.emplace_back(it->str);
			}
			it = it->nextInside;
		}
	}
	return tmp;
}

bool Postfix::IsOperator(string sym)
{
	if ((sym == "+") || (sym == "-") || (sym == "*") || (sym == "/"))
	{
		return true;
	}
	return false;
}

bool Postfix::BalanceBracket(list<string> prefix)
{
	stack<char> check;
	auto it = prefix.begin();
	//ïîèñê 1-îé ñêîáêè, åñëè îíà çàêðûâàþùàÿñÿ, òî ðàññòàíîâêà íå êîððåêòíà
	while ((it != prefix.end()) && (check.empty()))
	{
		if (*it == "(")
		{
			check.push('(');
		}
		else if (*it == ")")
		{
			return false;
		}
		++it;
	}
	//äîáàâëåíèå âñåõ ñêîáîê â ñòåê
	while (it != prefix.end())
	{
		if (*it == "(")
		{
			check.push('(');
		}
		else if (*it == ")")
		{
			check.push(')');
		}
		++it;
	}

	//åñëè âåðõíÿÿ ñêîáêà îòêðûâàþùàÿñÿ, òî ðàññòàíîâêà íåêîððåêòíà
	if ((!check.empty()) && (check.top() == '('))
		return false;
	int count = 0;
	//ïðîâåðêà áàëàíñà ñêîáîê
	while (!check.empty())
	{
		char c = check.top();
		if (c == ')')
		{
			count--;
		}
		else
		{
			count++;
		}
		check.pop();
	}
	return (count == 0) ? true : false;
}

bool Postfix::CheckOnCorrect(list<string> prefix)
{
	auto it = prefix.begin();
	auto itNext = it;
	++itNext;
	auto itPrev = it;
	if ((*it == "*") || (*it == "/"))
		return false;
	if (!BalanceBracket(prefix))
		return false;
	if ((*it == "(") && ((*(itNext) == "*") || (*(itNext) == "/")))
		return false;
	++it;
	++itNext;
	while (itNext != prefix.end())
	{
		if (*it == "(")
		{
			if (!IsOperator(*itPrev) && *itPrev != "(" && *itPrev != ")") 
				return false;
			else if ((*itNext == "*") || (*itNext == "/"))
				return false;
		}
		if (*it == ")")
		{
			if (!IsOperator(*itNext) && *itNext != "(" && *itNext != ")")
				return false;
			else if (IsOperator(*itPrev))
				return false;
		}
		if ((IsOperator(*it)) && (IsOperator(*itNext)))
			return false;
		++itPrev;
		++it;
		++itNext;
	}
	if (IsOperator(*it))
		return false;
	if ((*it == ")") && (IsOperator(*itPrev)))
		return false;
	return true;
}

int Postfix::PriorityOperator(string s)
{
	if (s == "(")
	{
		return 0;
	}
	else if (s == ")")
	{
		return 1;
	}
	else if ((s == "+") || (s == "-"))
	{
		return 2;
	}
	else if ((s == "/") || (s == "*"))
	{
		return 3;
	}
	throw "uncorrect symbol";
}

void Postfix::SetOperations()
{
	posoperations.insert(make_pair("+", make_pair(Var::_Type::_int, Var::_Type::_int)));
	posoperations.insert(make_pair("+", make_pair(Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(make_pair("+", make_pair(Var::_Type::_double, Var::_Type::_int)));
	posoperations.insert(make_pair("+", make_pair(Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(make_pair("+", make_pair(Var::_Type::_string, Var::_Type::_string)));
	posoperations.insert(make_pair("-", make_pair(Var::_Type::_int, Var::_Type::_int)));
	posoperations.insert(make_pair("-", make_pair(Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(make_pair("-", make_pair(Var::_Type::_double, Var::_Type::_int)));
	posoperations.insert(make_pair("-", make_pair(Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(make_pair("*", make_pair(Var::_Type::_int, Var::_Type::_int)));
	posoperations.insert(make_pair("*", make_pair(Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(make_pair("*", make_pair(Var::_Type::_double, Var::_Type::_int)));
	posoperations.insert(make_pair("*", make_pair(Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(make_pair("/", make_pair(Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(make_pair("+", make_pair(Var::_Type::_string, Var::_Type::_string)));
}

list<string> Postfix::ToPostfix(list<string> prefix)
{
	stack<string> stackPhrase;
	list<string> postfix;
	
	if (prefix.size() > 1)
	{
		if (!CheckOnCorrect(prefix)) //expression validation
			throw "error compile";
	}
	else
	{
		if ((IsOperator(prefix.front())) || (prefix.front() == "(") || (prefix.front() == ")"))
			throw "error compile";
	}
	bool F = true;
	for (auto it = prefix.begin(); it != prefix.end(); ++it)
	{
		if ((IsOperator(*it)) || (*it == "("))
		{
			if (F)
			{
				F = false;
			}
			if ((stackPhrase.empty()) || (*it == "("))
			{
				stackPhrase.push(*it);
			}
			else
			{
				if (Postfix::PriorityOperator(*it) > Postfix::PriorityOperator(stackPhrase.top()))
				{
					stackPhrase.push(*it);
				}
				else
				{
					while ((!stackPhrase.empty()) && (Postfix::PriorityOperator(stackPhrase.top())) >= Postfix::PriorityOperator(*it))
					{
						postfix.emplace_back(stackPhrase.top());
						stackPhrase.pop();
					}
					stackPhrase.push(*it);
				}
			}
		}
		else if (*it == ")")
		{
			if (F)
			{
				F = false;
			}
			while (stackPhrase.top() != "(")
			{
				postfix.emplace_back(stackPhrase.top());
				stackPhrase.pop();
			}
			stackPhrase.pop();
		}
		else
		{
			postfix.emplace_back(*it);
			F = true;
		}
	}

	while (!stackPhrase.empty())
	{
		postfix.emplace_back(stackPhrase.top());
		stackPhrase.pop();
	}
	return postfix;
}

Var::_Type Postfix::CheckOnCompile(Part* start, Part* end, std::map<std::string, std::pair<Var::_Type, bool>> vars)
{
	SetOperations();//Set possible operations  between types

	list<string> prefix;

	if (start != nullptr)
	{
		prefix = ToList(start, end);
	}
	else
	{
		throw "compile error";
	}
	//if there are no symbols in line after "=" or ":="
	list<string> postfix = ToPostfix(prefix);

	stack<Var::_Type> tmpOperand;
	Var::_Type tmp;
	auto it = postfix.begin();
	while ((it) != postfix.end())
	{
		if (IsOperator(*it))
		{
			bool Oper1find = false;
			bool Oper2find = false;
			Var::_Type Oper1, Oper2;
			if (!tmpOperand.empty())
			{
				Oper2 = tmpOperand.top(); 
				tmpOperand.pop();
				Oper2find = true;
			}
			if (!tmpOperand.empty())
			{
				Oper1 = tmpOperand.top(); 
				tmpOperand.pop();
				Oper1find = true;
			}
			if (!(Oper1find && Oper2find))
				throw "can't compile";

			auto iterMap = posoperations.equal_range(*it);
			bool F = false;
			for (auto itrMap = iterMap.first; itrMap != iterMap.second; ++itrMap)
			{
				if (itrMap->second == make_pair(static_cast<Var::_Type>(Oper1), static_cast<Var::_Type>(Oper2)))
				{
					if (Oper1 == Var::_Type::_double || Oper2 == Var::_Type::_double)
					{
						tmp = Var::_Type::_double;
					}
					else
					{
						tmp = Oper1;
					}
					F = true;
					break;
				}
			}
			if (!F)
			{
				throw "can't compile";
			}
			tmpOperand.push(tmp);
		}
		else
		{
			auto v = vars.find(*it);
			if (v != vars.end())
			{
				tmpOperand.push((v->second).first);
			}
			else
			{
				Var* va = Var::Assign(*it);
				tmpOperand.push(va->GetType());
			}
		}
		it++;
	}
	Var::_Type var;
	if (tmpOperand.size() == 1)
	{
		var = tmpOperand.top(); //TODO:: DELETE 
		tmpOperand.pop();
	}
	else
	{
		throw "error compile";
	}
	return var;
}
//Var* Postfix::Calculate(Part* start, Part* end, std::map<std::string, Var*> vars)

Var* Postfix::Calculate(Part* start, Part* end, std::map<std::string, pair<Var, bool>> vars)
{
	list<string> prefix = ToList(start, end);
	//if there are no symbols in line after "=" or ":="
	
	list<string> postfix = ToPostfix(prefix);
	string str = "";
	bool F = true;
	stack<Var*> tmpOperand;
	Var* tmp = nullptr;
	auto it = postfix.begin();
	while ((it) != postfix.end())
	{
		if (IsOperator(*it))
		{
			bool Oper1find = false;
			bool Oper2find = false;
			Var *Oper1, *Oper2;
			if (!tmpOperand.empty())
			{
				Oper2 = tmpOperand.top();
				tmpOperand.pop();
				Oper2find = true;
			}
			if (!tmpOperand.empty())
			{
				Oper1 = tmpOperand.top();
				tmpOperand.pop();
				Oper1find = true;
			}
			if (!(Oper1find && Oper2find))
				throw "False";
			if (*it == "+")
			{
				//tmp = Oper1 + Oper2;
			}
			else if (*it == "-")
			{
				//tmp = Oper1 - Oper2;
			}
			else if (*it == "*")
			{
				//tmp = Oper1 * Oper2;
			}
			else if (*it == "/")
			{
				//tmp = Oper1 / Oper2;
			}
			tmpOperand.push(tmp);
		}
		else
		{
			auto v = vars.find(*it);
			if (v != vars.end())
			{
				tmpOperand.push(&(v->second).first);
			}
			else
			{
				tmpOperand.push(Var::Assign(*it));
			}
		}
		it++;
	}
	return tmp;
}
