#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "IBaseComponent.hpp"

#include <glm/glm.hpp>

class TransformComponent : public IBaseComponent<TransformComponent> {
public:

	void init();

	void setPosition(glm::vec3 const &newPosition);
	void setrotation(glm::vec3 const &newRotation);
	void setScale(glm::vec3 const &newScale);

	void translate(glm::vec3 const &movement);
	void rotate(glm::vec3 const &rotation);
	void scale(glm::vec3 const &scale);

	glm::mat4 getModelMatrix();

private:

	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};

#endif