#include "AssetManager.hpp"

#include <RenderEngine/IRenderEngine.hpp>

#include <AssetLib/AssetLib.hpp>

#include <fstream>
#include <vector>
#include <memory>

#include "CoreGlobals.hpp"

// @temporary

struct Header {
	char tag[4];
	uint16_t major;
	uint16_t minor;
};

struct BoolFlags {
	uint32_t meshCount;
	bool useVNormals;
	bool useVColors;
	bool useVUV;
	bool padding;
	uint32_t vertCount;
	uint32_t triangleCount;
};

struct Triangle {
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;
};

struct Vertex5 {
	glm::vec3 pos;
	glm::vec2 uv;
};

struct MemoryBuffer {

	void deleteBuffer();

	void setData(void* data, uint32_t dataSize);
	void setOffset(uint32_t offset);

	void* returnBytes(uint32_t bytesToReturn);

private:

	uint32_t offset;
	uint32_t totalDataSize;
	void* memoryData;

};

void MemoryBuffer::deleteBuffer() {
	if (memoryData)
		free(memoryData);
	memoryData = nullptr;
	offset = totalDataSize = 0;
}

void MemoryBuffer::setData(void * data, uint32_t dataSize) {
	if (dataSize == 0)
		throw "Data size cannot be 0\n";
	if (data == nullptr)
		throw "Data cannot be null\n";

	memoryData = malloc(dataSize);
	if (memoryData != NULL) {
		memcpy(memoryData, data, dataSize);
	}

	totalDataSize = dataSize;
	offset = 0;
}

void MemoryBuffer::setOffset(uint32_t _offset) {
	if (_offset >= totalDataSize)
		throw "Cannot set offset to end of buffer\n";
	offset = _offset;
}

void * MemoryBuffer::returnBytes(uint32_t bytesToReturn) {
	if (bytesToReturn > (totalDataSize - offset)) throw "Trying to read more than avaible in the buffer\n";

	char* data = (char*)memoryData + offset;
	offset += bytesToReturn;

	return data;
}

void* createVertUVData(void* meshData, uint32_t &size) {

	MemoryBuffer memBuff;
	memBuff.setData(meshData, size);

	Header* h = (Header*)memBuff.returnBytes(sizeof(Header));

	int* tag = (int*)h->tag;

	int* meshTag = (int*)"MESH";

	if (*tag != *meshTag) {
		printf("Invalid mesh file\n");
		size = 0;
		return nullptr;
	}

	printf("Mesh version: %d.%d\n", h->major, h->minor);
	if (h->major == 1 && h->minor == 0) {
		BoolFlags* bf = (BoolFlags*)memBuff.returnBytes(sizeof(BoolFlags));

		glm::vec3* vertices = (glm::vec3*)memBuff.returnBytes(sizeof(glm::vec3) * bf->vertCount);
		glm::vec3* normals = bf->useVNormals ? (glm::vec3*)memBuff.returnBytes(sizeof(glm::vec3) * bf->vertCount) : nullptr;
		glm::vec4* colors = bf->useVColors ? (glm::vec4*)memBuff.returnBytes(sizeof(glm::vec4) * bf->vertCount) : nullptr;
		glm::vec2* uv = bf->useVUV ? (glm::vec2*)memBuff.returnBytes(sizeof(glm::vec2) * bf->vertCount) : nullptr;

		Triangle* triangles = (Triangle*)memBuff.returnBytes(sizeof(Triangle) * bf->triangleCount);

		std::vector<Vertex5> verts;

		for (size_t i = 0; i < bf->triangleCount; i++) {
			Triangle t = triangles[i];

			Vertex5 v1 = { vertices[t.v1], glm::vec2(0.0f, 0.0f) };
			Vertex5 v2 = { vertices[t.v2], glm::vec2(0.0f, 0.0f) };
			Vertex5 v3 = { vertices[t.v3], glm::vec2(0.0f, 0.0f) };

			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);
		}

		memBuff.deleteBuffer();

		size = (uint32_t)verts.size() * sizeof(Vertex5);

		Vertex5* v = new Vertex5[verts.size()];
		memcpy(v, verts.data(), size);

		return v;
	} else if (h->major == 1 && h->minor == 1) {
		uint32_t* nrObj = (uint32_t*)memBuff.returnBytes(sizeof(uint32_t));
		uint32_t* nrBones = (uint32_t*)memBuff.returnBytes(sizeof(uint32_t));
		printf("nrBones: %d\n", (int)*nrBones);

		glm::vec3* points = (glm::vec3*)memBuff.returnBytes(sizeof(glm::vec3) * (*nrBones));

		std::vector<Vertex5> verts;

		for (size_t i = 0; i < (*nrBones); i++) {

			points[i] *= 1.0f;

			printf("b (%f, %f, %f)\n", points[i].x, points[i].y, points[i].z);

			glm::vec3 vo1(-0.01f, -0.01f,  0.01f);
			glm::vec3 vo2(-0.01f,  0.01f,  0.01f);
			glm::vec3 vo3(-0.01f, -0.01f, -0.01f);
			glm::vec3 vo4(-0.01f,  0.01f, -0.01f);
			glm::vec3 vo5( 0.01f, -0.01f,  0.01f);
			glm::vec3 vo6( 0.01f,  0.01f,  0.01f);
			glm::vec3 vo7( 0.01f, -0.01f, -0.01f);
			glm::vec3 vo8( 0.01f,  0.01f, -0.01f);

			glm::vec3 p = points[i];

			Vertex5 v1 = { p + vo1, glm::vec2(0.0f, 0.0f) };
			Vertex5 v2 = { p + vo2, glm::vec2(0.0f, 0.0f) };
			Vertex5 v3 = { p + vo3, glm::vec2(0.0f, 0.0f) };
			Vertex5 v4 = { p + vo4, glm::vec2(0.0f, 0.0f) };
			Vertex5 v5 = { p + vo5, glm::vec2(0.0f, 0.0f) };
			Vertex5 v6 = { p + vo6, glm::vec2(0.0f, 0.0f) };
			Vertex5 v7 = { p + vo7, glm::vec2(0.0f, 0.0f) };
			Vertex5 v8 = { p + vo8, glm::vec2(0.0f, 0.0f) };

			verts.push_back( v2 ); verts.push_back( v3 ); verts.push_back( v1 ); 				 	  
			verts.push_back( v4 ); verts.push_back( v7 ); verts.push_back( v3 );
							 	 				   		 				 	  
			verts.push_back( v8 ); verts.push_back( v5 ); verts.push_back( v7 );
			verts.push_back( v6 ); verts.push_back( v1 ); verts.push_back( v5 );
							 	 				   		 				 	  
			verts.push_back( v7 ); verts.push_back( v1 ); verts.push_back( v3 );
			verts.push_back( v4 ); verts.push_back( v6 ); verts.push_back( v8 );
							 	 				   		 				 	  
			verts.push_back( v2 ); verts.push_back( v4 ); verts.push_back( v3 );
			verts.push_back( v4 ); verts.push_back( v8 ); verts.push_back( v7 );
							 	 				   		 				 	  
			verts.push_back( v8 ); verts.push_back( v6 ); verts.push_back( v5 );
			verts.push_back( v6 ); verts.push_back( v2 ); verts.push_back( v1 );
							 	 				   		 				 	  
			verts.push_back( v7 ); verts.push_back( v5 ); verts.push_back( v1 );
			verts.push_back( v4 ); verts.push_back( v2 ); verts.push_back( v6 );

		}

		memBuff.deleteBuffer();
		size = (uint32_t)verts.size() * sizeof(Vertex5);

		Vertex5* v = new Vertex5[verts.size()];
		memcpy(v, verts.data(), size);

		return v;
	}
	return nullptr;
}


// @end temporary 



void AssetManager::init() {
	gAssetManager = this;
	renderEngine = gRenderEngine;
	nrMeshesLoaded = 0;
	startupSceneRefId = 0;

	currentFile = new GameFile();

	currentFile->init();
	currentFile->load("data/foo.bin");

	startupSceneRefId = currentFile->getHeaderDataV2().startupSceneRef;

	Entry* e = currentFile->loadEntry(0);

	if (e) {
		printf("%s\n", (char*)e->data);
	}

	e = currentFile->loadEntry(1);

	if (e) {
		printf("%s\n", (char*)e->data);
	}

	e = currentFile->loadEntry(2);

	if (e) {
		printf("%s\n", (char*)e->data);
	}

}

void AssetManager::freeResources() {
	std::map<uint32_t, IMesh*>::iterator it = loadedMeshes.begin();
	std::map<uint32_t, IMesh*>::iterator eit = loadedMeshes.end();

	for (it; it != eit; it++) {
		if (it->second != nullptr) {
			it->second->release();
		}
	}

	delete currentFile;

}

uint32_t AssetManager::loadMesh(char * meshName) {

	uint32_t id = nrMeshesLoaded;

	uint32_t dataSize = 0;
	void * data = AssetLib::fileToMemory(meshName, dataSize);

	if (data) {

		void* meshData = createVertUVData(data, dataSize);

		IMesh* mesh = renderEngine->createMesh();

		mesh->init(MeshPrimitiveType::TRIANGLE);
		mesh->setMeshData(meshData, dataSize, MeshDataLayout::VERT_UV);

		nrMeshesLoaded++;
		delete meshData;
		delete data;

		loadedMeshes[id] = mesh;
	}

	return id;
}

IMesh * AssetManager::getMeshFromInstanceId(uint32_t instanceId) {
	if (loadedMeshes.count(instanceId)) {
		return loadedMeshes.at(instanceId);
	}
	return nullptr;
}

uint32_t AssetManager::getNextFormID() {

	return currentFile->getNextFormID();
}

void AssetManager::createNewEntry(uint32_t formID, const char* tag) {
	currentFile->createNewEntry(formID, tag);

}

Entry* AssetManager::getEntry(uint32_t formID) {
	return currentFile->loadEntry(formID);
}

void AssetManager::updateEntry(Entry* entry) {
	currentFile->updateEntry(entry);
}

void AssetManager::setStartupSceneRef(uint32_t ref) {
	startupSceneRefId = ref;
}


uint32_t AssetManager::getStartupSceneRef() {
	return 0;
}
