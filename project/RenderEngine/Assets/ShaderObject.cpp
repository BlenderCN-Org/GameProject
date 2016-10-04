#include "ShaderObject.hpp"

#include <iostream>

GLenum getShaderFromEnum(ShaderStages type)
{
	switch ( type )
	{
		case ShaderStages::VERTEX_STAGE:
			return GL_VERTEX_SHADER;
			break;
		case ShaderStages::GEOMETRY_STAGE:
			return GL_GEOMETRY_SHADER;
			break;
		case ShaderStages::FRAGMENT_STAGE:
			return GL_FRAGMENT_SHADER;
			break;
		case ShaderStages::SIZE:
		default:
			break;
	}

	return -1;
}

void ShaderObject::init()
{
	shaderCreated = false;
	memset(shaderCode, 0, (size_t)ShaderStages::SIZE);
}

void ShaderObject::release()
{
	if( shaderCreated )
		glDeleteProgram(shaderProgram);

	delete this;
}

void ShaderObject::setShaderCode(ShaderStages stage, char * code)
{
	shaderCode[(size_t)stage] = code;
}

bool ShaderObject::buildShader()
{
	bool success = true;

	GLuint program;

	GLuint shaders[(size_t)ShaderStages::SIZE] = {0};

	program = glCreateProgram();

	for ( size_t i = 0; i < (size_t)ShaderStages::SIZE; i++ )
	{

		if ( shaderCode[i] )
		{			
			shaders[i] = glCreateShader(getShaderFromEnum(ShaderStages(i)));

			glShaderSource(shaders[i], 1, &shaderCode[i], nullptr);
			glCompileShader(shaders[i]);

			GLint isCompiled = 0;
			glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &isCompiled);
			if ( isCompiled == GL_FALSE )
			{
				success = false;
				//printf("Failed to compile shader %s\n", shadernames[i].c_str());

				char* msg = "Failed to compile shader";
				glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, sizeof(msg), msg);

				GLchar log[10240];
				GLsizei length;
				glGetShaderInfoLog(shaders[i], 10239, &length, log);
				printf("Compiler log:\n%s\n", log);
				glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, length, log);
			}
			glAttachShader(program, shaders[i]);

		}
	}

	glLinkProgram(program);

	for ( size_t i = 0; i < (size_t)ShaderStages::SIZE; i++ )
	{
		if ( shaderCode[i] )
		{
			glDetachShader(program, shaders[i]);
			glDeleteShader(shaders[i]);
		}
	}

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if ( linked != GL_TRUE )
	{
		success = false;
		printf("Failed to link shader program %d\n", program);
		char* msg = "Failed to link shader program";
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, sizeof(msg), msg);

		GLchar log[10240];
		GLsizei length;
		glGetProgramInfoLog(program, 10239, &length, log);
		printf("Compiler log:\n%s\n", log);
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, length, log);
	}

	if ( success )
	{
		if ( shaderCreated )
			glDeleteProgram(shaderProgram);
		shaderProgram = program;
		shaderCreated = true;
	}
	else
	{
		glDeleteProgram(program);
	}

	return success;
}

void ShaderObject::useShader()
{
	if ( shaderCreated )
		glUseProgram(shaderProgram);
}

int ShaderObject::getShaderUniform(char * uniformName)
{
	return glGetUniformLocation(shaderProgram, uniformName);
}

void ShaderObject::bindData(int location, UniformDataType type, void* data)
{
	float* d = (float*)data;

	switch ( type )
	{
		case UniformDataType::UNI_INT:
			glProgramUniform1iv(shaderProgram, location, 1, (int*)d);
			break;
		case UniformDataType::UNI_FLOAT:
			glProgramUniform1fv(shaderProgram, location, 1, d);
			break;
		case UniformDataType::UNI_FLOAT2:
			glProgramUniform2fv(shaderProgram, location, 1, d);
			break;
		case UniformDataType::UNI_FLOAT3:
			glProgramUniform3fv(shaderProgram, location, 1, d);
			break;
		case UniformDataType::UNI_FLOAT4:
			glProgramUniform4fv(shaderProgram, location, 1, d);
			break;
		case UniformDataType::UNI_MATRIX4X4:
			glProgramUniformMatrix4fv(shaderProgram, location, 1, GL_FALSE, d);
			break;
		default:
			throw("NEIN");
			break;
	}
}
