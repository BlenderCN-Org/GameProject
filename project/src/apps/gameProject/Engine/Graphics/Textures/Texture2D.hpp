#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

/// Internal Includes

/// External Includes
#include <RenderEngine/ITexture.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Texture {

			class Texture2D {

			public:

				Texture2D();
				virtual ~Texture2D();

				void singleColor(float r, float g, float b, float a);

				void getSize(unsigned int& x, unsigned int& y);
				void setData(unsigned int x, unsigned int y, unsigned int numComponents, void* data);

				void bind();

			private:

				ITexture* texture;

			};

		} // namespace Texture
	} // namespace Graphics
} // namespace Engine

#endif