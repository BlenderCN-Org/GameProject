
// project includes
#include "OcParser.hpp"
#include "OcLexer.hpp"
#include "OcExpressionTree.hpp"

// std includes
#include <fstream>

#include <string>

void performParserTests() {

	testLexerTypes();
}

void parseOcFile(const char * fileName) {

	std::ifstream inFile(fileName, std::ios::in);
	
	std::string outFileName(fileName);
	outFileName = outFileName.substr(0, outFileName.find_first_of('.', 0));

	std::ofstream outFile(outFileName + ".ocp", std::ios::out);

	std::string line;

	std::vector<std::string> lines;

	while ( !inFile.eof() ) {
		std::getline(inFile, line);
		
		lines.push_back(line);
	}

	// lexing and type checking
	lines = performLexing(lines);

	// take parse lines and build expression tree

	lines = buildExpressions(lines);

	for ( size_t i = 0; i < lines.size(); i++ ) {
		outFile.write(lines[i].c_str(), lines[i].size());
	}

	inFile.close();
	outFile.close();

}

void parseOcString(const char * codeString, const char * outFile) {}

