#ifndef ANIMATEDMESH_GL_HPP
#define ANIMATEDMESH_GL_HPP

#include "../IAnimatedMesh.hpp"

class AnimatedMesh_gl : public IAnimatedMesh {
public:

	virtual void init(MeshPrimitiveType ptype);
	virtual void release();

	virtual void setMeshData(void* data, size_t size, MeshDataLayout layout);
	virtual void setMeshData(void* data, size_t size, DataLayout* layouts, unsigned char numLayouts);
	virtual float getRadius();

	virtual void bind() {};
	virtual void render() {};

	virtual void* map(size_t &dataSize) { return nullptr; };
	virtual void unmap() {};
	virtual void flush() {};

private:
};

#endif