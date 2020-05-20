#pragma once
#include "Var.h"
#include "Compiler.h"
#include <map>
#include <list>
struct Part;

using namespace std;

inline multimap<string, pair<Class, Class>> posoperations;
static class Postfix
{
private:
	static list<string> ToList(Part* part);
public:

	static bool IsOperator(string sym);
	static bool BalanceBracket(list<string> list);
	static int PriorityOperator(string s);

	static void SetOperations();
	static list<string> ToPostfix(list<string> prefix);
	static bool CheckOnCompile(Part* part, std::map<std::string, Var*> vars);
	static Var* Calculate(Part* part, std::map<std::string, Var*> vars);
};

