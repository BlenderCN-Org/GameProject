#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm\glm.hpp>
#include <map>

#include "Components\BaseComponent.hpp"
#include "Components\IBaseComponent.hpp"

class IMesh;

class GameObject
{
public:

	virtual void init();
	virtual void update(float dt);

	void addComponent(BaseComponent* component);

	template <typename T>
	BaseComponent* getComponent();

	uint32_t counter;

	void setMatrix(glm::mat4 mat);
	void setMesh(IMesh* mesh);

	glm::mat4 getMatrix() const;
	IMesh* getMesh() const;

private:

	uint64_t gameObjectId;
	glm::mat4 worldMatrix;
	IMesh* mesh;

	std::map<uint64_t, BaseComponent*> components;
};

#endif

template<typename T>
inline BaseComponent* GameObject::getComponent() {
	if ( components.count(IBaseComponent<T>::TypeID) ) {
		return components.find(IBaseComponent<T>::TypeID);
	}
	return nullptr;
}