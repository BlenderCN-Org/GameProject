#ifndef SHADER_CONTAINER_HPP
#define SHADER_CONTAINER_HPP

/// Internal Includes

/// External Includes
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )
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

		};
	} // namespace Graphics
} // namespace Engine

#endif

