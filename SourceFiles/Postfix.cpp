#include "../HeaderFiles/Postfix.h"
#include "../HeaderFiles/CompilerExceptions.h"
#include <string>
#include <list>
#include <stack>
#include <iostream>


std::list<std::string> Postfix::ToList(Part* start, Part* end)
{
	std::list<std::string> tmp;
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

bool Postfix::IsOperator(std::string sym)
{
	std::list<std::string> operations{
	"*",
	"/",
	"+",
	"-",
	"<>",
	"=",
	">",
	">=",
	"<",
	"<=",
	"and",
	"or",
	"not",
	"mod",
	"div"
	};
	return (find(operations.begin(), operations.end(), sym) != operations.end());
}

bool Postfix::BalanceBracket(std::list<std::string> prefix)
{
	std::stack<char> check;
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

bool Postfix::CheckOnCorrect(std::list<std::string> prefix)
{
	auto it = prefix.begin();
	auto itNext = it;
	++itNext;
	auto itPrev = it;
	if (IsOperator(*it) && *it != "+" && *it != "-" && *it!="not") 
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
			if (!IsOperator(*itPrev) && *itPrev != "(") 
				return false;
			else if ((*itNext == "*") || (*itNext == "/"))
				return false;
		}
		if (*it == ")")
		{
			if (!IsOperator(*itNext) && *itNext != ")" && *itNext!="not")
				return false;
			else if (IsOperator(*itPrev))
				return false;
		}
		//if 2 operators in a row and not "and not" or "or not"
		if ((IsOperator(*it)) && (IsOperator(*itNext)) && !((*itNext == "not") && (*it == "and" || *it == "or")))
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

int Postfix::PriorityOperator(std::string s)
{
	if ((s == "=") || (s == "<>") || (s == "<") || (s == ">") || (s == "<=") || (s == ">="))
	{
		return 3;
	} 
	else if (s == "(")
	{
		return -1;
	}
	else if (s == ")")
	{
		return 0;
	}
	else if (s == "or")
	{
		return 1;
	}
	else if (s == "and")
	{
		return 2;
	}
	else if ((s == "+") || (s == "-"))
	{
		return 4;
	}
	else if ((s == "/") || (s == "*") || (s == "div") || (s == "mod"))
	{
		return 5;
	}
	else if (s == "not")
	{
		return 6;
	}
	throw CompilerExc("uncorrect symbol");
}

void Postfix::SetOperations()
{
	//tuple.first - left operand, tuple.second - rignt operand, tuple.third - result
	posoperations.insert(std::make_pair("+", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_int)));
	posoperations.insert(std::make_pair("+", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(std::make_pair("+", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(std::make_pair("+", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(std::make_pair("+", std::make_tuple(Var::_Type::_string, Var::_Type::_string, Var::_Type::_string)));

	posoperations.insert(std::make_pair("-", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_int)));
	posoperations.insert(std::make_pair("-", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(std::make_pair("-", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(std::make_pair("-", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_double)));

	posoperations.insert(std::make_pair("*", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_int)));
	posoperations.insert(std::make_pair("*", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(std::make_pair("*", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(std::make_pair("*", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_double)));

	posoperations.insert(std::make_pair("/", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(std::make_pair("/", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_double)));
	posoperations.insert(std::make_pair("/", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(std::make_pair("/", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_double)));
	posoperations.insert(std::make_pair("mod", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_int)));
	posoperations.insert(std::make_pair("div", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_int)));

	posoperations.insert(std::make_pair("=", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("=", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("=", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_bool))); 
	posoperations.insert(std::make_pair("=", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("=", std::make_tuple(Var::_Type::_bool, Var::_Type::_bool, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("=", std::make_tuple(Var::_Type::_string, Var::_Type::_string, Var::_Type::_bool)));

	posoperations.insert(std::make_pair("<>", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<>", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<>", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_bool))); 
	posoperations.insert(std::make_pair("<>", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<>", std::make_tuple(Var::_Type::_bool, Var::_Type::_bool, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<>", std::make_tuple(Var::_Type::_string, Var::_Type::_string, Var::_Type::_bool)));

	posoperations.insert(std::make_pair("<", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<", std::make_tuple(Var::_Type::_string, Var::_Type::_string, Var::_Type::_bool)));

	posoperations.insert(std::make_pair("<=", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<=", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<=", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_bool))); 
	posoperations.insert(std::make_pair("<=", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("<=", std::make_tuple(Var::_Type::_string, Var::_Type::_string, Var::_Type::_bool)));

	posoperations.insert(std::make_pair(">=", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair(">=", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair(">=", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_bool))); 
	posoperations.insert(std::make_pair(">=", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_bool)));
	posoperations.insert(std::make_pair(">=", std::make_tuple(Var::_Type::_string, Var::_Type::_string, Var::_Type::_bool)));

	posoperations.insert(std::make_pair(">", std::make_tuple(Var::_Type::_int, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair(">", std::make_tuple(Var::_Type::_double, Var::_Type::_int, Var::_Type::_bool)));
	posoperations.insert(std::make_pair(">", std::make_tuple(Var::_Type::_int, Var::_Type::_double, Var::_Type::_bool))); 
	posoperations.insert(std::make_pair(">", std::make_tuple(Var::_Type::_double, Var::_Type::_double, Var::_Type::_bool)));
	posoperations.insert(std::make_pair(">", std::make_tuple(Var::_Type::_string, Var::_Type::_string, Var::_Type::_bool)));

	posoperations.insert(std::make_pair("and", std::make_tuple(Var::_Type::_bool, Var::_Type::_bool, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("or", std::make_tuple(Var::_Type::_bool, Var::_Type::_bool, Var::_Type::_bool)));
	posoperations.insert(std::make_pair("not", std::make_tuple(Var::_Type::_void, Var::_Type::_bool, Var::_Type::_bool)));

}

std::list<std::string> Postfix::ToPostfix(std::list<std::string> prefix)
{
	std::stack<std::string> stackPhrase;
	std::list<std::string> postfix;
	
	if (prefix.size() > 1)
	{
		if (!CheckOnCorrect(prefix)) //expression validation
			throw CompilerExc("uncorrect expression");
	}
	else
	{
		if ((IsOperator(prefix.front())) || (prefix.front() == "(") || (prefix.front() == ")"))
			throw CompilerExc("uncorrect expression");
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

	std::list<std::string> prefix;

	if (start != nullptr)
	{
		prefix = ToList(start, end);
	}
	else
	{
		throw CompilerExc("uncorrect expression");
	}
	//if there are no symbols in line after "=" or ":="
	std::list<std::string> postfix = ToPostfix(prefix);

	std::stack<Var::_Type> tmpOperand;
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
			if (!tmpOperand.empty() || *it == "not")
			{
				if (*it != "not")
				{
					Oper1 = tmpOperand.top();
					tmpOperand.pop();
				}
				else
				{
					Oper1 = Var::_Type::_void;
				}
				Oper1find = true;
			}
			if (!(Oper1find && Oper2find))
				throw CompilerExc("uncorrect expression");

			auto iterMap = posoperations.equal_range(*it);
			bool F = false;
			for (auto itrMap = iterMap.first; itrMap != iterMap.second; ++itrMap)
			{
				auto pair = std::make_pair(std::get<0>(itrMap->second), std::get<1>(itrMap->second));
				if (pair == std::make_pair(Oper1, Oper2))
				{
					tmp = std::get<2>(itrMap->second);
					F = true;
					break;
				}
			}
			if (!F)
			{
				throw CompilerExc("uncorrect expression");
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
		throw CompilerExc("uncorrect expression");
	}
	return var;
}
//Var* Postfix::Calculate(Part* start, Part* end, std::map<std::string, Var*> vars)

Var* Postfix::Calculate(Part* start, Part* end, TableHash& vars)
{
	std::list<std::string> prefix = ToList(start, end);
	//if there are no symbols in line after "=" or ":="
	
	std::list<Var*> toDelete;

	std::list<std::string> postfix = ToPostfix(prefix);
	//string str = "";
	bool F = true;
	std::stack<Var*> tmpOperand;
	Var* tmp;
	auto it = postfix.begin();
	while ((it) != postfix.end())
	{
		if (IsOperator(*it))
		{
			bool Oper1find = false;
			bool Oper2find = false;
			Var *Oper1 = nullptr, *Oper2 = nullptr;
			if (!tmpOperand.empty())
			{
				Oper2 = tmpOperand.top();
				tmpOperand.pop();
				Oper2find = true;
			}
			if (!tmpOperand.empty() || *it == "not")
			{
				if (*it != "not")
				{
					Oper1 = tmpOperand.top();
					tmpOperand.pop();
				}
				Oper1find = true;
			}
			if (!(Oper1find && Oper2find))
				throw CompilerExc("uncorrect expression");
			if (*it == "+")
			{
				tmp = *Oper1 + Oper2;
			}
			else if (*it == "-")
			{
				tmp = *Oper1 - Oper2;
			}
			else if (*it == "*")
			{
				tmp = *Oper1 * Oper2;
			}
			else if (*it == "/")
			{
				tmp = *Oper1 / Oper2;
			}
			else if (*it == "<>")
			{
				tmp = *Oper1 != Oper2;
			}
			else if (*it == "=")
			{
				tmp = *Oper1 == Oper2;
			}
			else if (*it == ">")
			{
				tmp = *Oper1 > Oper2;
			}
			else if (*it == ">=")
			{
				tmp = *Oper1 >= Oper2;
			}				
			else if (*it == "<")
			{
				tmp = *Oper1 < Oper2;
			}
			else if (*it == "<=")
			{
				tmp = *Oper1 <= Oper2;
			}
			else if (*it == "and")
			{
				tmp = *Oper1 && Oper2;
			}
			else if (*it == "or")
			{
				tmp = *Oper1 || Oper2;

			}
			else if (*it == "not")
			{
				tmp = !(*Oper2);
			}
			else if (*it == "mod")
			{
				tmp = *Oper1 % Oper2;
			}
			else if (*it == "div")
			{
				tmp = dynamic_cast<_Int*>(Oper1)->Div(Oper2);
			}
			toDelete.push_back(tmp);
			tmpOperand.push(tmp);
		}
		else
		{
			bool F = false;
			auto v = vars.Find(*it);
			//auto v = vars.find(*it);
			if (v != nullptr)
			{
				tmpOperand.push(v->first);
				F = true;
			}
			else
			{
				Var* _var = Var::Assign(*it);
				toDelete.push_back(_var);
				tmpOperand.push(_var);
				F = true;
			}
			if (!F)
			{
				throw CompilerExc("uncorrect expression");
			}
		}
		it++;
	}



	Var* result = Var::CreateCopy(tmpOperand.top());

	for (auto it = toDelete.begin(); it != toDelete.end(); it++)
	{
		delete (*it);
	}

	return result;
}
