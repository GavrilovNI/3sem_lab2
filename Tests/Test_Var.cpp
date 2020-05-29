#include<gtest/gtest.h>
#include "..\HeaderFiles\Var.h"

TEST(Var, default_constructor_gives_no_exception)
{
	ASSERT_NO_THROW(Var v);
}

TEST(Var, assign_works_properly)
{
	Var* v;
	std::string s = "25.4";
	ASSERT_NO_THROW(v = Var::Assign(s));
	ASSERT_EQ(v->GetType(), Var::_Type::_double);
	ASSERT_EQ(dynamic_cast<_Double*>(v)->value, 25.4);
}

TEST(Var, gettype_works_properly)
{
	Var* v;
	v = Var::Assign("25.4");
	ASSERT_EQ(Var::GetTypeName(v->GetType()), "double");
	v = Var::Assign("true");
	ASSERT_EQ(Var::GetTypeName(v->GetType()), "boolean");
}

TEST(Var, cancast_works_properly)
{
	ASSERT_EQ(Var::CanCast(Var::GetTypeByString("integer"), Var::GetTypeByString("double")), true);
	ASSERT_NE(Var::CanCast(Var::GetTypeByString("double"), Var::GetTypeByString("integer")), true);
}

TEST(Var, cast_works_properly)
{
	Var *v1, *v2;
	v1 = Var::Assign("25.4");
	v2 = Var::Assign("2");
	ASSERT_NO_THROW(Var::Cast(v1, v2));
	ASSERT_ANY_THROW(Var::Cast(v2, v1));
}

TEST(Var, operator_add_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("25.4");
	v2 = Var::Assign("2");
	ASSERT_NO_THROW(v3 = *v1 + v2);
	ASSERT_TRUE(dynamic_cast<_Double*>(v3)->value == 27.4);
}

TEST(Var, operator_substract_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("25.4");
	v2 = Var::Assign("2.3");
	ASSERT_NO_THROW(v3 = *v1 - v2);
	ASSERT_TRUE(23.1 - dynamic_cast<_Double*>(v3)->value <= 0.001);
}

TEST(Var, operator_multiply_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("4");
	v2 = Var::Assign("2.5");
	ASSERT_NO_THROW(v3 = *v1 * v2);
	ASSERT_TRUE(dynamic_cast<_Double*>(v3)->value == 10);
}

TEST(Var, operator_division_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("13.0");
	v2 = Var::Assign("52");
	ASSERT_NO_THROW(v3 = *v1 / v2);
	ASSERT_TRUE(dynamic_cast<_Double*>(v3)->value == 0.25);
}

TEST(Var, operator_div_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("13");
	v2 = Var::Assign("52");
	ASSERT_NO_THROW(v3 = dynamic_cast<_Int*>(v1)->Div(v2));
	ASSERT_TRUE(dynamic_cast<_Int*>(v3)->value == 0);
}

TEST(Var, operator_mod_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("51");
	v2 = Var::Assign("15");
	ASSERT_NO_THROW(v3 = *v1 % v2);
	ASSERT_TRUE(dynamic_cast<_Int*>(v3)->value == 6);
}

TEST(Var, operator_and_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("true");
	v2 = Var::Assign("false");
	ASSERT_NO_THROW(v3 = *v1 && v2);
	ASSERT_FALSE(dynamic_cast<_Bool*>(v3)->value);
	v2 = Var::Assign("true");
	ASSERT_NO_THROW(v3 = *v1 && v2);
	ASSERT_TRUE(dynamic_cast<_Bool*>(v3)->value);
}

TEST(Var, operator_or_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("false");
	v2 = Var::Assign("false");
	ASSERT_NO_THROW(v3 = *v1 || v2);
	ASSERT_FALSE(dynamic_cast<_Bool*>(v3)->value);
	v2 = Var::Assign("true");
	ASSERT_NO_THROW(v3 = *v1 || v2);
	ASSERT_TRUE(dynamic_cast<_Bool*>(v3)->value);
}

TEST(Var, operator_notequal_numeric_works_properly)
{
	Var* v1, * v2;
	v1 = Var::Assign("13.0");
	v2 = Var::Assign("52");
	ASSERT_TRUE((dynamic_cast<_Bool*>(*v1 != v2))->value);
}

TEST(Var, operator_notequal_boolean_works_properly)
{
	Var* v1, * v2;
	v1 = Var::Assign("false");
	v2 = Var::Assign("true");
	ASSERT_TRUE((dynamic_cast<_Bool*>(*v1 != v2))->value);
}

TEST(Var, operator_notequal_string_works_properly)
{
	Var* v1, * v2;
	v1 = Var::Assign("'AA'");
	v2 = Var::Assign("'AB'");
	ASSERT_TRUE((dynamic_cast<_Bool*>(*v1 != v2))->value);
}

TEST(Var, operator_less_works_properly)
{
	Var* v1, * v2;
	v1 = Var::Assign("13.0");
	v2 = Var::Assign("52");
	ASSERT_TRUE((dynamic_cast<_Bool*>(*v1 < v2))->value);
}

TEST(Var, operator_less_or_equal_works_properly)
{
	Var* v1, * v2;
	v1 = Var::Assign("13.0");
	v2 = Var::Assign("52");
	ASSERT_TRUE((dynamic_cast<_Bool*>(*v1 <= v2))->value);
}

TEST(Var, operator_more_works_properly)
{
	Var* v1, * v2;
	v1 = Var::Assign("13.0");
	v2 = Var::Assign("52");
	ASSERT_FALSE((dynamic_cast<_Bool*>(*v1 > v2))->value);
}

TEST(Var, operator_more_or_equal_works_properly)
{
	Var* v1, * v2;
	v1 = Var::Assign("13.0");
	v2 = Var::Assign("52");
	ASSERT_FALSE((dynamic_cast<_Bool*>(*v1 >= v2))->value);
}

TEST(Var, operator_inversion_works_properly)
{
	Var* v;
	v = Var::Assign("false");
	ASSERT_TRUE(dynamic_cast<_Bool*>(!(*v))->value);
}

TEST(Var, operator_concatination_works_properly)
{
	Var* v1, * v2, * v3;
	v1 = Var::Assign("'Ok'");
	v2 = Var::Assign("'ey'");
	ASSERT_NO_THROW(v3 = *v1 + v2);
	ASSERT_EQ(dynamic_cast<_String*>(v3)->value, dynamic_cast<_String*>(Var::Assign("'Okey'"))->value);
}