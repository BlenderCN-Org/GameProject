#ifndef RENDEROBJECT_HPP
#define RENDEROBJECT_HPP

#include "../IRenderObject.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class RenderObject : public IRenderObject
{
public:

	virtual void init();
	virtual void release();

	virtual void setMesh(IMesh* mesh);

	virtual void addInstance(float* worldMatrices);

	virtual void render();

private:
	
	IMesh* mesh;



};


#endif