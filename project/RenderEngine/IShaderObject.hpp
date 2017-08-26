#ifndef ISHADEROBJECT_HPP
#define ISHADEROBJECT_HPP

enum class ShaderStages {
	VERTEX_STAGE,
	GEOMETRY_STAGE,
	FRAGMENT_STAGE,

	SIZE,
};

enum class UniformDataType {
	UNI_INT,
	UNI_FLOAT,
	UNI_FLOAT2,
	UNI_FLOAT3,
	UNI_FLOAT4,
	UNI_MATRIX4X4,
};

class IShaderObject {
public:

	virtual void init() = 0;
	virtual void release() = 0;

	virtual void setShaderCode(ShaderStages stage, const char* code) = 0;

	/*
		@return The completion of the shader compiling. True means success.
		If an old shader was already created it will be replaced only if the new one succeeded.
		Else the state is unchanged.
	*/
	virtual bool buildShader() = 0;

	virtual void useShader() = 0;

	virtual int getShaderUniform(char* uniformName) = 0;
	virtual void bindData(int location, UniformDataType type, void* data) = 0;
};

#endif