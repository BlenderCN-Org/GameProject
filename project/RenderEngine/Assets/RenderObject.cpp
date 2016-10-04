#include "RenderObject.hpp"

#include "Mesh.hpp"

void RenderObject::init()
{

}

void RenderObject::release()
{
	
	delete this;
}

void RenderObject::setMesh(IMesh * _mesh) {
	mesh = _mesh;
}

void RenderObject::addInstance(float * worldMatrices) {
	
}

void RenderObject::render()
{
	Mesh* m = (Mesh*)mesh;
	m->bind();
	m->render();
}
