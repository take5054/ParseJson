#include "JsonParse.hpp"

int main()
{
	JsonParse* jsonParse = new JsonParse();

	delete jsonParse;
	jsonParse = nullptr;

	return 0;
}
