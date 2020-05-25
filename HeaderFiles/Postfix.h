#pragma once
#include "Var.h"
#include <map>
#include <list>

#include "Part.h"

using namespace std;

inline multimap<string, pair<_Type, _Type>> posoperations;

static class Postfix
{
private:
	static list<string> ToList(Part* part); //convert to list operands
public:

	static bool IsOperator(string sym); 
	static bool BalanceBracket(list<string> prefix); //bracket validation
	static bool CheckOnCorrect(list<string> prefix); //expression validation
	static int PriorityOperator(string s);
	
	static void SetOperations();	//set possible operations
	static list<string> ToPostfix(list<string> prefix); //convert to postfix
	static Var* CheckOnCompile(Part* part, std::map<std::string, Var*> vars);
	static Var* Calculate(Part* part, std::map<std::string, Var*> vars);
};

