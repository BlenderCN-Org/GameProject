#ifndef SHADEROBJECT_GL_HPP
#define SHADEROBJECT_GL_HPP

#include "../IShaderObject.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class ShaderObject_gl : public IShaderObject {
public:

	virtual void init();
	virtual void release();

	virtual void setShaderCode(ShaderStages stage, const char* code);
	virtual bool buildShader();

	virtual void useShader();

	virtual int getShaderUniform(char* uniformName);
	virtual void bindData(int location, UniformDataType type, void* data);

private:

	bool shaderCreated;
	GLuint shaderProgram;

	const char* shaderCode[(size_t)ShaderStages::SIZE];
};

#endif