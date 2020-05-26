#pragma once
#include "Var.h"
#include <map>
#include <list>

#include "Part.h"

using namespace std;

inline multimap<string, pair<Var::_Type, Var::_Type>> posoperations;

static class Postfix
{
private:
	static list<string> ToList(Part* start, Part* end); //convert to list operands
public:

	static bool IsOperator(string sym); 
	static bool BalanceBracket(list<string> prefix); //bracket validation
	static bool CheckOnCorrect(list<string> prefix); //expression validation
	static int PriorityOperator(string s);
	
	static void SetOperations();	//set possible operations
	static list<string> ToPostfix(list<string> prefix); //convert to postfix
	static Var::_Type CheckOnCompile(Part* start, Part* end, std::map<std::string, std::pair<Var::_Type, bool>> vars);
	static Var* Calculate(Part* start, Part* end, std::map<std::string, pair<Var, bool>> vars);
};

