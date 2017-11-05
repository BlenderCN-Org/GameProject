#ifndef SHADER_CONTAINER_HPP
#define SHADER_CONTAINER_HPP

/// Internal Includes

/// External Includes
#include <glm/glm.hpp>
#include <RenderEngine/IMesh.hpp>
#include <RenderEngine/IShaderObject.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		struct GuiShaderContainer {
			IShaderObject* guiElementShader;
			IShaderObject* guiTextShader;

			IMesh* standardQuad;

			glm::mat4 orthoMatrix;

			int elementVpMat;
			int elementTexture;
			int elementTransformMat;

			int textVpMat;
			int textTransform;
			int textTexture;
			int textColor;

		};
	} // namespace Graphics
} // namespace Engine

#endif

