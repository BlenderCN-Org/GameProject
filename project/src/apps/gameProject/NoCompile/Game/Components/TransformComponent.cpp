#include "TransformComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

const uint32_t IBaseComponent<TransformComponent>::typeID = (uint32_t)Components::TRANSFORM_COMPONENT;

void TransformComponent::init() {
	mPosition = glm::vec3(0.0f);
	mRotation = glm::vec3(0.0f);
	mScale = glm::vec3(1.0f);
}

void TransformComponent::setPosition(glm::vec3 const & newPosition) {
	mPosition = newPosition;
}

void TransformComponent::setrotation(glm::vec3 const & newRotation) {
	mRotation = newRotation;
}

void TransformComponent::setScale(glm::vec3 const & newScale) {
	mScale = newScale;
}

void TransformComponent::translate(glm::vec3 const & movement) {
	mPosition += movement;
}

void TransformComponent::rotate(glm::vec3 const & rotation) {
	mRotation += rotation;
}

void TransformComponent::scale(glm::vec3 const & scale) {
	mScale *= scale;
}

glm::mat4 TransformComponent::getModelMatrix() {
	glm::mat4 modelMat = glm::mat4(1.0f);

	glm::mat4 rot = glm::mat4(glm::quat(mRotation));

	modelMat *= rot;
	modelMat = glm::scale(modelMat, mScale);
	modelMat = glm::transpose(glm::translate(modelMat, mPosition));

	return modelMat;
}