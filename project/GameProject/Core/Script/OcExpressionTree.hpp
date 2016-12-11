#ifndef OCEXPRESSIONTREE_HPP
#define OCEXPRESSIONTREE_HPP

#include <string>
#include <vector>

enum class DataType
{
	eDataType_int = 0,
	eDataType_char = 1,
	eDataType_short = 2,
	eDataType_bool = 3,
	eDataType_float= 4,
};

enum class Operation
{
	eOperation_ADD = 0,
	eOperation_SUB = 1,
	eOperation_MUL = 2,
	eOperation_DIV = 3,
	eOperation_ASSIGN = 4,
};

struct Variable
{
	std::string variableName;
	DataType variableDataType;

	bool isUnsigned;
	int variableValue;
};

struct Function
{
	std::string functionName;
	DataType returnType;

	bool unsignedReturn;
	int parameterCount;

	Variable* parameters;

};

struct Expression
{
	// expression line number
	int lineNr;
	// expression size
	int size;

	Variable var;

};

std::vector<std::string> buildExpressions(std::vector<std::string> parseLines);



#endif