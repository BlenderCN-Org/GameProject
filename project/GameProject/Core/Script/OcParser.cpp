
// project includes
#include "OcParser.hpp"

// std includes
#include <fstream>

#include <string>

void parseOcFile(const char * fileName) {

	std::ifstream inFile(fileName, std::ios::in);
	
	std::string outFileName(fileName);
	outFileName = outFileName.substr(0, outFileName.find_first_of('.', 0));

	std::ofstream outFile(outFileName + ".ocp", std::ios::out);

	std::string line;
	std::string parseWrite;

	//while ( !inFile.eof() ) {
	//	parseWrite = "";
	//	std::getline(inFile, line);
	//	
	//	// if line contains stuff
	//	if ( line.size() > 0 ) {
	//
	//		if ( !isLineComment(line.c_str()) ) {
	//			parseWrite = line + "\n";
	//		}
	//	}
	//
	//	outFile.write(parseWrite.c_str(), parseWrite.size());
	//
	//}

	inFile.close();
	outFile.close();

}

void parseOcString(const char * codeString, const char * outFile) {}

bool isLineComment(const char * line) {

	bool isComment = false;

	const char * c = line;
	while ( *c != '\0' && !isComment) {

		if ( *c == '/' && *(c + 1) == '/' )
			isComment = true;
		
		if ( *c == ';' )
			break;
		c++;
	}

	return isComment;

}
