// project includes
#include "Sys.hpp"

// project dependices

// std includes

#include <thread>

// os-specific includes


int getLogicalProcessorCount() {

	int c = std::thread::hardware_concurrency();

	return c;
}
