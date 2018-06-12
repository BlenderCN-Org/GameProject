#ifndef GAME_HPP
#define GAME_HPP

///Internal Includes
#include "Camera.hpp"
#include "CameraInput.hpp"
#include "CameraPath.hpp"
#include "Editor.hpp"
#include "Loader/MapLoader.hpp"
#include "Map.hpp"
#include "MainMenu.hpp"
#include "PhysicsParticle.hpp"
#include "Player.hpp"
#include "RenderBatch.hpp"
#include "Sky.hpp"

#include "Asteroids/AsteroidsMap.hpp"
#include "Editor/3dViewGizmo.hpp"

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Mesh/Mesh.hpp"
#include "../Engine/Graphics/Mesh/MirrorMesh.hpp"
#include "../Engine/Graphics/Gui/TextureView.hpp"
#include "../Engine/Graphics/Gui/Cursor.hpp"
#include "../Engine/Utils/FPSCounter.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

enum class GameState {
	MAIN_MENU,
	PLAY,
	PLAY_ASTEROIDS,
	EDIT,
	PAUSE,
};

class Game {
public:

	Game(CEngine* _engine);
	virtual ~Game();

	void update(float dt, uint64_t clocks);

	void render();

private:
	CEngine * engine;
	IMap* map;
	MainMenu* menu;

	Engine::Core::FPSCounter fpsCounter;

	GameState currentState;
	GameState lastState;

	MapLoader mapLoader;

	CameraInput camInput;
	Camera camera;

	void updateMenu(float dt);
	void updatePlay(float dt);
	void updateEdit(float dt);
	void updateAsteroids(float dt);
	void updatePaused(float dt);

	void renderSky();
	void renderScene();
	void renderShadowMap();

	void present();

	void renderAsteroids();

	RenderBatch* batchTmp;

	Editor* editor;

	Engine::Graphics::CGui* gameGui;
	Engine::Graphics::Gui::Panel* metrixPanel;
	Engine::Graphics::Gui::Label* infoLabel;
	Engine::Graphics::Texture::Texture2D* panelTexture;
	
	//Player* player;

	glm::mat4 vpMat;

	float rotSpeed;
	float meshRotation;
	float rotTime;

	Engine::Graphics::Mesh::CMesh* mesh;
	Engine::Graphics::Mesh::MirrorMesh* mirror;
	//Sky* sky;

	float r;
	float a;

	Engine::Graphics::Mesh::CMesh* aabbTest;

	IFrameBuffer* gBuffer;
	IFrameBuffer* shadowMap;

	IShaderObject* shader;
	IShaderObject* gBufferShader;
	IShaderObject* gBufferBlit;

	IShaderObject* shadowShader;

	int vpLocation;
	int matLocation;
	int selectedLoc;

	int vpLocationGBuff;
	int matLocationGBuff;
	int refMatLocationGBuff;
	int selectedLocGBuff;
	int clipPlane;
	int skinArray;

	int blitTexDiff;
	int blitTexNorm;
	int blitTexWPos;
	int blitTextShadow;
	int blitDepthMvp;
	int blitEyePos;

	int shadowMVP;

	IShaderObject* testShader;

	std::vector<PhysicsParticle*> physParticles;

	std::vector<Renderable*> frameObjects;


	ViewGizmo* viewGizmo;

};


#endif
