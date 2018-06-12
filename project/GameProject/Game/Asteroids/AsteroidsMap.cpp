
/// Internal Includes
#include "AsteroidsMap.hpp"
#include "../../Engine/Core/System.hpp"
#include "../../Engine/Graphics/Graphics.hpp"
#include "../../Engine/Input/Input.hpp"

/// External Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <RenderEngine/IRenderEngine.hpp>


/// Std Includes

AsteroidsMap::AsteroidsMap(CameraInput& camInput, Camera& cam) : camInput(camInput), camera(cam) {
	asteroidShip = new Ship();

	asteroidShader = gRenderEngine->createShaderObject();
	asteroidShader->init();

	std::string vs = Engine::System::readShader("data/shaders/asteroids/ast.vs.glsl");
	std::string fs = Engine::System::readShader("data/shaders/asteroids/ast.fs.glsl");

	asteroidShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
	asteroidShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	asteroidShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

	if (!asteroidShader->buildShader()) {
		assert(0 && "asteroidShader failed to build");
	}

	asteroidVP = asteroidShader->getShaderUniform("viewProjMatrix");
	asteroidMdl = asteroidShader->getShaderUniform("worldMat");
	asteroidColor = asteroidShader->getShaderUniform("selectedColor");

	bulletMesh = gRenderEngine->createMesh();
	bulletMesh->init(MeshPrimitiveType::TRIANGLE);

	float points[] = {
		+2.0F * 1.0F, 0.0F, +2.0F * 1.0F, 0.0F, 0.0F,
		+2.0F * 1.0F, 0.0F, -2.0F * 1.0F, 0.0F, 0.0F,
		-2.0F * 1.0F, 0.0F, -2.0F * 1.0F, 0.0F, 0.0F,

		-2.0F * 1.0F, 0.0F, +2.0F * 1.0F, 0.0F, 0.0F,
		+2.0F * 1.0F, 0.0F, +2.0F * 1.0F, 0.0F, 0.0F,
		-2.0F * 1.0F, 0.0F, -2.0F * 1.0F, 0.0F, 0.0F,

	};

	bulletMesh->setMeshData(&points, sizeof(points), MeshDataLayout::VERT_UV);

	camInput.setCam(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0));

	int w = 0;
	int h = 0;
	Engine::Input::Input::GetInput()->getWindowSize(w, h);

	*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::ortho(float(-w), float(w), float(-h), float(h));

	//			*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::ortho(-100.0F, 100.0F, -100.0F, 100.0F);

	gRenderEngine->setClearColor(0.0F, 0.0F, 0.0F, 1.0F);

	for (size_t i = 0; i < 3; i++) {
		Asteroid* a = new Asteroid(ASTEROID_SIZE::LARGE);
		asteroids.push_back(a);
	}

}

AsteroidsMap::~AsteroidsMap() {

	auto bit = bullets.begin();
	auto beit = bullets.end();

	while (bit != beit) {
		Bullet* b = *bit;

		delete b;
		bit = bullets.erase(bit);
		beit = bullets.end();
	}

	auto ait = asteroids.begin();
	auto aeit = asteroids.end();

	while (ait != aeit) {
		Asteroid* a = *ait;

		delete a;
		ait = asteroids.erase(ait);
		aeit = asteroids.end();
	}

	bulletMesh->release();
	asteroidShader->release();
	delete asteroidShip;

}


IPlayer * AsteroidsMap::getPlayer() const {
	return nullptr;
}

void AsteroidsMap::update(float dt) {

	asteroidShip->update(dt);

	size_t nBullets = bullets.size();

	for (size_t i = 0; i < nBullets; i++) {
		bullets[i]->update(dt);
	}

	auto it = bullets.begin();
	auto eit = bullets.end();

	while (it != eit) {
		Bullet* b = *it;

		if (b->dead()) {
			delete b;
			it = bullets.erase(it);
			eit = bullets.end();
		} else {
			it++;
		}
	}

	size_t nAsteroids = asteroids.size();

	for (size_t i = 0; i < nAsteroids; i++) {
		asteroids[i]->update(dt);
	}

	nBullets = bullets.size();

	for (size_t i = 0; i < nAsteroids; i++) {

		for (size_t j = 0; j < nBullets; j++) {

			if (asteroids[i]->collide(bullets[j]->xPos, bullets[j]->yPos)) {

				bullets[j]->ttl = -1.0F;

				printf("Collision\n");

			}
		}
	}

	auto ait = asteroids.begin();
	auto aeit = asteroids.end();

	while (ait != aeit) {
		Asteroid* a = *ait;

		if (a->isDead()) {
			ait = asteroids.erase(ait);

			if (a->mSize == ASTEROID_SIZE::LARGE) {
				for (size_t i = 0; i < 3; i++) {
					Asteroid* ad = new Asteroid(ASTEROID_SIZE::MEDIUM, a->xPos, a->yPos);
					asteroids.push_back(ad);
				}
			}

			if (a->mSize == ASTEROID_SIZE::MEDIUM) {
				for (size_t i = 0; i < 3; i++) {
					Asteroid* ad = new Asteroid(ASTEROID_SIZE::SMALL, a->xPos, a->yPos);
					asteroids.push_back(ad);
				}
			}

			delete a;
			ait = asteroids.begin();
			aeit = asteroids.end();
		} else {
			ait++;
		}
	}

	float sx = 0, sy = 0;

	if (asteroidShip->getShootPoint(sx, sy)) {

		Bullet* b = new Bullet(sx, sy, asteroidShip->rot);

		bullets.push_back(b);

	}

}

void AsteroidsMap::updateRenderBatch(RenderBatch & batch) {}

void AsteroidsMap::render() {

	// ACTUAL RENDERING

	glm::vec3 white(1.0F);

	glm::mat4 mdlMat(1.0F);

	mdlMat *= glm::rotate(asteroidShip->rot, glm::vec3(0, 1, 0));

	mdlMat *= glm::transpose(glm::translate(glm::mat4(), glm::vec3(asteroidShip->xPos, 0, asteroidShip->yPos)));

	//mdlMat = glm::scale(mdlMat, glm::vec3(10.0F));

	glm::mat4 vpMat = camera.viewProjection();

	asteroidShader->useShader();
	asteroidShader->bindData(asteroidVP, UniformDataType::UNI_MATRIX4X4, &vpMat);
	asteroidShader->bindData(asteroidMdl, UniformDataType::UNI_MATRIX4X4, &mdlMat);
	asteroidShader->bindData(asteroidColor, UniformDataType::UNI_FLOAT3, &white);

	asteroidShip->render();

	bulletMesh->bind();

	size_t nBullets = bullets.size();
	for (size_t i = 0; i < nBullets; i++) {

		mdlMat = glm::transpose(glm::translate(glm::mat4(), glm::vec3(bullets[i]->xPos, 0, bullets[i]->yPos)));
		asteroidShader->bindData(asteroidMdl, UniformDataType::UNI_MATRIX4X4, &mdlMat);
		bulletMesh->render();
	}

	size_t nAsteroids = asteroids.size();

	for (size_t i = 0; i < nAsteroids; i++) {
		mdlMat = glm::mat4();
		mdlMat *= glm::rotate(asteroids[i]->rot, glm::vec3(0, 1, 0));
		mdlMat *= glm::transpose(glm::translate(glm::mat4(), glm::vec3(asteroids[i]->xPos, 0, asteroids[i]->yPos)));
		asteroidShader->bindData(asteroidMdl, UniformDataType::UNI_MATRIX4X4, &mdlMat);
		asteroids[i]->render();
	}

	// END ACTUAL RENDERING

}
