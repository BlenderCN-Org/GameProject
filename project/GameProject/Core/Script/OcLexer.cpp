#include "OcLexer.hpp"

void testLexerTypes() {

	printf("-------------\n");
	printf("Testing Lexer\n");
	printf("-------------\n");

	printf("-------------\n");
	printf("Types\n");
	printf("-------------\n");

	for ( size_t i = 0; i < nrLexerKeywords; i++ ) {
		bool ok = hasValidTypes(std::string(keywords[i]) + " variableName = 0;");
		if ( ok ) {
			printf("SUCCESS: %s is a valid keyword\n", keywords[i]);
		} else {
			printf("FAILED: %s is not valid keyword when it should be\n", keywords[i]);
		}
	}

	bool ok = hasValidTypes("foobar variableName = 0;");
	if ( !ok ) {
		printf("SUCCESS: foobar is not a valid keyword\n");
	} else {
		printf("FAILED: foobar is a valid keyword when it should not be\n");
	}

	ok = hasValidTypes("        spaces variableName = 0;");
	if ( !ok ) {
		printf("SUCCESS: spaces is not a valid keyword\n");
	} else {
		printf("FAILED: spaces is a valid keyword when it should not be\n");
	}

	ok = hasValidTypes("        int variableName = 0;");
	if ( ok ) {
		printf("SUCCESS: int is a valid keyword\n");
	} else {
		printf("FAILED: int is not a valid keyword when it should be\n");
	}


	printf("-------------\n");
	printf("Expression\n");
	printf("-------------\n");

	ok = lineIsExpression("           asdf = 0;");
	if ( ok ) {
		printf("SUCCESS: asdf = 0 is a valid expression\n");
	} else {
		printf("FAILED: asdf = 0 should be a valid expression\n");
	}

	ok = lineIsExpression("qwert = 0;");
	if ( ok ) {
		printf("SUCCESS: qwert = 0 is a valid expression\n");
	} else {
		printf("FAILED: qwert = 0 should be a valid expression\n");
	}

	ok = lineIsExpression("int i = 0;");
	if ( !ok ) {
		printf("SUCCESS: int i = 0 is a not valid expression\n");
	} else {
		printf("FAILED: int i = 0 should not be a valid expression\n");
	}

	ok = lineIsExpression("int a=0;");
	if ( !ok ) {
		printf("SUCCESS: int a=0 is a not valid expression\n");
	} else {
		printf("FAILED: int a=0 should not be a valid expression\n");
	}

	ok = lineIsExpression("sma=0;");
	if ( ok ) {
		printf("SUCCESS: sma=0 is a valid expression\n");
	} else {
		printf("FAILED: sma=0 should be a valid expression\n");
	}

	ok = lineIsExpression("boo(param);");
	if ( ok ) {
		printf("SUCCESS: boo(param) is a valid expression\n");
	} else {
		printf("FAILED: boo(param) should be a valid expression\n");
	}

	ok = lineIsExpression("fun (param);");
	if ( ok ) {
		printf("SUCCESS: fun (param) is a valid expression\n");
	} else {
		printf("FAILED: fun (param) should be a valid expression\n");
	}

	ok = lineIsExpression("int fun(param);");
	if ( !ok ) {
		printf("SUCCESS: int fun(param) is a not valid expression\n");
	} else {
		printf("FAILED: int fun(param) should not be a valid expression\n");
	}

	ok = lineIsExpression("int fun (param);");
	if ( !ok ) {
		printf("SUCCESS: int fun (param) is a not valid expression\n");
	} else {
		printf("FAILED: int fun (param) should not be a valid expression\n");
	}

	ok = lineIsExpression("     foo(param);");
	if ( ok ) {
		printf("SUCCESS: foo(param) is a valid expression\n");
	} else {
		printf("FAILED: foo(param) should be a valid expression\n");
	}

	printf("-------------\n");
	printf("Scope\n");
	printf("-------------\n");

	ok = isScope("{");
	if ( ok ) {
		printf("SUCCESS: { is scope character\n");
	} else {
		printf("FAILED: { should be a scope character\n");
	}

	ok = isScope("}");
	if ( ok ) {
		printf("SUCCESS: } is scope character\n");
	} else {
		printf("FAILED: } should be a scope character\n");
	}

	ok = isScope(";");
	if ( !ok ) {
		printf("SUCCESS: ; is not a scope character\n");
	} else {
		printf("FAILED: ; should not be a scope character\n");
	}

	ok = isScope("'");
	if ( !ok ) {
		printf("SUCCESS: ' is not a scope character\n");
	} else {
		printf("FAILED: ' should not be a scope character\n");
	}

	// ?
	ok = isScope("asdasda{");
	if ( !ok ) {
		printf("SUCCESS: asdasda{ is not a scope character\n");
	} else {
		printf("FAILED: asdasda{ should not be a scope character\n");
	}

}

bool hasValidTypes(std::string code) {

	std::string type = "";

	char c = 0;

	for ( size_t i = 0; i < code.size(); i++ ) {
		c = code[i];
		if ( c == ' ' && type.size() != 0 ) {
			break;
		} else {
			if ( c != ' ' ) {
				type += c;
			}
		}
	}

	bool found = false;

	for ( size_t i = 0; i < nrLexerKeywords && !found; i++ ) {
		if ( strcmp(type.c_str(), keywords[i]) == 0 ) {
			found = true;
		}
	}
	return found;
}

bool isLineComment(const char * line) {

	bool isComment = false;

	const char * c = line;
	while ( *c != '\0' && !isComment ) {

		if ( *c == '/' && *(c + 1) == '/' )
			isComment = true;

		if ( *c == ';' )
			break;
		c++;
	}

	return isComment;

}

std::string removeAddedCommentsAndTabs(std::string code) {

	std::string line = "";
	const char * c = code.c_str();

	bool isComment = false;

	while ( *c != '\0' && !isComment ) {


		if ( *c == '/' && *(c + 1) == '/' ) {
			isComment = true;
		} else {
			if ( *c == '\t' ) {
				line += ' ';
			} else {
				line += *c;
			}
		}
		c++;
	}

	return line;
}

bool lineIsExpression(std::string line) {

	const char * c = line.c_str();
	bool isExpression = false;

	int charactersPassed = 0;
	int wordsStarted = 0;
	char last = *c;

	// find if line conains "someString = someExpr with spaces;"

	while ( *c != '\0' && !isExpression ) {

		// found equal sign (word should be 1)
		if ( *c == '=' && wordsStarted == 1 ) {
			isExpression = true;
		}

		// found a space new word
		if ( *c == ' ' && last != ' ' ) {
			charactersPassed = 0;
		} else {
			if ( charactersPassed == 0 ) {
				wordsStarted++;
			}
			charactersPassed++;
		}

		// end of line
		if ( *c == ';' ) {
			break;
		}

		last = *c;
		c++;
	}

	// find if we call a function "function(parameter);"
	if ( !isExpression ) {
		const char * c = line.c_str();
		wordsStarted = 0;
		last = *c;
		charactersPassed = 0;
		while ( *c != '\0' && !isExpression ) {

			if ( *c == '(' && wordsStarted == 1 ) {
				isExpression = true;
			}

			// found a space new word
			if ( *c == ' ' && last != ' ' ) {
				charactersPassed = 0;
			} else {
				if ( charactersPassed == 0 ) {
					wordsStarted++;
				}
				charactersPassed++;
			}

			// end of line
			if ( *c == ';' ) {
				break;
			}

			last = *c;
			c++;
		}
	}

	return isExpression;
}

bool lineIsKeyword(std::string line) {
	return hasValidTypes(line);
}

bool isScope(std::string line) {

	const char * c = line.c_str();
	bool scope = false;

	while ( *c != '\0' && !scope ) {
		if ( *c == '{' || *c == '}' ) {
			scope = true;
		}
		c++;
	}

	return scope;
}

std::vector<std::string> performLexing(std::vector<std::string> code) {

	std::vector<std::string> codeBlocks;

	for ( size_t i = 0; i < code.size(); i++ ) {
		std::string line = code[i];
		if ( !isLineComment(line.c_str()) && line.size() > 0 ) {
			line = removeAddedCommentsAndTabs(line);
			if ( lineIsExpression(line) || lineIsKeyword(line) || isScope(line) ) {
				codeBlocks.push_back("{" + std::to_string(i + 1) + "," + line + "}\n");
			}
			else 				{
				printf("Failed to parse %s\n", line.c_str());
			}
		}
	}

	return codeBlocks;
}
