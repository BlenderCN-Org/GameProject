#include "ReGlobal.hpp"

#include <GL/glew.h>

#include <thread>
#include <vector>

std::thread::id activeThread = std::thread::id();
bool lockdown = false;

std::thread::id getThreadId() {
	auto id = std::this_thread::get_id();
	return id;
}

struct RequestStruct {
	bool deleteRequest;
	uint32_t type;
	uint32_t count;
	uint32_t* idPtr;
};

std::vector<RequestStruct> requests;

void postGlCreateRequest(uint32_t type, uint32_t count, uint32_t* idPtr) {
	RequestStruct re;
	re.count = count;
	re.type = type;
	re.idPtr = idPtr;
	re.deleteRequest = false;
	requests.push_back(re);
}

void postGlDeleteRequest(uint32_t type, uint32_t count, uint32_t* idPtr) {
	RequestStruct re;
	re.count = count;
	re.type = type;
	re.idPtr = idPtr;
	re.deleteRequest = true;
	requests.push_back(re);
}

void processGlRequests() {

	size_t nRequests = requests.size();

	for (size_t i = 0; i < nRequests; i++) {
		RequestStruct& re = requests[i];

		switch (re.type) {
			case GL_REQUEST_BUFFER:
			{
				if (re.deleteRequest) {
					glDeleteBuffers(re.count, re.idPtr);
				}
				else {
					glCreateBuffers(re.count, re.idPtr);
				}
			}
			break;
			case GL_REQUEST_VERTEX_ARRAY:
			{
				if (re.deleteRequest) {
					glDeleteVertexArrays(re.count, re.idPtr);
				} else {
					glCreateVertexArrays(re.count, re.idPtr);
				}
			}
			break;
			default:
				break;
		}

	}

	requests.clear();

}

std::vector<std::function<void(void)>> functions;

void postGlFunction(std::function<void(void)> func) {
	functions.push_back(func);
}

void processGlFunctions() {

	size_t nFuncs = functions.size();
	for (size_t i = 0; i < nFuncs; i++) {
		functions[i]();
	}
	functions.clear();
}