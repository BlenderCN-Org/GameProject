#ifndef TEXTURE_HPP
#define TEXTURE_HPP

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

				void bind();

			private:

				ITexture* texture;

			};

		} // namespace Texture
	} // namespace Graphics
} // namespace Engine

#endif