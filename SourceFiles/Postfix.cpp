#include "../HeaderFiles/Postfix.h"

#include <stack>
#include <iostream>

/*bool PostfixCheckOnCompile(Part* part, std::map<std::string, Var> vars);
part  = :=
Var* Postfix(...);*/

list<string> Postfix::ToList(Part* part)
{
	list<string> tmp;
	auto it = part;
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
		if (it->str == ";")
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
	posoperations.insert(make_pair("+", make_pair(_int, _int)));
	posoperations.insert(make_pair("+", make_pair(_int, _double)));
	posoperations.insert(make_pair("+", make_pair(_double, _int)));
	posoperations.insert(make_pair("+", make_pair(_double, _double)));
	posoperations.insert(make_pair("+", make_pair(_string, _string)));
	posoperations.insert(make_pair("-", make_pair(_int, _int)));
	posoperations.insert(make_pair("-", make_pair(_int, _double)));
	posoperations.insert(make_pair("-", make_pair(_double, _int)));
	posoperations.insert(make_pair("-", make_pair(_double, _double)));
	posoperations.insert(make_pair("*", make_pair(_int, _int)));
	posoperations.insert(make_pair("*", make_pair(_int, _double)));
	posoperations.insert(make_pair("*", make_pair(_double, _int)));
	posoperations.insert(make_pair("*", make_pair(_double, _double)));
	posoperations.insert(make_pair("/", make_pair(_double, _double)));
	posoperations.insert(make_pair("+", make_pair(_string, _string)));
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

Var* Postfix::CheckOnCompile(Part* part, std::map<std::string, Var*> vars)
{
	SetOperations();//Set possible operations  between types

	list<string> prefix;
	if (part->next->str != ";")
	{
		throw "expected \";\" ";
	}
	if (part->nextInside != nullptr)
	{
		prefix = ToList(part->nextInside);
	}
	else
	{
		throw "compile error";
	}
	//if there are no symbols in line after "=" or ":="
	list<string> postfix = ToPostfix(prefix);

	stack<_Type> tmpOperand;
	_Type tmp;
	auto it = postfix.begin();
	while ((it) != postfix.end())
	{
		if (IsOperator(*it))
		{
			bool Oper1find = false;
			bool Oper2find = false;
			_Type Oper1, Oper2;
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
				if (itrMap->second == make_pair(static_cast<_Type>(Oper1 % 3), static_cast<_Type>(Oper2 % 3)))
				{
					if (Oper1 == _double || Oper2 == _double)
					{
						tmp = _double;
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
				tmpOperand.push(v->second->GetType());
			}
			else
			{
				Var* va = Var::Assign(*it);
				tmpOperand.push(va->GetType());
			}
		}
		it++;
	}
	Var v;
	if (tmpOperand.size() == 1)
	{
		v.SetType(tmpOperand.top()); //TODO:: DELETE 
		tmpOperand.pop();
	}
	else
	{
		throw "error compile";
	}
	return &v;
}

Var* Postfix::Calculate(Part* part, std::map<std::string, Var*> vars)
{
	list<string> prefix = ToList(part);
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
				tmpOperand.push(v->second);
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
