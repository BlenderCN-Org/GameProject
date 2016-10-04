#ifndef IRENDEROBJECT_HPP
#define IRENDEROBJECT_HPP

#include "IMesh.hpp"


class IRenderObject
{
public:

	virtual void init() = 0;
	virtual void release() = 0;

	virtual void setMesh(IMesh* mesh) = 0;

	virtual void addInstance(float* worldMatrices) = 0;

	virtual void render() = 0;

};

#endif