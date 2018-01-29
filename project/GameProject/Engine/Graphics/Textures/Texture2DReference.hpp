#ifndef TEXTURE_2D_REFERENCE_HPP
#define TEXTURE_2D_REFERENCE_HPP

/// Internal Includes

/// External Includes
#include <RenderEngine/ITexture.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Texture {

			// Reference to an existing texture resource
			class Texture2DReference {

			public:

				Texture2DReference();
				virtual ~Texture2DReference();

				void getTextureSize(unsigned int& width, unsigned int& height);

				void setTexture(ITexture* tex);

				void bind();

			private:

				ITexture* texture;

			};

		} // namespace Texture
	} // namespace Graphics
} // namespace Engine

#endif