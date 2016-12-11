#ifndef OCLEXER_HPP
#define OCLEXER_HPP

#include <string>
#include <vector>

const char* const keywords[] = { "void", "int", "char", "short", "unsigned", "bool" , "if", "while", "do", "for", "return", "else", "float"};
const int nrLexerKeywords = 13;

void testLexerTypes();

bool hasValidTypes(std::string code);

bool isLineComment(const char* line);

std::string removeAddedCommentsAndTabs(std::string code);

bool lineIsExpression(std::string line);
bool lineIsKeyword(std::string line);
bool isScope(std::string line);

std::vector<std::string> performLexing(std::vector<std::string> lines);

#endif