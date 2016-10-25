#include "Mesh.hpp"

#include "../Utils/MemoryManager.hpp"
#include <memory>

struct Vertex5 {
	Vertex5(float _x, float _y, float _z, float _u, float _v) {
		x = _x;
		y = _y;
		z = _z;
		u = _u;
		v = _v;
	}
	float x, y, z;
	float u, v;
};

void Mesh::init(MeshPrimitiveType ptype) {
	primitiveType = ptype;
	usesIndexBuffer = false;
	isMapped = false;
	mapPtr = nullptr;
	dataLayout = NONE;
	glCreateBuffers(1, &vertexBuffer);
	glCreateBuffers(1, &indexBuffer);
	glCreateVertexArrays(1, &vertexArrayObject);

	vertexCount = 0;
	triangleCount = 0;

}

void Mesh::release() {

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vertexArrayObject);

	delete this;
}

void Mesh::setMeshData(void * data, size_t size, MeshDataLayout layout) {

	dataLayout = layout;
	if (layout = VERT_UV)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		glBindVertexArray(vertexArrayObject);

		// vertex in location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)0);

		// uv in location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)(sizeof(GLfloat) * 3));

		vertexCount = (GLsizei)size / sizeof(Vertex5);

	}
}


void Mesh::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindVertexArray(vertexArrayObject);
}

void Mesh::render() {

	if (usesIndexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_SHORT, 0);
	} else {
		if(primitiveType == MeshPrimitiveType::TRIANGLE )
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		else if(primitiveType == MeshPrimitiveType::TRIANGLE_STRIP )
			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
	}
}

void * Mesh::map(size_t & dataSize)
{
	void* dataPtr = nullptr;

	if ( !isMapped ) {

		if ( dataLayout = VERT_UV ) {
			dataSize = vertexCount * sizeof(Vertex5);
		}

		dataPtr = MemoryManager_alloc(char, dataSize);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		void* d = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
		memcpy(dataPtr, d, dataSize);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		isMapped = true;
		mapPtr = dataPtr;
		mappedSize = dataSize;
	}
	else
	{
		dataPtr = mapPtr;
	}

	return dataPtr;
}

void Mesh::unmap()
{
	if ( isMapped ) {
		flush();
		MemoryManager::getMemoryManager()->deallocate(mapPtr);
		mapPtr = nullptr;
		isMapped = false;
	}

}

void Mesh::flush()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mappedSize, mapPtr, GL_STATIC_DRAW);
}



