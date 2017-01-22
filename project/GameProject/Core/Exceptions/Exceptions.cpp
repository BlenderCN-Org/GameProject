#include "Exceptions.hpp"

BaseException::BaseException(ExceptionSeverity _severity) : severity(_severity) {}

BaseException::BaseException(ExceptionSeverity _severity, char * _message) : severity(_severity), message(_message) {}
