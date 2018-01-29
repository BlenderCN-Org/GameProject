#ifndef SKY_HPP
#define SKY_HPP

/// Internal Includes
#include "../Engine/Graphics/SkyDome.hpp"


/// External Includes
#include <RenderEngine/IRenderEngine.hpp>
#include <glm/glm.hpp>


class Sky {

public:

	Sky();
	~Sky();

	void update(float dt);

	void render(glm::mat4& vpMat, glm::vec3& cameraPos, glm::vec3& cameraDir, glm::mat4* reflect = nullptr);

private:

	SkyDome *skyDome;

	IShaderObject* skyDomeShader;

	int skydomeVpLocation;
	int skydomeMatLocation;
	int skydomeTimeLoc;
	int skydomeCamPos;
	int skydomeEyeDir;
	int skydomeSunMoon;

	float skyTime;
	glm::vec3 sunMoonDir;

};

#endif