#ifndef GAME_HPP
#define GAME_HPP

///Internal Includes
#include "CameraInput.hpp"
#include "Camera.hpp"
#include "CameraPath.hpp"

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Mesh/StaticMesh.hpp"
#include "../Engine/Graphics/Mesh/MirrorMesh.hpp"
#include "../Engine/Graphics/SkyDome.hpp"

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

	void renderSky();
	void renderScene();
	void renderShadowMap();

	bool paused;
	Engine::Graphics::CGui* gameGui;
	Engine::Graphics::Gui::Panel* metrixPanel;
	Engine::Graphics::Gui::Label* infoLabel;
	Engine::Graphics::Texture::Texture2D* panelTexture;

	CEngine* engine;
	CameraInput camInput;
	Camera camera;
	CameraPath camPath;

	glm::mat4 vpMat;

	Engine::Graphics::Mesh::StaticMesh* mesh;
	Engine::Graphics::Mesh::MirrorMesh* mirror;
	SkyDome* skyDome;

	float r;
	float a;

	IFrameBuffer* gBuffer;
	IFrameBuffer* shadowMap;

	IShaderObject* shader;
	IShaderObject* gBufferShader;
	IShaderObject* skyDomeShader;
	IShaderObject* gBufferBlit;

	IShaderObject* shadowShader;

	int vpLocation;
	int matLocation;
	int selectedLoc;

	int vpLocationGBuff;
	int matLocationGBuff;
	int refMatLocationGBuff;
	int selectedLocGBuff;

	int skydomeVpLocation;
	int skydomeMatLocation;
	int skydomeTimeLoc;
	int skydomeCamPos;
	int skydomeEyeDir;
	int skydomeSunMoon;

	float skyTime;
	glm::vec3 sunMoonDir;

	int blitTexDiff;
	int blitTexNorm;
	int blitTexWPos;
	int blitTextShadow;
	int blitDepthMvp;

	int shadowMVP;

	float dtOneSec;
	int fps;
	int fpsCounter;

};	


#endif
