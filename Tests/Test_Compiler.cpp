#include<gtest/gtest.h>

#include "../HeaderFiles/Compiler.h"

#include <iostream>
#include <sstream> 

TEST(Compiler, default_constructor_gives_no_exception)
{
	ASSERT_NO_THROW(Compiler c);
}

TEST(Compiler, can_compile_simple_program)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p;"));
}

TEST(Compiler, can_compile_without_program_name)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile(""));
	ASSERT_NO_THROW(c.Compile("begin end."));
	ASSERT_NO_THROW(c.Compile("var a:integer; begin end."));
}

TEST(Compiler, cant_compile_bad_simple_program)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p"));
	ASSERT_ANY_THROW(c.Compile("program ;"));
	ASSERT_ANY_THROW(c.Compile("program"));
	ASSERT_ANY_THROW(c.Compile("progtam p;"));
}

TEST(Compiler, can_compile_program_body)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; begin end."));
}

TEST(Compiler, can_compile_const_creation)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; const i=1; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; const i=1.2; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; const i='asd'; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; const i=true; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; const i=true; x=2; begin end."));
}

TEST(Compiler, can_compile_var_creation)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; var i:integer; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var i:double; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var i:string; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var i:boolean; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var i2:integer; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var i,b:integer; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var i,b2:integer; begin end."));
}

TEST(Compiler, cant_compile_var_creation_like_const)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; var i=5; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var i=5:integer; begin end."));
}
TEST(Compiler, cant_compile_const_creation_like_var)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; const i:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const i; begin end."));
}

TEST(Compiler, cant_compile_stupid_var_creation)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; var iinteger; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var i:; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var i,:; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var i,=:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var i=b:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var ,b:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var 2b:integer; begin end."));
}

TEST(Compiler, cant_compile_stupid_const_creation)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; const i=; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const i=str; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const i2; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const i2 begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const i2,e2=2; begin end."));
}

TEST(Compiler, cant_compile_var_or_const_creating_with_existing_name)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; const a=2; a=3; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a,a:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a,b,a:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; a:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; var a:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const a=2; var a:integer; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; const a=2; begin end."));
}

TEST(Compiler, can_compile_const_creation_equal_another_const)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; const a=2; b=a; begin end."));
}

TEST(Compiler, cant_compile_const_creation_equal_not_created_const)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; const a=b; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const a=b; b=2; begin end."));
}

TEST(Compiler, can_compile_var_setting)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin a:=2; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a,b:integer; begin b:=a; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a,b:integer; begin a:=-4; b:=a; end."));
}

TEST(Compiler, cant_compile_const_resetting)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; const a=2; begin a=1; end."));
	ASSERT_ANY_THROW(c.Compile("program p; const a=2; begin a:=1; end."));
	ASSERT_ANY_THROW(c.Compile("program p; const a=2.5; begin a:=3; end."));
}

TEST(Compiler, can_compile_var_and_const_creating_splited)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; const a=2; const b=2; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; b:integer; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; const a=2; var b:integer; const c=2; begin end."));
	ASSERT_NO_THROW(c.Compile("program p; var c:integer; const d=2; var e:integer; begin end."));
}

TEST(Compiler, can_compile_variable_casting)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; var a:integer; b:double; begin b:=a; end."));
}

TEST(Compiler, cant_compile_bad_variable_casting)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; b:double; begin a:=b; end."));
}

TEST(Compiler, can_compile_counting)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin a:=2+1; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin a:=2-1+3 mod (7*5); end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin a:=3 mod 2; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a,b,c:integer; begin a:=b+(c mod a*(4+5)) ; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a,b:double; const c=5; begin a:=b+(5*3/4+c / a*(4+5)) ; end."));
}

TEST(Compiler, cant_divide_by_zero)
{
	Compiler c;
	c.Compile("program p; var a:double; begin a:=2/0; end.");
	ASSERT_ANY_THROW(c.Run());
	c.Compile("program p; var a:double; begin a:=2 mod 0; end.");
	ASSERT_ANY_THROW(c.Run());

	c.Compile("program p; var a:double; const b=0.0; begin a:=2 / b; end.");
	ASSERT_ANY_THROW(c.Run());
}

TEST(Compiler, cant_compile_with_bad_pairs)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; const a='d; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; const a=d'; begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; begin "));
	ASSERT_ANY_THROW(c.Compile("program p; end."));
	ASSERT_ANY_THROW(c.Compile("program p; end"));
	ASSERT_ANY_THROW(c.Compile("program p; begin end"));
	ASSERT_ANY_THROW(c.Compile("program p; begin begin end."));
	ASSERT_ANY_THROW(c.Compile("program p; begin end end."));
	ASSERT_ANY_THROW(c.Compile("program p; begin end. end."));
	ASSERT_ANY_THROW(c.Compile("program p; begin begin end end end."));
	ASSERT_ANY_THROW(c.Compile("program p; ("));
	ASSERT_ANY_THROW(c.Compile("program p; )"));
	ASSERT_ANY_THROW(c.Compile("program p; ()"));
}

TEST(Compiler, can_compile_if)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin if true then a:=a; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin if true then begin end; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin if true then a:=a else a:=a; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end else a:=a; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end else if false then a:=a; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end else if false then begin end; end."));
	ASSERT_NO_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end else if false then begin end else a:=a; end."));
}

TEST(Compiler, cant_compile_bad_if)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then ; end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then a:=a else ; end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then a:=a end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then a:=a; else a:=a; end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin end end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin else a:=a end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end; else a:=a; end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then a:=a else a:=a end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin end; else a:=a end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin end else a:=a else a:=a end."));

	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end else if false then a:=a end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end else if false then begin end end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end; else if false then begin end; end."));
	ASSERT_ANY_THROW(c.Compile("program p; var a:integer; begin if true then begin a:=a; end else if false then begin end else end."));
}

TEST(Compiler, can_compile_functions)
{
	Compiler c;

	ASSERT_NO_THROW(c.Compile("begin read(); end."));
	ASSERT_NO_THROW(c.Compile("begin readln(); end."));
	ASSERT_NO_THROW(c.Compile("begin write(); end."));
	ASSERT_NO_THROW(c.Compile("begin writeln(); end."));

	ASSERT_NO_THROW(c.Compile("const c=2; var i:integer; begin read(i); end."));
	ASSERT_NO_THROW(c.Compile("const c=2; var i:integer; begin read(i,i,i,i,i); end."));
	ASSERT_NO_THROW(c.Compile("const c=2; var i:integer; begin readln(i); end."));
	ASSERT_NO_THROW(c.Compile("const c=2; var i:integer; begin write(i,c,i); end."));
	ASSERT_NO_THROW(c.Compile("const c=2; var i:integer; begin writeln(c); end."));
	ASSERT_NO_THROW(c.Compile("const c=2; var i:integer; begin writeln(i); end."));
}

TEST(Compiler, cant_compile_bad_functions)
{
	Compiler c;

	ASSERT_ANY_THROW(c.Compile("begin read() end."));
	ASSERT_ANY_THROW(c.Compile("begin read end."));
	ASSERT_ANY_THROW(c.Compile("begin read; end."));

	ASSERT_ANY_THROW(c.Compile("const c=2; var i:integer; begin read(c); end."));
	ASSERT_ANY_THROW(c.Compile("const c=2; var i:integer; begin read(i,i,c,i,i); end."));
	ASSERT_ANY_THROW(c.Compile("const c=2; var i:integer; begin readln(x); end."));
	ASSERT_ANY_THROW(c.Compile("const c=2; var i:integer; begin write(x); end."));
}

TEST(Compiler, test_function_write)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin write(); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

	geek.str("");
	c.Compile("begin write('qwe'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "qwe");

	geek.str("");
	c.Compile("begin write('qwe',2); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "qwe2");

	geek.str("");
	c.Compile("var i:integer; begin i:=100; write(i); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "100");

	geek.str("");
	c.Compile("const c=5; var i:integer; begin i:=100; write(i,c,'str'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1005str");

}

TEST(Compiler, test_function_writeln)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin writeln(); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "\n");

	geek.str("");
	c.Compile("begin writeln('qwe'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "qwe\n");

	geek.str("");
	c.Compile("begin writeln('qwe',2); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "qwe2\n");

	geek.str("");
	c.Compile("var i:integer; begin i:=100; writeln(i); writeln(i+5); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "100\n105\n");

	geek.str("");
	c.Compile("const c=5; var i:integer; begin i:=100; writeln(i,'str'); writeln(c); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "100str\n5\n");

}


TEST(Compiler, test_function_read)
{
	Compiler c;

	std::streambuf* backupCIN = std::cin.rdbuf();
	std::streambuf* backupCOUT = std::cout.rdbuf();

	
	std::istringstream ncin;
	std::ostringstream ncout;

	c.Compile("var i:integer; begin read(i); write(i); end.");
	ncin.str("12");
	ncout.str("");
	std::cin.rdbuf(ncin.rdbuf());
	std::cout.rdbuf(ncout.rdbuf());
	c.Run();
	std::cin.rdbuf(backupCIN);
	std::cout.rdbuf(backupCOUT);
	ASSERT_EQ(ncout.str(), "12");

	c.Compile("var i,j:integer; begin read(i,j); write(i,j); end.");
	ncin.str("12\n13");
	ncout.str("");
	std::cin.rdbuf(ncin.rdbuf());
	std::cout.rdbuf(ncout.rdbuf());
	c.Run();
	std::cin.rdbuf(backupCIN);
	std::cout.rdbuf(backupCOUT);
	ASSERT_EQ(ncout.str(), "1213");
}

TEST(Compiler, test_function_readln)
{
	Compiler c;

	std::streambuf* backupCIN = std::cin.rdbuf();
	std::streambuf* backupCOUT = std::cout.rdbuf();


	std::istringstream ncin;
	std::ostringstream ncout;

	c.Compile("var i:integer; begin readln(i); write(i); end.");
	ncin.str("12");
	ncout.str("");
	std::cin.rdbuf(ncin.rdbuf());
	std::cout.rdbuf(ncout.rdbuf());
	c.Run();
	std::cin.rdbuf(backupCIN);
	std::cout.rdbuf(backupCOUT);
	ASSERT_EQ(ncout.str(), "\n12");

	c.Compile("var i,j:integer; begin readln(i,j); write(i,j); end.");
	ncin.str("12\n13");
	ncout.str("");
	std::cin.rdbuf(ncin.rdbuf());
	std::cout.rdbuf(ncout.rdbuf());
	c.Run();
	std::cin.rdbuf(backupCIN);
	std::cout.rdbuf(backupCOUT);
	ASSERT_EQ(ncout.str(), "\n1213");

	c.Compile("var i,j:integer; begin readln(i); readln(j); write(i,j); end.");
	ncin.str("12\n13");
	ncout.str("");
	std::cin.rdbuf(ncin.rdbuf());
	std::cout.rdbuf(ncout.rdbuf());
	c.Run();
	std::cin.rdbuf(backupCIN);
	std::cout.rdbuf(backupCOUT);
	ASSERT_EQ(ncout.str(), "\n\n1213");
}

TEST(Compiler, if_works_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin if true then write('1'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1");

	geek.str("");
	c.Compile("begin if true then begin write('2'); end; end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "2");

	geek.str("");
	c.Compile("begin if false then write('3'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

	geek.str("");
	c.Compile("begin if false then begin write('4'); end; end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

}

TEST(Compiler, else_works_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin if true then write('1') else write('2'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1");

	geek.str("");
	c.Compile("begin if false then write('3') else write('4'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "4");

	
	geek.str("");
	c.Compile("begin if true then write('5') else begin write('6'); end; end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "5");

	geek.str("");
	c.Compile("begin if false then write('7') else begin write('8'); end; end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "8");

}


TEST(Compiler, else_if_works_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin if true then write('1') else if true then write('2'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1");

	geek.str("");
	c.Compile("begin if false then write('3') else if true then write('4'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "4");

	geek.str("");
	c.Compile("begin if true then write('5') else if false then write('6'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "5");

	geek.str("");
	c.Compile("begin if false then write('7') else if false then write('8'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

}


TEST(Compiler, else_if_else_works_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin if true then write('1') else if true then write('2') else write('3'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1");

	geek.str("");
	c.Compile("begin if false then write('4') else if true then write('5') else write('6'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "5");

	geek.str("");
	c.Compile("begin if false then write('7') else if false then write('8') else write('9'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "9");

	geek.str("");
	c.Compile("begin if true then write('q') else if false then write('w') else write('e'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "q");
}

TEST(Compiler, if_in_if_works_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin if true then if true then write('1'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1");

	geek.str("");
	c.Compile("begin if false then if true then write('2'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

	geek.str("");
	c.Compile("begin if true then if false then write('3'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

	geek.str("");
	c.Compile("begin if false then if false then write('4'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

}

TEST(Compiler, if_else_in_if_works_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin if true then if true then write('1') else write('2'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1");

	geek.str("");
	c.Compile("begin if false then if true then write('3') else write('4'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

	geek.str("");
	c.Compile("begin if true then if false then write('5') else write('6'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "6");

	geek.str("");
	c.Compile("begin if false then if false then write('7') else write('8'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "");

}

TEST(Compiler, if_else_in_if_else_works_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("begin if true then if true then write('1') else write('2') else write('3'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1");

	geek.str("");
	c.Compile("begin if false then if true then write('4') else write('5') else write('6'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "6");

	geek.str("");
	c.Compile("begin if true then if false then write('7') else write('8') else write('9'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "8");

	geek.str("");
	c.Compile("begin if false then if false then write('q') else write('w') else write('e'); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "e");

}

TEST(Compiler, vars_sets_right)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	c.Compile("var i:integer; d:double; b:boolean; s:string; begin i:=123; d:=1.5; b:=true; s:='asd'; write(i); write(d); write(b); write(s); end.");
	std::cout.rdbuf(geek.rdbuf());
	c.Run();
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1231.500trueasd");
}

TEST(Compiler, int_can_cast_to_double)
{
	Compiler c;

	std::ostringstream geek;
	std::streambuf* coutbuf = std::cout.rdbuf();

	geek.str("");
	ASSERT_NO_THROW(c.Compile("var d:double; i:integer; begin i:=1; d:=i; write(d); end."));
	std::cout.rdbuf(geek.rdbuf());
	ASSERT_NO_THROW(c.Run());
	std::cout.rdbuf(coutbuf);
	ASSERT_EQ(geek.str(), "1.000");
}

TEST(Compiler, cant_compile_if_double_casting_to_int)
{
	Compiler c;
	ASSERT_ANY_THROW(c.Compile("var d:double; i:integer; begin d:=1.5; i:=d; write(i); end."));
}