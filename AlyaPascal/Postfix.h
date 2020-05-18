#pragma once
#include "Var.h"
#include "map"
#include "list"

using namespace std;
static class Postfix
{
public:

	static bool IsOperator(Operand op);
	static bool BalanceBracket(list<Operand> operands);
	static int PriorityOperator(Operand op);
	
	static list<Operand> ToPostfix(map<string, Var> vars, list<Operand> operands);
	
};

