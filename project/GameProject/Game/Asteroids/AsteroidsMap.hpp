#ifndef ASTEROIDS_MAP_HPP
#define ASTEROIDS_MAP_HPP

/// Internal Includes
#include "../IMap.hpp"
#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "Ship.hpp"

#include "../CameraInput.hpp"
#include "../Camera.hpp"

/// External Includes

/// Std Includes

class AsteroidsMap : public IMap {

public:

	AsteroidsMap(CameraInput& camIn, Camera& cam);
	virtual ~AsteroidsMap();

	// Inherited via IMap
	virtual Cell* getCurrentCell() const override { return nullptr; };
	virtual IPlayer * getPlayer() const override;

	virtual void update(float dt) override;

	virtual void updateRenderBatch(RenderBatch & batch) override;

	virtual void render() override;

private:

	CameraInput& camInput;
	Camera& camera;

	Ship * asteroidShip;

	std::vector<Asteroid*> asteroids;
	std::vector<Bullet*> bullets;

	IShaderObject* asteroidShader;
	int asteroidVP;
	int asteroidMdl;
	int asteroidColor;

	IMesh* bulletMesh;

};

#endif