#include<gtest/gtest.h>

#include "../HeaderFiles/Compiler.h"

TEST(Compiler, default_constructor_gives_no_exception)
{
	ASSERT_NO_THROW(Compiler c);
}