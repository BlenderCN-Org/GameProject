// project includes
#include "OcParser.hpp"
#include "OcParsable.hpp"

// std includes
#include <fstream>
#include <vector>
#include <string>

void performParserTests() {
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

	std::vector<Parsable> p = parseLines(lines);

	if ( checkParsableTypes(p) ) {
		for ( size_t i = 0; i < p.size(); i++ ) {
			std::string s = std::to_string(p[i].line);
			outFile.write(s.c_str(), s.size());
			outFile.write(", ", 2);
			outFile.write(p[i].code.c_str(), p[i].code.size());
			outFile.write("\n", 1);
		}
	}

	inFile.close();
	outFile.close();
}

void parseOcString(const char * codeString, const char * outFile) {}