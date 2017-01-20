#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

enum class ExceptionSeverity
{
	eSeverityLow = 0,
	eSeverityNormal = 1,
	eSeverityHigh = 2,
	eSeverityCritical = 3,
};

class BaseException {

public:
	ExceptionSeverity severity;


};



#endif