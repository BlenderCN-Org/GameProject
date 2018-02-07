
#include "Asteroid.hpp"

#include "../../Engine/Graphics/Graphics.hpp"
#include "../../Engine/Input/Input.hpp"
#include "../../Engine/Core/System.hpp"


Asteroid::Asteroid(ASTEROID_SIZE size, float ox, float oy) {

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	int wndX = 0;
	int wndY = 0;
	Engine::Input::Input::GetInput()->getWindowSize(wndX, wndY);

	mSize = size;

	asteroid = gRenderEngine->createMesh();
	asteroid->init(MeshPrimitiveType::LINE);

	if (size == ASTEROID_SIZE::LARGE) {
		xPos = (Engine::System::randomFloat(0.0F, 2.0F * wndX) - wndX);
		yPos = (Engine::System::randomFloat(0.0F, 2.0F * wndY) - wndY);

		xVel = (Engine::System::randomFloat(0.0F, 4.0F) - 2.0F);
		yVel = (Engine::System::randomFloat(0.0F, 4.0F) - 2.0F);
	}

	if (size == ASTEROID_SIZE::MEDIUM) {
		xPos = ox;
		yPos = oy;

		xVel = (Engine::System::randomFloat(0.0F, 6.0F) - 3.0F);
		yVel = (Engine::System::randomFloat(0.0F, 6.0F) - 3.0F);
	}

	if (size == ASTEROID_SIZE::SMALL) {
		xPos = ox;
		yPos = oy;

		xVel = (Engine::System::randomFloat(0.0F, 8.0F) - 4.0F);
		yVel = (Engine::System::randomFloat(0.0F, 8.0F) - 4.0F);
	}


	rot = 0.0F;

	rotVel = (Engine::System::randomFloat(0.0F, 2.0F * 3.1415F) - 3.1415F) * 0.003F;

	float fx = 0;
	float fy = 0;

	float randMin = 0.0;
	float randMax = 0.0;

	if (size == ASTEROID_SIZE::LARGE) {
		randMax = 70.0F;
		randMin = 50.0F;
	}

	if (size == ASTEROID_SIZE::MEDIUM) {
		randMax = 40.0F;
		randMin = 20.0F;
	}

	if (size == ASTEROID_SIZE::SMALL) {
		randMax = 15.0F;
		randMin = 7.0F;
	}

	for (int i = 0; i < 23; i++) {

		float TWO_PI = 2.0F * 3.1415F;

		float part = (float)i / 23.0F;

		float d = Engine::System::randomFloat(randMin, randMax);

		float py = glm::cos(TWO_PI * part) * d;
		float px = glm::sin(TWO_PI * part) * d;

		if (i == 0) {
			fx = px;
			fy = py;
		}

		points[(i * 5) + 0] = px;
		points[(i * 5) + 1] = 0;
		points[(i * 5) + 2] = py;
		points[(i * 5) + 3] = 0;
		points[(i * 5) + 4] = 0;

		if (i == 22) {
			points[((i + 1) * 5) + 0] = fx;
			points[((i + 1) * 5) + 1] = 0;
			points[((i + 1) * 5) + 2] = fy;
			points[((i + 1) * 5) + 3] = 0;
			points[((i + 1) * 5) + 4] = 0;
		}

	}

	asteroid->setMeshData(&points, sizeof(points), MeshDataLayout::VERT_UV);

	dead = false;

}

Asteroid::~Asteroid() {
	asteroid->release();
}

void Asteroid::update(float dt) {

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	int wndX = 0;
	int wndY = 0;
	Engine::Input::Input::GetInput()->getWindowSize(wndX, wndY);

	rot += rotVel;

	xPos -= xVel;
	yPos += yVel;

	if (yPos < -wndY) {
		yPos = float(wndY);
	}

	if (yPos > wndY) {
		yPos = float(-wndY);
	}

	if (xPos < -wndX) {
		xPos = float(wndX);
	}

	if (xPos > wndX) {
		xPos = float(-wndX);
	}

}

void Asteroid::render() {

	asteroid->bind();
	asteroid->render();

}

bool Asteroid::collide(float x, float y) {

	bool collided = false;

	float precheckDistance = 0.0F;

	if (mSize == ASTEROID_SIZE::LARGE) {
		precheckDistance = 70.0F;
	}
	if (mSize == ASTEROID_SIZE::MEDIUM) {
		precheckDistance = 40.0F;
	}
	if (mSize == ASTEROID_SIZE::SMALL) {
		precheckDistance = 15.0F;
	}

	float dst = glm::sqrt(glm::pow(xPos - x, 2.0F) + glm::pow(yPos - y, 2.0F));

	if (dst < precheckDistance) {
		dead = true;
		collided = true;
	}

	return collided;
}

bool Asteroid::isDead() const {
	return dead;
}