#include "Mesh_gl.hpp"

#include <glm/glm.hpp>

#include "../Utils/MemoryManager.hpp"
#include <memory>
#include "../ReGlobal.hpp"

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

struct Vertex9 {
	Vertex9(float _x, float _y, float _z, float _u, float _v, float _r, float _g, float _b, float _a) {
		x = _x;
		y = _y;
		z = _z;
		u = _u;
		v = _v;
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	float x, y, z;
	float u, v;
	float r, g, b, a;
};

int layoutSize(DataLayout &layout) {

	switch (layout.dataType) {
		case LayoutDataType::POSITION:
			return 3 * sizeof(float);
		case LayoutDataType::UV:
			return 2 * sizeof(float);
		case LayoutDataType::NORMAL:
			return 3 * sizeof(float);
		case LayoutDataType::COLOR:
			return 4 * sizeof(float);
		case LayoutDataType::WEIGHT:
			return 4 * sizeof(float);
		case LayoutDataType::INDEX:
			return 4 * sizeof(int);
		case LayoutDataType::INVALID:
			return 0;
		default:
			throw "invalid DataType";
			break;
	}
}

int layoutElemetCount(DataLayout &layout) {

	switch (layout.dataType) {
		case LayoutDataType::POSITION:
			return 3;
		case LayoutDataType::UV:
			return 2;
		case LayoutDataType::NORMAL:
			return 3;
		case LayoutDataType::COLOR:
			return 4;
		case LayoutDataType::WEIGHT:
			return 4;
		case LayoutDataType::INDEX:
			return 4;
		case LayoutDataType::INVALID:
			return 0;
		default:
			throw "invalid DataType";
			break;
	}
}

int layoutStride(DataLayout* layout, int size) {

	int stride = 0;

	for (int i = 0; i < size; i++) {
		stride += layoutSize(layout[i]);
	}

	return stride;
}

void Mesh_gl::init(MeshPrimitiveType ptype) {
	primitiveType = ptype;
	usesIndexBuffer = false;
	isMapped = false;
	mapPtr = nullptr;
	tInfo.data = nullptr;
	tInfo.reallocated = false;
	tInfo.size = 0U;
	dataLayout = NONE;

	auto fn = [&]() {
		glCreateBuffers(1, &vertexBuffer);
		glCreateBuffers(1, &indexBuffer);
		glCreateVertexArrays(1, &vertexArrayObject);
	};

	if (activeThread == getThreadId()) {
		fn();
	} else {
		postGlFunction(fn);
	}

	vertexCount = 0;
	triangleCount = 0;
	radius = 0.0f;
}

void Mesh_gl::release() {

	auto fn = [&]() {
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteVertexArrays(1, &vertexArrayObject);
	};

	if (activeThread == getThreadId()) {
		fn();
	} else {
		postGlFunction(fn);
	}

	delete this;
}

void Mesh_gl::setMeshData(void * data, size_t size, MeshDataLayout layout) {
	if (primitiveType == MeshPrimitiveType::TRIANGLE)
		meshPrimitive = GL_TRIANGLES;
	else if (primitiveType == MeshPrimitiveType::TRIANGLE_STRIP)
		meshPrimitive = GL_TRIANGLE_STRIP;
	else if (primitiveType == MeshPrimitiveType::QUAD)
		meshPrimitive = GL_QUADS;
	else if (primitiveType == MeshPrimitiveType::POINT)
		meshPrimitive = GL_POINTS;


	tInfo.data = data;
	tInfo.size = size;
	tInfo.reallocated = false;

	auto fn = [&]() {

		if (dataLayout == VERT_UV) {
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			glBufferData(GL_ARRAY_BUFFER, tInfo.size, tInfo.data, GL_STATIC_DRAW);

			glBindVertexArray(vertexArrayObject);

			// vertex in location 0
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)0);

			// uv in location 1
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)(sizeof(GLfloat) * 3));
		} else if (dataLayout == VERT_UV_COL) {
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			glBufferData(GL_ARRAY_BUFFER, tInfo.size, tInfo.data, GL_STATIC_DRAW);

			glBindVertexArray(vertexArrayObject);

			// vertex in location 0
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex9), (void*)0);

			// uv in location 1
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex9), (void*)(sizeof(GLfloat) * 3));
			// colors in location 2
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex9), (void*)(sizeof(GLfloat) * 5));
		}

		if (tInfo.reallocated) {
			free(tInfo.data);
		}

		tInfo.data = nullptr;
		tInfo.reallocated = false;
		tInfo.size = 0U;
	};

	dataLayout = layout;
	if (layout == VERT_UV) {

		if (activeThread == getThreadId()) {
			fn();
		} else {

			tInfo.data = malloc(size);
			memcpy(tInfo.data, data, size);
			tInfo.reallocated = true;

			postGlFunction(fn);
		}

		vertexCount = (GLsizei)size / sizeof(Vertex5);
		radius = 0;
		Vertex5* verts = (Vertex5*)data;
		for (GLsizei i = 0; i < vertexCount; i++) {
			Vertex5& v = verts[i];
			glm::vec3 v2 = glm::vec3(v.x, v.y, v.z);
			radius = glm::max(radius, glm::distance(v2, glm::vec3(0.0f)));
		}
	} else if (layout == VERT_UV_COL) {
		if (activeThread == getThreadId()) {
			fn();
		} else {

			tInfo.data = malloc(size);
			memcpy(tInfo.data, data, size);
			tInfo.reallocated = true;

			postGlFunction(fn);
		}

		vertexCount = (GLsizei)size / sizeof(Vertex9);
		radius = 0;
		Vertex9* verts = (Vertex9*)data;
		for (GLsizei i = 0; i < vertexCount; i++) {
			Vertex9& v = verts[i];
			glm::vec3 v2 = glm::vec3(v.x, v.y, v.z);
			radius = glm::max(radius, glm::distance(v2, glm::vec3(0.0f)));
		}
	}
}

void attribData(DataLayout& layout, int stride, int &offset) {

	if (layout.dataType != LayoutDataType::INVALID) {
		unsigned char attribLoc = static_cast<unsigned char>(layout.dataType);
		int attribSize = layoutElemetCount(layout);

		glEnableVertexAttribArray(attribLoc);
		size_t o = offset;
		int type = GL_FLOAT;
		if (layout.dataType == LayoutDataType::INDEX) {
			glVertexAttribIPointer(attribLoc, attribSize, GL_INT, stride, (void*)(o));
		} else {
			glVertexAttribPointer(attribLoc, attribSize, type, GL_FALSE, stride, (void*)(o));
		}



		offset += layoutSize(layout);
	}
}

void Mesh_gl::setMeshData(void* data, size_t size, DataLayout* layouts, unsigned char numLayouts) {

	tInfo.data = data;
	tInfo.size = size;
	tInfo.reallocated = false;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, tInfo.size, tInfo.data, GL_STATIC_DRAW);

	glBindVertexArray(vertexArrayObject);

	int stride = layoutStride(layouts, numLayouts);
	int offset = 0;

	for (unsigned char i = 0; i < numLayouts; i++) {

		attribData(layouts[i], stride, offset);
	}

	vertexCount = (GLsizei)size / stride;
	meshPrimitive = GL_TRIANGLES;

	// vertex in location 0
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	//
	//// uv in location 1
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
	//
	//// colors in location 2
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
	//
	//// normals in location 3
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
	//
	//// weights in location 4
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
	//
	//// index in location 5
	//glEnableVertexAttribArray(5);
	//glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
}

float Mesh_gl::getRadius() {
	return radius;
}

void Mesh_gl::bind() {
	if (activeThread == getThreadId()) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindVertexArray(vertexArrayObject);
	} else {
		// only render thread may bind the object for rendering
	}
}

void Mesh_gl::render() {
	if (vertexCount > 0) {
		if (activeThread == getThreadId()) {
			if (usesIndexBuffer) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

				glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_SHORT, 0);
			} else {
				glDrawArrays(meshPrimitive, 0, vertexCount);
			}
		} else {
			// only render thread may render object
		}
	}
}

void * Mesh_gl::map(size_t & dataSize) {
	void* dataPtr = nullptr;

	if (activeThread == getThreadId()) {
		if (!isMapped) {
			if (dataLayout == VERT_UV) {
				dataSize = vertexCount * sizeof(Vertex5);
			}

			dataPtr = new char[dataSize];

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			void* d = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
			memcpy(dataPtr, d, dataSize);
			glUnmapBuffer(GL_ARRAY_BUFFER);

			isMapped = true;
			mapPtr = dataPtr;
			mappedSize = dataSize;
		} else {
			dataPtr = mapPtr;
		}
	} else {
		// only allow mapping from render thread, for now
	}

	return dataPtr;
}

void Mesh_gl::unmap() {
	if (activeThread == getThreadId()) {
		if (isMapped) {
			flush();
			delete mapPtr;
			mapPtr = nullptr;
			isMapped = false;
		}
	} else {
		// only allow mapping from render thread, for now
	}
}

void Mesh_gl::flush() {
	if (activeThread == getThreadId()) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mappedSize, mapPtr, GL_STATIC_DRAW);
	} else {
		// only allow mapping from render thread, for now
	}
}