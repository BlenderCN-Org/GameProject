/// Internal Includes
#include "Graphics.hpp"
#include "Gui.hpp"
#include "GuiShaders.hpp"
#include "../Input/Input.hpp"

/// External Includes
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
	namespace Graphics {

		CGui::CGui()
			: guiItems()
			, shaders() {

			// create resources from renderEngine
			shaders.guiElementShader = gRenderEngine->createShaderObject();
			shaders.guiTextShader = gRenderEngine->createShaderObject();
			shaders.standardQuad = gRenderEngine->createMesh();

			// init text shader
			shaders.guiTextShader->init();
			shaders.guiTextShader->setShaderCode(ShaderStages::VERTEX_STAGE, defaultTextVertexShaderCode);
			shaders.guiTextShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
			shaders.guiTextShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, defaultTextFragmentShaderCode);

			if (shaders.guiTextShader->buildShader() == false) {
				//gConsole->print("Failed to build default text shader\n");
				assert(0 && "BAD");
			}

			// init element shader
			shaders.guiElementShader->init();
			shaders.guiElementShader->setShaderCode(ShaderStages::VERTEX_STAGE, defaultPassthroughVertexShaderCode);
			shaders.guiElementShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, defaultPointToQuadGeomentryShaderCode);
			shaders.guiElementShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, defaultQuadFragmentShaderCode);

			if (shaders.guiElementShader->buildShader() == false) {
				//gConsole->print("Failed to build default quad to point shader\n");
				assert(0 && "BAD");
			}

			// init mesh
			shaders.standardQuad->init(MeshPrimitiveType::POINT);

			// fetch shader uniforms
			shaders.elementVpMat = shaders.guiElementShader->getShaderUniform("viewProjMatrix");
			shaders.elementTransformMat = shaders.guiElementShader->getShaderUniform("worldMat");
			shaders.elementTexture = shaders.guiElementShader->getShaderUniform("tex");
			
			shaders.textVpMat = shaders.guiTextShader->getShaderUniform("viewProjMatrix");
			shaders.textTransform = shaders.guiTextShader->getShaderUniform("worldMat");
			shaders.textTexture = shaders.guiTextShader->getShaderUniform("text");
			shaders.textColor = shaders.guiTextShader->getShaderUniform("textColor");

			// set dummy data for rendering a single point
			float data[5];
			memset(data, 0, sizeof(data));
			shaders.standardQuad->setMeshData(data, sizeof(data), VERT_UV);
		}

		CGui::~CGui() {

			shaders.guiElementShader->release();
			shaders.guiTextShader->release();

			shaders.standardQuad->release();
		}

		void CGui::addGuiItem(GuiItem* guiItem) {

			guiItems.push_back(guiItem);

		}

		void CGui::render() {

			std::vector<GuiItem*>::const_iterator it = guiItems.begin();
			std::vector<GuiItem*>::const_iterator eit = guiItems.end();

			glm::mat4 screenSize;

			screenSize[0].x = 0;
			screenSize[0].y = 0;

			int w = 0, h = 0;

			Input::Input::GetInput()->getWindowSize(w, h);

			screenSize[2].x = float(w);
			screenSize[2].y = float(h);

			shaders.orthoMatrix = glm::ortho(0.0f, float(w), float(h), 0.0f);

			gRenderEngine->setDepthTest(false);

			for (it; it != eit; it++) {
				screenSize[2].x = float(w);
				screenSize[2].y = float(h);

				screenSize[0].x = 0;
				screenSize[0].y = 0;

				gRenderEngine->setScissorTest(false);
				(*it)->render(screenSize, shaders);
			}

			gRenderEngine->setDepthTest(true);
		}
	}
}