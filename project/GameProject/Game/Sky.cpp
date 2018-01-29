#include "Sky.hpp"
#include "../Engine/Graphics/Graphics.hpp"

#include <string>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// @temporary
inline std::string readShader(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		//Debug::DebugOutput("Could not open file %s", filePath);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}
// @temporary end

Sky::Sky() {

	skyDome = new SkyDome();

	skyDomeShader = gRenderEngine->createShaderObject();
	skyDomeShader->init();

	std::string vs = readShader("data/shaders/skydome.vs.glsl");
	std::string fs = readShader("data/shaders/skydome.fs.glsl");

	skyDomeShader->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	skyDomeShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	skyDomeShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if (!skyDomeShader->buildShader()) {
		assert(0 && "shader failed to build");
	}

	skydomeVpLocation = skyDomeShader->getShaderUniform("viewProjMatrix");
	skydomeMatLocation = skyDomeShader->getShaderUniform("worldMat");
	skydomeTimeLoc = skyDomeShader->getShaderUniform("time");
	skydomeCamPos = skyDomeShader->getShaderUniform("cameraPos");
	skydomeEyeDir = skyDomeShader->getShaderUniform("eyeDir");
	skydomeSunMoon = skyDomeShader->getShaderUniform("sunMoonDir");

}

Sky::~Sky() {
	delete skyDome;

	skyDomeShader->release();
}

void Sky::update(float dt) {

	skyTime += dt * 0.01F;
	if (skyTime > 1.0F) {
		skyTime -= 1.0F;
	}
	//skyTime = 0.5F;
	//a += 0.3F * dt;


	const float r = 100.0F;
	const float PI = glm::pi<float>();

	float s = (glm::abs(glm::sin(PI * skyTime * 2.0F + (PI * 0.5F))) * 1.6F) - 0.6F;
	float c = glm::cos(PI * skyTime * 2.0F + (PI * 0.5F));

	if (skyTime >= 0.25F && skyTime <= 0.75F) {
		c = -c;
	}
	if (s > 0.0F) {
		s *= 0.2F;
	}

	sunMoonDir = glm::vec3(-r * 0.4F, r * s, r * c);

	sunMoonDir = normalize(glm::vec3(0) - sunMoonDir);

}

void Sky::render(glm::mat4& vpMat, glm::vec3& cameraPos, glm::vec3& cameraDir, glm::mat4* reflect) {

	skyDomeShader->useShader();

	glm::mat4 m;
	m = glm::translate(m, cameraPos);
	m = glm::transpose(m);


	skyDomeShader->bindData(skydomeVpLocation, UniformDataType::UNI_MATRIX4X4, &vpMat);
	skyDomeShader->bindData(skydomeMatLocation, UniformDataType::UNI_MATRIX4X4, &m);
	skyDomeShader->bindData(skydomeTimeLoc, UniformDataType::UNI_FLOAT, &skyTime);
	skyDomeShader->bindData(skydomeCamPos, UniformDataType::UNI_FLOAT3, &cameraPos);
	skyDomeShader->bindData(skydomeEyeDir, UniformDataType::UNI_FLOAT3, &cameraDir);

	if (reflect) {
		sunMoonDir = glm::vec3(glm::vec4(sunMoonDir, 1.0F) * (*reflect));
	}

	skyDomeShader->bindData(skydomeSunMoon, UniformDataType::UNI_FLOAT3, &sunMoonDir);

	gRenderEngine->depthMask(false);

	skyDome->render();

	gRenderEngine->depthMask(true);

}

