//#include <GL\glew.h>

#include <WindowSystem\WindowSystem.hpp>

#include <iostream>

#include <Windows.h>
#include <fstream>

#include <GL\GL.h>

using namespace std;

#ifdef _DEBUG
#endif
#ifdef _stuff
extern "C"
{
	void __stdcall openglCallbackFunction(GLenum source,
										  GLenum type,
										  GLuint id,
										  GLenum severity,
										  GLsizei length,
										  const GLchar* message,
										  void* userParam)
	{
		//if ( severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH )
		{
			printf("---------------------opengl-callback-start------------\n");
			printf("message: %s\n", message);
			printf("type: ");
			switch ( type ) {
				case GL_DEBUG_TYPE_ERROR:
					printf("ERROR");
					break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
					printf("DEPRECATED_BEHAVIOR");
					break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
					printf("UNDEFINED_BEHAVIOR");
					break;
				case GL_DEBUG_TYPE_PORTABILITY:
					printf("PORTABILITY");
					break;
				case GL_DEBUG_TYPE_PERFORMANCE:
					printf("PERFORMANCE");
					break;
				case GL_DEBUG_TYPE_OTHER:
					printf("OTHER");
					break;
			}
			printf("\n");

			printf("id: %d\n", id);
			printf("severity: ");
			switch ( severity ) {
				case GL_DEBUG_SEVERITY_LOW:
					printf("LOW");
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					printf("MEDIUM");
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					printf("HIGH");
					break;
			}
			printf("\n");
			printf("---------------------opengl-callback-end--------------\n");
		}
	}
}
#endif

/*void init()
{
	glewInit();

#ifdef _DEBUG
	if ( glDebugMessageCallback ) {
		printf("Register OpenGL debug callback\n");
		glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	} else
		printf("glDebugMessageCallback not available\n");
#endif
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);

}
*/
void term_func() {
	cout << "term_func was called by terminate." << endl;
}

LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	std::ofstream f;
	f.open("VectoredExceptionHandler.txt", std::ios::out | std::ios::trunc);
	f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	f.close();

	return EXCEPTION_CONTINUE_SEARCH;
}

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	std::ofstream f;
	f.open("TopLevelExceptionHandler.txt", std::ios::out | std::ios::trunc);
	f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	f.close();

	return EXCEPTION_CONTINUE_SEARCH;
}

int main()
{

	set_terminate(term_func);

	AddVectoredExceptionHandler(1, VectoredExceptionHandler);
	SetUnhandledExceptionFilter(TopLevelExceptionHandler);

	float f = 0;
	//glfwInit();
	wsInit();

	WSWindow* window = wsCreateWindow(500, 500, "title", 0, 0);

	wsCreateOGLContext(window);
	wsMakeCurrent(window);

	while ( wsIsWindowVisible(window) )
	{
		wsPollEvents(window);

		glClearColor(f, f, f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		wsSwapBuffers(window);

		f += 0.01f;
		if ( f > 1.0f )
			f -= 1.0f;

	}
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//
	//glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
	//
	//
	//GLFWwindow* wnd = glfwCreateWindow(1280, 720, "Title", nullptr, nullptr);
	//
	//glfwMakeContextCurrent(wnd);
	//glfwShowWindow(wnd);
	//glfwSwapInterval(1);

	//init();

	//while ( !glfwWindowShouldClose(wnd) )
	

	//glfwDestroyWindow(wnd);

	//glfwTerminate();

	wsDestroyWindow(window);

	wsTerminate();

	return 0;
}