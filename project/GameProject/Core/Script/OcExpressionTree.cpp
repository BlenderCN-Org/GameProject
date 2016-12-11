#include "OcExpressionTree.hpp"

std::vector<std::string> buildExpressions(std::vector<std::string> parseLines) {
	
	std::vector<std::string> expressions;

	for ( size_t i = 0; i < parseLines.size(); i++ ) {

		expressions.push_back(parseLines[i]);
	}

	return expressions;
}
