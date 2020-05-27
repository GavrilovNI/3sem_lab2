#pragma once

#include<string>

struct Part
{
	std::string str;
	Part* next;
	Part* nextInside;

	Part* prev;
	//Part* prevInside;
};