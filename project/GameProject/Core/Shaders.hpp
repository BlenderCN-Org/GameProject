#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <RenderEngine\IRenderEngine.hpp>

struct DefaultShader
{
	IShaderObject* shaderObject;
	int vp_location;
	int mdl_location;
	int tex_location;
};

struct TextShader
{
	IShaderObject* shaderObject;
	int vp_location;
	int mdl_location;
	int tex_location;
};

class Shaders
{

public:

	static Shaders* getShaders();
	static void release();

	DefaultShader defShader;
	TextShader textShader;

private:
	static Shaders* singleInstance;

};

#endif