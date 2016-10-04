#ifndef SHADEROBJECT_HPP
#define SHADEROBJECT_HPP

#include "../IShaderObject.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class ShaderObject : public IShaderObject
{
public:

	virtual void init();
	virtual void release();

	virtual void setShaderCode(ShaderStages stage, char* code);
	virtual bool buildShader();

	virtual void useShader();

	virtual int getShaderUniform(char* uniformName);
	virtual void bindData(int location, UniformDataType type, void* data);

private:
	
	bool shaderCreated;
	GLuint shaderProgram;

	char* shaderCode[(size_t)ShaderStages::SIZE];

};

#endif