#pragma once
#include<string>
#include<list>
//#include<algorithm>

class TextSplitter
{
public:
	static std::list<std::string> Split(std::string str, std::string chars[], int charsLength)
	{
		std::list<std::string> result;

		std::string word = "";

		for (int i = 0; i < str.length();)
		{
			bool found = false;

			for (int j = 0; j < charsLength; j++)
			{
				std::string a = "";
				

				if (str.length() >= i + chars[j].length() && str.substr(i, chars[j].length()) == chars[j])
				{
					if (word.length() != 0)
					{
						result.push_back(word);
						word = "";
					}
					result.push_back(chars[j]);
					found = true;
					i += chars[j].length();
					break;
				}
			}
			if (!found)
			{
				word += str[i];
				i++;
			}
		}
		if (word.length() != 0)
		{
			result.push_back(word);
		}

		return result;
	}
};