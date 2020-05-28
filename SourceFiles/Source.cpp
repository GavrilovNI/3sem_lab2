#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include "../HeaderFiles/Compiler.h"
#include <fstream>
#include <streambuf>
#include "../HeaderFiles/Visual.h"
#include <stdlib.h>
#include <stdio.h>
#include <istream>
//#include<windows.h>


//using namespace std;



int main()
{
	Visual visual;
	
	/*std::ifstream t("../prog.pas");
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
		std::cout << ex.what();
	}
	catch (...)
	{
		throw "";
	}*/
	
}