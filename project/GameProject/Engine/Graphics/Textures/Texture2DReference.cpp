
/// Internal Includes
#include "Texture2DReference.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

namespace Engine {
	namespace Graphics {
		namespace Texture {

			Texture2DReference::Texture2DReference() {}

			Texture2DReference::~Texture2DReference() {}

			void Texture2DReference::getTextureSize(unsigned int& width, unsigned int& height) {
				width = 0;
				height = 0;

				if (texture) {
					texture->getTextureSize(width, height);
				}

			}

			void Texture2DReference::setTexture(ITexture* tex) {
				texture = tex;
			}

			void Texture2DReference::bind() {
				if (texture) {
					texture->bind();
				}

			}

		} // namespace Texture
	} // namespace Graphics
} // namespace Engine
