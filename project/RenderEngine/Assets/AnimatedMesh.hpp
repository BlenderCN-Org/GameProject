#ifndef ANIMATEDMESH_HPP
#define ANIMATEDMESH_HPP

#include "../IAnimatedMesh.hpp"

class AnimatedMesh : public IAnimatedMesh{

public:

	virtual void init();
	virtual void release();

	virtual void setMeshData(void* data, size_t size, MeshDataLayout layout);

	virtual void bind() {};
	virtual void render() {};

	virtual void* map(size_t &dataSize) { return nullptr; };
	virtual void unmap() {};
	virtual void flush() {};

private:



};

#endif