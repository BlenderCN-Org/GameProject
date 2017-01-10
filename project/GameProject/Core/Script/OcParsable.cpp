#include "OcParsable.hpp"

std::string cleanupLine(std::string line) {
	if ( line.size() == 0 ) {
		return "";
	} else {
		std::string newLine;

		const char* str = line.c_str();

		while ( *str != '\0' ) {
			if ( *str == '/' && *(str + 1) == '/' ) {
				break;
			}

			if ( *str != '\t' ) {
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
	std::string temp;

	const char* str;

	for ( size_t i = 0; i < length; i++ ) {
		line = cleanupLine(lines[i]);

		if ( line.size() != 0 ) {
			temp = "";
			str = line.c_str();
			while ( *str != '\0' ) {
				if ( *str == ' ' ) {
					if ( temp.size() > 0 ) {
						parsed.push_back(Parsable{ (int)i + 1, temp });
						temp = "";
					}
				} else if ( *str == '(' || *str == ')' || *str == ';' ) {
					if ( temp.size() > 0 ) {
						parsed.push_back(Parsable{ (int)i + 1, temp });
						temp = *str;
						parsed.push_back(Parsable{ (int)i + 1, temp });
						temp = "";
					}
				} else {
					temp += *str;
				}
				str++;
			}
			if ( temp.size() > 0 ) {
				parsed.push_back(Parsable{ (int)i + 1, temp });
			}
		}
	}

	return parsed;
}

bool isType(std::string str) {
	const char* s = str.c_str();

	if ( strcmp(s, "int") == 0 ) {
		return true;
	} else if ( strcmp(s, "float") == 0 ) {
		return true;
	} else if ( strcmp(s, "bool") == 0 ) {
		return true;
	} else if ( strcmp(s, "char") == 0 ) {
		return true;
	} else if ( strcmp(s, "short") == 0 ) {
		return true;
	} else if ( strcmp(s, "return") == 0 ) {
		return true;
	} else if ( strcmp(s, "for") == 0 ) {
		return true;
	} else if ( strcmp(s, "do") == 0 ) {
		return true;
	} else if ( strcmp(s, "while") == 0 ) {
		return true;
	} else if ( strcmp(s, "if") == 0 ) {
		return true;
	} else if ( strcmp(s, "else") == 0 ) {
		return true;
	}

	return false;
}

bool checkParsableTypes(std::vector<Parsable> par) {
	bool passed = true;

	size_t length = par.size();

	std::string str;

	std::vector<NameType> definedNames;

	for ( size_t i = 0; i < length && passed; i++ ) {
		str = par[i].code;

		if ( isType(str) ) {
			// we have found
			// type name;
			// type name = value;
			// type function(...)
			// for(...) if(..) etc

			printf("Type: %s\n", str.c_str());
		} else {
			printf("Other: %s\n", str.c_str());
		}
	}

	return passed;
}