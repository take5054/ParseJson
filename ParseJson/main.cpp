#include "JsonParse.hpp"
#include <iostream>

int main()
{
	JsonParse* jsonParse = new JsonParse();

	jsonParse->ParseFile("sample.json");
	auto jsonMap = jsonParse->GetJsonArray();
	for (const auto& map : jsonMap)
	{
		for (const auto& [key, value] : map)
		{
			std::cout << key << ": " << value << std::endl;
		}
	}

	delete jsonParse;
	jsonParse = nullptr;

	return 0;
}
