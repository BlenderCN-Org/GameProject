#ifndef IMESH_HPP
#define IMESH_HPP

/*
The layout specify what components are in the dataset
*/

enum class MeshPrimitiveType {
	POINT,
	LINE,
	TRIANGLE,
	TRIANGLE_STRIP,
	QUAD,
};

enum MeshDataLayout {
	VERT,
	VERT_UV,
	VERT_UV_NORM,
	VERT_UV_COL,
	NONE,
};

/**
 elements to mapp data.
 if offset is 0 it will be calculated depending on previous elements
*/

enum class LayoutDataType : unsigned char {
	POSITION = 0U,
	UV = 1U,
	COLOR = 2U,
	NORMAL = 3U,
	WEIGHT = 4U,
	INDEX = 5U,
	INVALID = 0xFF
};

struct DataLayout {
	LayoutDataType dataType;
	unsigned char offset;
};

class IMesh {
public:

	virtual void init(MeshPrimitiveType ptype) = 0;
	virtual void release() = 0;

	virtual void setMeshData(void* data, size_t size, MeshDataLayout layout) = 0;
	virtual void setMeshData(void* data, size_t size, DataLayout* layouts, unsigned char numLayouts) = 0;

	virtual float getRadius() = 0;

	virtual void bind() = 0;
	virtual void render() = 0;

	virtual void* map(size_t &dataSize) = 0;
	virtual void unmap() = 0;
	virtual void flush() = 0;
};

#endif