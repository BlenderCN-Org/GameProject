#ifndef OCPARSABLE_HPP
#define OCPARSABLE_HPP

#include <vector>
#include <string>

struct Parsable
{
	int line;
	std::string code;
};

struct NameType
{
	bool isFunction;
	std::string name;
};

std::vector<Parsable> parseLines(std::vector<std::string> lines);

bool checkParsableTypes(std::vector<Parsable> par);

#endif