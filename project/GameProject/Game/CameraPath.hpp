#ifndef CAMERA_PATH_HPP
#define CAMERA_PATH_HPP

/// Internal Includes
#include "CameraInput.hpp"

/// External Includes

/// Std Includes
#include <vector>

struct CameraPathInfo
{
	glm::vec3 startPos;
	glm::vec3 endPos;

	glm::vec3 startDir;
	glm::vec3 endDir;

	float duration;
};

class CameraPath {

public:
	CameraPath();

	void init(CameraInput* cam);

	void followPaths(bool follow);
	void update(float dt);

private:

	CameraInput* camInput;

	bool enabled;

	float currentPathTime;
	size_t pathIndex;
	size_t maxPaths;
	std::vector<CameraPathInfo> cameraPaths;

};

#endif
