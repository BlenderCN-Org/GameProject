#ifndef OCPARSER_HPP
#define OCPARSER_HPP

void parseOcFile(const char* file);

void parseOcString(const char* codeString, const char* outFile);

bool isLineComment(const char* line);

#endif