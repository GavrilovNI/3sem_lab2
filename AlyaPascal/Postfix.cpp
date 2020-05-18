#include "Postfix.h"

#include <stack>



bool Postfix::IsOperator(Operand op)
{
	return true;
}

bool Postfix::BalanceBracket(list<Operand> operands)
{
	if (operands.empty())
		throw "phrase is empty";
	stack<char> check;
	auto it = operands.begin();
	//ïîèñê 1-îé ñêîáêè, åñëè îíà çàêðûâàþùàÿñÿ, òî ðàññòàíîâêà íå êîððåêòíà
	while ((it != operands.end()) && (check.empty()))
	{
		if ((*it).str == "(")
		{
			check.push('(');
		}
		else if ((*it).str == ")")
		{
			return false;
		}
		++it;
	}
	//äîáàâëåíèå âñåõ ñêîáîê â ñòåê
	while ((it != operands.end()))
	{
		if ((*it).str == "(")
		{
			check.push('(');
		}
		else if ((*it).str == ")")
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

int Postfix::PriorityOperator(Operand op)
{
	string s = op;
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

list<Operand> Postfix::ToPostfix(map<string, Var> vars, list<Operand> operands)
{
	stack<Operand> stackPhrase;
	list<Operand> postfix;

	if (!BalanceBracket(operands))
		throw "bracket unpaired";
	bool F = true;
	for (auto it = operands.begin(); it != operands.end(); ++it)
	{
		//cout << (*it).str << endl;
		/*if (it->str == "integral")
		{
			postfix.emplace_back(*it);
		}
		else */if ((IsOperator(*it)) || (it->str == "("))
		{
			if (F)
			{
				F = false;
			}
			if ((stackPhrase.empty()) || (it->str == "("))
			{
				stackPhrase.push(*it);
			}
			else
			{
				if (Postfix::PriorityOperator(it->str) > Postfix::PriorityOperator(stackPhrase.top().str))
				{
					stackPhrase.push(*it);
				}
				else
				{
					while ((!stackPhrase.empty()) && (Postfix::PriorityOperator(stackPhrase.top().str)) >= Postfix::PriorityOperator(it->str))
					{
						postfix.emplace_back(stackPhrase.top());
						stackPhrase.pop();
					}
					stackPhrase.push(*it);
				}
			}
		}
		else if (it->str == ")")
		{
			if (F)
			{
				F = false;
			}
			while (stackPhrase.top().str != "(")
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
