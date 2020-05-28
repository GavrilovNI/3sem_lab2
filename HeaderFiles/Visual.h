#pragma once
#include <string>
#include "TConsole.h"
#include <list>


class Visual
{
private:
	char allowedChars[1]{ '.' };

	enum class menus
	{
		EditProgram,
		CompileAndRunProgram,
		Exit,
		menusCount
	};

	std::string menuStrings[(int)menus::menusCount] = {
		"Создать|редактировать программу",
		"Запустить программу",
		"Выйти"
	};

	TConsole console;
	Vector2i consoleSize;

	menus curMenu;

	bool OnMenuPressed();
	void InputOnMenu();
	void UpdateMenu();

	std::string InputStr(std::string info, bool goBack);

	Vector2i prevMenuPos;

	std::list<std::string> prevInputs;
public:
	Visual();

	

};

