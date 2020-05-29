#include<gtest/gtest.h>
#include<string> 
#include <map>
#include "../HeaderFiles/Compiler.h"
#include "../HeaderFiles/Postfix.h"
#include "../HeaderFiles/Var.h"

//Compiler c;
std::map<std::string, std::pair<Var::_Type, bool>> vars;

TEST(Postfix, no_throw_if_check_compile)
{
	std::string str = "program xs; const x = 5 * 5 + 4; begin end."; 
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_NO_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}

TEST(Postfix, can_check_compile)
{
	std::string str = "program xs; const x = 5 * 5 + 4; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	Var::_Type v = Postfix::CheckOnCompile(p->nextInside, p->next, vars);
	ASSERT_EQ(Var::_Type::_int, v);
}

TEST(Postfix, can_check_compile_logical_expression)
{
	std::string str = "program xs; const x = 5 < 4 AND 6 > 3 OR NOT true; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_NO_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
	Var::_Type v = Postfix::CheckOnCompile(p->nextInside, p->next, vars);
	ASSERT_EQ(Var::_Type::_bool, v);
}

TEST(Postfix, error_if_uncorrect_bracket)
{
	std::string str = "program xs; const x = 5 * ) + 5 + 4; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_ANY_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}

TEST(Postfix, error_if_uncorrect_bracket_2)
{
	std::string str = "program xs; const x = (5 * 5)() + 4; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_ANY_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}

TEST(Postfix, error_if_uncorrect_sign)
{
	std::string str = "program xs; const x = 5 * / 5 + 4; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_ANY_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}

TEST(Postfix, error_if_uncorrect_operator)
{
	std::string str = "program xs; const x = 5 n 5 + 4; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_ANY_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}

TEST(Postfix, error_if_uncorrect_cast_types)
{
	std::string str = "program xs; const x = 5 + sae + 4; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_ANY_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}

TEST(Postfix, no_error_if_int_divisible_on_int)
{
	std::string str = "program xs; const x = 5 / 3 + 4; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_NO_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}

TEST(Postfix, can_compile_expression)
{
	std::string str = "program xs; const x = 7 < 2 AND 4 > 3 AND not true; begin end.";
	Part* p = CompilerUtility::SplitStr(str);
	p = p->next->next->nextInside->next;
	ASSERT_NO_THROW(Postfix::CheckOnCompile(p->nextInside, p->next, vars));
}
