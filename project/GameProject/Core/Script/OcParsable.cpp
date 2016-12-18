#include "OcParsable.hpp"

std::string cleanupLine(std::string line) {
	if ( line.size() == 0 ) {
		return "";
	}
	else {

		std::string newLine;

		const char* str = line.c_str();

		while ( *str != '\0' ) {

			if ( *str == '/' && *(str + 1) == '/' ) {
				break;
			}

			if ( *str != '\t') {
				newLine += *str;
			} else {
				newLine += ' ';
			}

			str++;
		}

		return newLine;

	}
}

std::vector<Parsable> parseLines(std::vector<std::string> lines) {
	std::vector<Parsable> parsed;

	size_t length = lines.size();

	std::string line;

	for ( size_t i = 0; i < length; i++ ) {

		line = cleanupLine(lines[i]);

		if ( line.size() != 0 ) {
			parsed.push_back(Parsable{ (int)i + 1, line });
		}
	}

	return parsed;
}

bool checkParsableTypes(std::vector<Parsable> par) {

	bool passed = false;

	size_t length = par.size();

	std::string str;

	for ( size_t i = 0; i < length; i++ ) {

		str = par[i].code;
	
		if ( str.rfind(';') != std::string::npos) {
			printf("something\n");
			printf("%s\n", str.c_str());
		}
		else {
			printf("else\n");
			printf("%s\n", str.c_str());
		}

	}

	return passed;
}
