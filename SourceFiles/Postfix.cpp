#include "../HeaderFiles/Postfix.h"

#include <stack>

/*bool PostfixCheckOnCompile(Part* part, std::map<std::string, Var> vars);
part  = :=
Var* Postfix(...);*/

list<string> Postfix::ToList(Part* part)
{
	list<string> tmp;
	auto it = part;
	while (it->nextInside->str != ";")
	{
		if (it->nextInside != nullptr)
		{
			it = it->nextInside;
			tmp.emplace_back(it->str);	
		}
		else
		{
			it = it->next;
			tmp.emplace_back(it->str);
		}
	}
	return tmp;
}

bool Postfix::IsOperator(string sym)
{
	if ((sym == "+") || (sym == "-") || (sym == "*") || (sym == "/") || (sym == "(") || (sym == ")"))
		return true;
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
	posoperations.insert(make_pair("=", make_pair(_int, _int)));
	posoperations.insert(make_pair("=", make_pair(_double, _int)));
	posoperations.insert(make_pair("=", make_pair(_bool, _bool)));
	posoperations.insert(make_pair("=", make_pair(_string, _string)));
	posoperations.insert(make_pair(":=", make_pair(_int, _int)));
	posoperations.insert(make_pair(":=", make_pair(_double, _int)));
	posoperations.insert(make_pair(":=", make_pair(_bool, _bool)));
	posoperations.insert(make_pair(":=", make_pair(_string, _string)));
}

list<string> Postfix::ToPostfix(list<string> prefix)
{
	stack<string> stackPhrase;
	list<string> postfix;

	if (!BalanceBracket(prefix))
		throw "bracket unpaired";
	
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

bool Postfix::CheckOnCompile(Part* part, std::map<std::string, Var*> vars)
{
	Class leftOper = Var::Assign(part->prev->str)->GetType(); //operand after "=" or ":="
	SetOperations();//Set possible operations  between types
	list<string> prefix = ToList(part);
	
	//if there are no symbols in line after "=" or ":="
	if (prefix.empty())
		throw "compile error";
	list<string> postfix = ToPostfix(prefix);

	stack<Class> tmpOperand;
	Class tmp;
	auto it = postfix.begin();
	while ((it) != postfix.end())
	{
		if (IsOperator(*it))
		{
			bool Oper1find = false;
			bool Oper2find = false;
			Class Oper1, Oper2;
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
				return false;
			auto iterMap = posoperations.equal_range(*it);
			for (auto itrMap = iterMap.first; itrMap != iterMap.second; ++itrMap)
			{
				if (itrMap->second == make_pair(static_cast<Class>(Oper1 % 3), static_cast<Class>(Oper2 % 3)))
				{
					if (Oper1 == _double || Oper2 == _double)
					{
						tmp = _double;
					}
					else
					{
						tmp = Oper1;
					}
				}
				else
				{
					return false;
				}
			}
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
	auto iterMap = posoperations.equal_range(part->str);
	for (auto itrMap = iterMap.first; itrMap != iterMap.second; ++itrMap)
	{
		if (itrMap->second == make_pair(static_cast<Class>(leftOper % 3), static_cast<Class>(tmp % 3)))
		{
			return true;
		}
	}
	return false;
}

Var* Postfix::Calculate(Part* part, std::map<std::string, Var*> vars)
{
	Var* t = Var::Assign(part->prev->str);

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
