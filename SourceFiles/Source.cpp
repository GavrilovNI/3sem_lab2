#include <string>
#include <iostream>
#include "../HeaderFiles/Compiler.h"
#include <fstream>
#include <streambuf>
#include <typeindex>
#include <any>

#include<variant>

using namespace std;
/*
struct Foo
{
	int xx(int x) const { return x; }
   bool operator==(int) const {}
   void operator()(std::string) const {}
};

template<typename T>
struct HasUsedMemoryMethod
{
	template<typename U, int(U::*)(int x) const> struct SFINAE {};
	template<typename U> static char Test(SFINAE<U, &U::xx>*);
	template<typename U> static int Test(...);
	static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
};


class No { bool b[2]; };
template<typename T, typename Arg> No operator== (const T&, const Arg&);

bool Check(...);
No& Check(const No&);

template <typename T, typename Arg = T>
struct EqualExists
{
	enum { value = (sizeof(Check(*(T*)(0) == *(Arg*)(0))) != sizeof(No)) };
};
*/


int main()
{
	std::ifstream t("../prog.pas");
	if (!t.is_open()) return -1;
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	Compiler c;

	try
	{
		c.Compile(str);
		c.Run();
	}
	catch (CompilerExc ex)
	{
		//compile error
		cout << ex.what();
	}
	catch (...)
	{
		throw "";
	}

}