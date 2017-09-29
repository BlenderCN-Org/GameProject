#ifndef MESH_GL_HPP
#define MESH_GL_HPP

#include "../IMesh.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

struct TransferInfo {
	void* data;
	size_t size;
	bool reallocated;
};

class Mesh_gl : public IMesh {
public:

	virtual void init(MeshPrimitiveType p);
	virtual void release();

	virtual void setMeshData(void* data, size_t size, MeshDataLayout layout);
	virtual float getRadius();

	virtual void bind();
	virtual void render();

	virtual void* map(size_t &dataSize);
	virtual void unmap();
	virtual void flush();

private:
	MeshPrimitiveType primitiveType;
	MeshDataLayout dataLayout;

	bool usesIndexBuffer;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;

	GLsizei vertexCount;
	size_t triangleCount;

	GLenum meshPrimitive;
	// mapping stuff

	float radius;

	bool isMapped;
	void* mapPtr;
	size_t mappedSize;

	TransferInfo tInfo;
};

#endif