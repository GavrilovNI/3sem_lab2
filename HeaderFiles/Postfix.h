#pragma once
#include "Var.h"
#include <map>
#include <list>
#include <tuple>

#include "TableHash.h"
#include "Part.h"

//inline multimap<string, pair<Var::_Type, Var::_Type>> posoperations;
inline std::multimap<std::string, std::tuple<Var::_Type, Var::_Type, Var::_Type>> posoperations;

static class Postfix
{
private:
	static std::list<std::string> ToList(Part* start, Part* end); //convert to list operands
public:

	static bool IsOperator(std::string sym);
	static bool BalanceBracket(std::list<std::string> prefix); //bracket validation
	static bool CheckOnCorrect(std::list<std::string> prefix); //expression validation
	static int PriorityOperator(std::string s);
	
	static void SetOperations();	//set possible operations
	static std::list<std::string> ToPostfix(std::list<std::string> prefix); //convert to postfix
	static Var::_Type CheckOnCompile(Part* start, Part* end, std::map<std::string, std::pair<Var::_Type, bool>> vars);
	static Var* Calculate(Part* start, Part* end, TableHash& vars);
};

