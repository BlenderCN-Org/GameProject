#include "ExceptionHandling.hpp"

#include <fstream>
#include <Windows.h>

void term_func() {
	int breakHere = 3;
}

LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
	std::ofstream f;
	f.open("VectoredExceptionHandler.txt", std::ios::out | std::ios::trunc);
	f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	f << pExceptionInfo->ExceptionRecord->ExceptionInformation << std::endl;
	f.close();

	return EXCEPTION_CONTINUE_SEARCH;
}

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
	std::ofstream f;
	f.open("TopLevelExceptionHandler.txt", std::ios::out | std::ios::trunc);
	f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	f.close();

	return EXCEPTION_CONTINUE_SEARCH;
}

void initExceptionHandlers() {}