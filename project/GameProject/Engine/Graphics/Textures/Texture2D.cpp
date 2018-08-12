
/// Internal Includes
#include "Texture2D.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

namespace Engine {
	namespace Graphics {
		namespace Texture {

			Texture2D::Texture2D() {
				texture = gRenderEngine->createTexture();
				texture->init(4, false);
			}

			Texture2D::~Texture2D() {
				texture->release();
			}

			void Texture2D::singleColor(float r, float g, float b, float a) {

				unsigned char colors[4];

				colors[0] = (unsigned char)(r * 255.0F);
				colors[1] = (unsigned char)(g * 255.0F);
				colors[2] = (unsigned char)(b * 255.0F);
				colors[3] = (unsigned char)(a * 255.0F);

				texture->setTextureData(1, 1, 4, colors);
			}

			void Texture2D::getSize(unsigned int& x, unsigned int& y) {
				texture->getTextureSize(x, y);
			}

			void Texture2D::setData(unsigned int x, unsigned int y, unsigned int numComponents, void* data) {
				texture->setTextureData(x, y, numComponents, data);
			}

			void Texture2D::bind() {
				texture->bind();
			}

		} // namespace Texture
	} // namespace Graphics
} // namespace Engine
