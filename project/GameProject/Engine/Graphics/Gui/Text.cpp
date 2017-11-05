/// Internal Includes
#include "Text.hpp"
#include "../Graphics.hpp"

///

Text::Text() : font(nullptr), textMesh(nullptr), width(0), height(0) {
	textMesh = gRenderEngine->createMesh();
	font = gRenderEngine->createFont();

	if (font) {
		font->init("C:/Windows/Fonts/Arial.ttf", 12);
	}

	if (textMesh) {
		textMesh->init(MeshPrimitiveType::TRIANGLE);
	}

}
Text::~Text() {
	if (font) {
		font->release();
	}

	if (textMesh) {
		textMesh->release();
	}
}

void Text::setText(Engine::Core::String text) {
	if (textMesh) {
		const char* cStr = text.cStr();
		size_t len = text.getSize();

		char c = '\0';
		Character chr;

		const size_t floatsPerVertex = 5; // 5 elements per vertex
		const size_t nrVertices = 6; // 3 verts per triangle = 6 verts

		const size_t nrVerexFloats = nrVertices * floatsPerVertex; // number of floats per character
		const size_t bytesPerCharacter = sizeof(float) * nrVerexFloats;

		size_t arraySize = nrVerexFloats * len;

		float* verts = new float[arraySize];

		float x = 10;
		float y = 10;
		float scale = 1.0F;

		float dx = 0;

		float yAdv = 0;
		float oldYAdv = 0;

		int fSize = font->getFontSize();

		for (size_t i = 0; i < len; i++) {

			c = cStr[i];
			chr = font->getCharacter(c);

			if (i == 0)
				y += fSize;

			if (c == '\n') {
				if (yAdv) {
					y += (fSize);
					oldYAdv = (float)fSize;
				} else {
					y += oldYAdv;
				}
				yAdv = 0;
				x = dx;
				memset(&verts[i * nrVerexFloats], 0, bytesPerCharacter);
				continue;
			} else if (c == '\t') {
				x += (chr.size.x * scale * 2.0f);
				memset(&verts[i * nrVerexFloats], 0, bytesPerCharacter);
				continue;
			} else if (c == '\0') {
				memset(&verts[i * nrVerexFloats], 0, bytesPerCharacter);
				continue;
			}

			float xpos = x + chr.bearing.x * scale;
			float ypos = y + ((chr.size.y - chr.bearing.y) * scale);

			float w = chr.size.x * scale;
			float h = chr.size.y * scale;

			yAdv = glm::max(yAdv, h);

			int texWidth;
			int texHeight;

			font->getTextureInfo(texWidth, texHeight);

			// Update VBO for each character
			float texX1 = (float)chr.texturePos.x / (float)texWidth;
			float texY1 = (float)chr.texturePos.y / (float)texHeight;

			float texX2 = (float)(chr.texturePos.x + chr.size.x) / (float)texWidth;
			float texY2 = (float)(chr.texturePos.y + chr.size.y) / (float)texHeight;

			float vertices[6][5] = {
				{ xpos,     ypos - h, 0,   texX1, texY1 },
				{ xpos,     ypos, 0,       texX1, texY2 },
				{ xpos + w, ypos, 0,       texX2, texY2 },

				{ xpos,     ypos - h, 0,   texX1, texY1 },
				{ xpos + w, ypos, 0,       texX2, texY2 },
				{ xpos + w, ypos - h, 0,   texX2, texY1 }
			};

			memcpy(&verts[i * nrVerexFloats], vertices, sizeof(vertices));

			x += (chr.advance >> 6) * scale;

			assert(sizeof(vertices) == bytesPerCharacter);
			assert((i*nrVerexFloats) < (nrVerexFloats*len));

			width = glm::max(width, int(x));
			height = glm::max(height, int(y));

		}

		textMesh->setMeshData(verts, arraySize * sizeof(float), MeshDataLayout::VERT_UV);

		delete[] verts;
	}
}

void Text::render(int textureSlot) {

	font->bindFontTexture();
	textMesh->bind();

	textMesh->render();

}