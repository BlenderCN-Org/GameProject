#ifndef IMESH_HPP
#define IMESH_HPP

/*
The layout specify what components are in the dataset
*/

enum class MeshPrimitiveType
{
	POINT,
	LINE,
	TRIANGLE,
	QUAD,
};

enum MeshDataLayout {
	VERT,
	VERT_UV,
	VERT_UV_NORM,

	NONE,
};

class IMesh {

public:

	virtual void init(MeshPrimitiveType ptype) = 0;
	virtual void release() = 0;

	virtual void setMeshData(void* data, size_t size, MeshDataLayout layout) = 0;

	virtual void bind() = 0;
	virtual void render() = 0;

	virtual void* map(size_t &dataSize) = 0;
	virtual void unmap() = 0;
	virtual void flush() = 0;


};

#endif