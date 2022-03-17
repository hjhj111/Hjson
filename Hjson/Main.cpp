#include "Hjson.h"

using namespace Hjson;

int main()
{
	Json test0 = Object{
	{ "key1", "value1" },
		{ "key2", "บร"},
		{ "key3", Array { 1, 2, 3 } }, };

	std::cout << test0.GetRawStr()<<std::endl;
	std::cout << test0.at("key3").second.at(1).second.GetRawStr() << "  " << test0.at("key2").second.GetRawStr() << std::endl;

	auto test1 = Json::Parse("{\"key1\": \"value1\", \"key2\": false, \"key3\": [1, 2, 3]}");
	if (test1.first.empty())
	{
		std::cout << test1.second.GetRawStr() << std::endl;
		std::cout << test1.second.at("key3").second.at(1).second.GetRawStr() << "  " << test1.second.at("key2").second.GetRawStr();

	}
	else
	{
		std::cout << test1.first << std::endl;
	}

	auto str = ReadFile("test.json");
	auto test2 = Json::Parse(str);
	if (test2.first.empty())
	{
		std::cout << test2.second.GetRawStr() << std::endl;
		std::cout << test2.second.at("key3").second.at(1).second.GetRawStr() << "  " << test2.second.at("key2").second.GetRawStr()<<std::endl;
	}
	else
	{
		std::cout << test1.first << std::endl;
	}
}