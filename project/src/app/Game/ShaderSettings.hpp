#ifndef SHADER_SETTINGS_HPP
#define SHADER_SETTINGS_HPP

/// Internal Includes

/// External Includes
#include <glm/glm.hpp>
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

struct ShaderSettings {

	IShaderObject* activeShader;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 viewMatrix;

	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;

	int viewProjectionLocation;
	int clipPlaneLocation;
	int modelMatrixLocation;
	int reflectionMatrixLocation;
	


};

#endif