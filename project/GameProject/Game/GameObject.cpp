#include "GameObject.hpp"

void GameObject::init() {}

void GameObject::update(float dt) {}

void GameObject::addComponent(BaseComponent* component) {
	components[component->getComponentID()] = component;
	component->setAttachedOn(this);
}

void GameObject::setMatrix(glm::mat4 mat) {
	worldMatrix = mat;
}

void GameObject::setMesh(IMesh * _mesh) {
	mesh = _mesh;
}

glm::mat4 GameObject::getMatrix() const {
	return worldMatrix;
}

IMesh * GameObject::getMesh() const {
	return mesh;
}
