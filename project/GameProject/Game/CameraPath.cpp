#include "CameraPath.hpp"

CameraPath::CameraPath()
	: camInput(nullptr)
	, enabled(false)
	, currentPathTime(0.0F)
	, pathIndex(0U)
	, maxPaths(0U)
	, cameraPaths() {

	/*
	5, 1, 0 (-1, 0, 0)
	4, 1, 0 (-1, 0, 0)
	*/
	CameraPathInfo info;
	info.startPos = glm::vec3(5, 1, 0);
	info.endPos = glm::vec3(3, 1, 0);
	info.startDir = glm::vec3(-1, 0, 0);
	info.endDir = glm::vec3(-1, 0, 0);
	info.duration = 10.0F;
	cameraPaths.push_back(info);

	/*
	13.2, 6.11, 9.67   (-0.85, -0.26, -0.34)
	-5.92, 3.35, 13.06 (0.24, -0.19, -0.95)
	*/
	info.startPos = glm::vec3(13.2, 6.11, 9.67);
	info.endPos = glm::vec3(-5.92, 3.35, 13.06);
	info.startDir = glm::vec3(-0.85, -0.26, -0.34);
	info.endDir = glm::vec3(0.24, -0.19, -0.95);
	info.duration = 15.0F;
	cameraPaths.push_back(info);
	maxPaths = cameraPaths.size();

	/*
	-8.31, 1.40, -5.24  (0.65, -0.05, 0.75)
	-6.59, 1.75, -11.31 (0.33, -0.09, 0.94)
	*/
	info.startPos = glm::vec3(-8.31, 1.40, -5.24);
	info.endPos = glm::vec3(-6.59, 1.75, -9.31);
	info.startDir = glm::vec3(0.65, -0.05, 0.75);
	info.endDir = glm::vec3(0.33, -0.09, 0.94);
	info.duration = 15.0F;
	cameraPaths.push_back(info);
	maxPaths = cameraPaths.size();
	/*
	-6.59, 1.75, -11.31 (0.33, -0.09, 0.94)
	4.48, 1.75, -12.46 (-0.09, -0.06, 0.99)
	*/
	info.startPos = glm::vec3(-6.59, 1.75, -9.31);
	info.endPos = glm::vec3(4.48, 1.75, -12.46);
	info.startDir = glm::vec3(0.33, -0.09, 0.94);
	info.endDir = glm::vec3(-0.09, -0.06, 0.99);
	info.duration = 15.0F;
	cameraPaths.push_back(info);
	maxPaths = cameraPaths.size();
	/*
	4.48, 1.75, -12.46 (-0.09, -0.06, 0.99)
	4.48, 1.75, -12.46 (-0.06, -0.05, 0.77)
	*/
	info.startPos = glm::vec3(4.48, 1.75, -12.46);
	info.endPos = glm::vec3(4.48, 1.75, -12.46);
	info.startDir = glm::vec3(-0.09, -0.06, 0.99);
	info.endDir = glm::vec3(-0.63, -0.05, 0.77);
	info.duration = 8.0F;
	cameraPaths.push_back(info);
	maxPaths = cameraPaths.size();
}

void CameraPath::init(CameraInput* cam) {
	camInput = cam;
}

void CameraPath::followPaths(bool follow) {
	enabled = follow;
}

void CameraPath::update(float dt) {

	if (camInput && enabled) {

		glm::vec3 position;
		glm::vec3 direction;

		if (maxPaths != 0) {

			const CameraPathInfo pathInfo = cameraPaths[pathIndex];

			float t = currentPathTime / pathInfo.duration;

			position = (pathInfo.startPos * (1.0F - t)) + (pathInfo.endPos * t);
			direction = (pathInfo.startDir * (1.0F - t)) + (pathInfo.endDir * t);

			camInput->setCam(position, direction);

			currentPathTime += dt;
			if (currentPathTime > pathInfo.duration) {
				currentPathTime = 0.0F;
				pathIndex = (pathIndex + 1) % maxPaths;
			}


		}
	}
}