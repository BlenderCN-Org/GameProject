#ifndef GAME_HPP
#define GAME_HPP

///Internal Includes
#include "CameraInput.hpp"
#include "Camera.hpp"

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Mesh/StaticMesh.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

class Game {
public:

	Game(CEngine* _engine);
	virtual ~Game();

	void update(float dt);

	void render();

private:

	bool paused;
	Engine::Graphics::CGui* gameGui;
	Engine::Graphics::Gui::Panel* metrixPanel;
	Engine::Graphics::Gui::Label* infoLabel;
	Engine::Graphics::Texture::Texture2D* panelTexture;

	CEngine* engine;
	CameraInput camInput;
	Camera camera;

	glm::mat4 vpMat;

	Engine::Graphics::Mesh::StaticMesh* mesh;
	IShaderObject* shader;

	int vpLocation;
	int matLocation;
	int selectedLoc;

	float dtOneSec;
	int fps;
	int fpsCounter;

};	


#endif
