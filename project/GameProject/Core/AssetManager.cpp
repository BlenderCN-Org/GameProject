#include "AssetManager.hpp"

#include <RenderEngine/IRenderEngine.hpp>

#include <AssetLib/AssetLib.hpp>

#include <fstream>
#include <vector>
#include <memory>

#include "CoreGlobals.hpp"
#include "System\Console.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

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

struct Bone {
	int32_t id;
	int32_t parent;
	glm::vec3 headPos;
	glm::vec3 tailPos;
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

void makeCube(std::vector<Vertex5> &verts, glm::vec3 pos) 	{

	glm::vec3 vo1(-0.01f, -0.01f,  0.01f);
	glm::vec3 vo2(-0.01f,  0.01f,  0.01f);
	glm::vec3 vo3(-0.01f, -0.01f, -0.01f);
	glm::vec3 vo4(-0.01f,  0.01f, -0.01f);
	glm::vec3 vo5( 0.01f, -0.01f,  0.01f);
	glm::vec3 vo6( 0.01f,  0.01f,  0.01f);
	glm::vec3 vo7( 0.01f, -0.01f, -0.01f);
	glm::vec3 vo8( 0.01f,  0.01f, -0.01f);

	glm::vec3 p = pos;

	Vertex5 v1 = { p + vo1, glm::vec2(0.0f, 0.0f) };
	Vertex5 v2 = { p + vo2, glm::vec2(0.0f, 0.0f) };
	Vertex5 v3 = { p + vo3, glm::vec2(0.0f, 0.0f) };
	Vertex5 v4 = { p + vo4, glm::vec2(0.0f, 0.0f) };
	Vertex5 v5 = { p + vo5, glm::vec2(0.0f, 0.0f) };
	Vertex5 v6 = { p + vo6, glm::vec2(0.0f, 0.0f) };
	Vertex5 v7 = { p + vo7, glm::vec2(0.0f, 0.0f) };
	Vertex5 v8 = { p + vo8, glm::vec2(0.0f, 0.0f) };

	verts.push_back(v2); verts.push_back(v3); verts.push_back(v1);
	verts.push_back(v4); verts.push_back(v7); verts.push_back(v3);

	verts.push_back(v8); verts.push_back(v5); verts.push_back(v7);
	verts.push_back(v6); verts.push_back(v1); verts.push_back(v5);

	verts.push_back(v7); verts.push_back(v1); verts.push_back(v3);
	verts.push_back(v4); verts.push_back(v6); verts.push_back(v8);

	verts.push_back(v2); verts.push_back(v4); verts.push_back(v3);
	verts.push_back(v4); verts.push_back(v8); verts.push_back(v7);

	verts.push_back(v8); verts.push_back(v6); verts.push_back(v5);
	verts.push_back(v6); verts.push_back(v2); verts.push_back(v1);

	verts.push_back(v7); verts.push_back(v5); verts.push_back(v1);
	verts.push_back(v4); verts.push_back(v2); verts.push_back(v6);
}

void makeLine(std::vector<Vertex5> &verts, glm::vec3 head, glm::vec3 tail) {

	glm::vec3 vo1(-0.005f, -0.005f,  0.005f);
	glm::vec3 vo2(-0.005f,  0.005f,  0.005f);
	glm::vec3 vo3(-0.005f, -0.005f, -0.005f);
	glm::vec3 vo4(-0.005f,  0.005f, -0.005f);
	glm::vec3 vo5( 0.005f, -0.005f,  0.005f);
	glm::vec3 vo6( 0.005f,  0.005f,  0.005f);
	glm::vec3 vo7( 0.005f, -0.005f, -0.005f);
	glm::vec3 vo8( 0.005f,  0.005f, -0.005f);

	glm::vec3 p = head;
	glm::vec3 p2 = tail;

	glm::vec3 ori(0);
	glm::vec3 dir = glm::vec3(1, 0, 0) * glm::distance(p, p2);

	Vertex5 v1 = { ori + vo1, glm::vec2(0.0f, 0.0f) };
	Vertex5 v2 = { ori + vo2, glm::vec2(0.0f, 0.0f) };
	Vertex5 v3 = { ori + vo3, glm::vec2(0.0f, 0.0f) };
	Vertex5 v4 = { ori + vo4, glm::vec2(0.0f, 0.0f) };
	Vertex5 v5 = { dir + vo5, glm::vec2(0.0f, 0.0f) };
	Vertex5 v6 = { dir + vo6, glm::vec2(0.0f, 0.0f) };
	Vertex5 v7 = { dir + vo7, glm::vec2(0.0f, 0.0f) };
	Vertex5 v8 = { dir + vo8, glm::vec2(0.0f, 0.0f) };

	// rotate vertics

	glm::vec3 a = glm::normalize(p2 - p);
	glm::vec3 b = glm::vec3(1, 0, 0);

	float angle = glm::angle(a, b);

	glm::vec3 normal = -glm::normalize(cross(p2 - p, b));

	v1.pos = glm::rotate(v1.pos, angle, normal);
	v2.pos = glm::rotate(v2.pos, angle, normal);
	v3.pos = glm::rotate(v3.pos, angle, normal);
	v4.pos = glm::rotate(v4.pos, angle, normal);
	v5.pos = glm::rotate(v5.pos, angle, normal);
	v6.pos = glm::rotate(v6.pos, angle, normal);
	v7.pos = glm::rotate(v7.pos, angle, normal);
	v8.pos = glm::rotate(v8.pos, angle, normal);

	// translate vertices
	v1.pos += p;
	v2.pos += p;
	v3.pos += p;
	v4.pos += p;
	v5.pos += p;
	v6.pos += p;
	v7.pos += p;
	v8.pos += p;
	
	verts.push_back(v2); verts.push_back(v3); verts.push_back(v1);
	verts.push_back(v4); verts.push_back(v7); verts.push_back(v3);

	verts.push_back(v8); verts.push_back(v5); verts.push_back(v7);
	verts.push_back(v6); verts.push_back(v1); verts.push_back(v5);

	verts.push_back(v7); verts.push_back(v1); verts.push_back(v3);
	verts.push_back(v4); verts.push_back(v6); verts.push_back(v8);

	verts.push_back(v2); verts.push_back(v4); verts.push_back(v3);
	verts.push_back(v4); verts.push_back(v8); verts.push_back(v7);

	verts.push_back(v8); verts.push_back(v6); verts.push_back(v5);
	verts.push_back(v6); verts.push_back(v2); verts.push_back(v1);

	verts.push_back(v7); verts.push_back(v5); verts.push_back(v1);
	verts.push_back(v4); verts.push_back(v2); verts.push_back(v6);
}

void* createVertUVData(void* meshData, uint32_t &size) {

	MemoryBuffer memBuff;
	memBuff.setData(meshData, size);

	Header* h = (Header*)memBuff.returnBytes(sizeof(Header));

	int* tag = (int*)h->tag;

	int* meshTag = (int*)"MESH";

	if (*tag != *meshTag) {
		gConsole->print("Invalid mesh file\n");
		size = 0;
		return nullptr;
	}

	gConsole->print("Mesh version: %d.%d\n", h->major, h->minor);
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

		int triCount = (int)bf->triangleCount;

		memBuff.deleteBuffer();

		size = (uint32_t)verts.size() * sizeof(Vertex5);

		Vertex5* v = new Vertex5[verts.size()];
		memcpy(v, verts.data(), size);

		gConsole->print("NrVerts: %d\nNrTris: %d\n", (int)verts.size(), triCount);

		return v;
	} else if (h->major == 1 && h->minor == 1) {
		uint32_t* nrObj = (uint32_t*)memBuff.returnBytes(sizeof(uint32_t));
		uint32_t* nrBones = (uint32_t*)memBuff.returnBytes(sizeof(uint32_t));
		gConsole->print("nrBones: %d\n", (int)*nrBones);

		Bone* bones = (Bone*)memBuff.returnBytes(sizeof(Bone) * (*nrBones));

		std::vector<Vertex5> verts;

		for (size_t i = 0; i < (*nrBones); i++) {

			bones[i].headPos *= 1.0f;
			bones[i].tailPos*= 1.0f;

			//printf("b (%f, %f, %f)\n", bones[i].headPos.x, bones[i].headPos.y, bones[i].headPos.z);

			//makeCube(verts, bones[i].headPos);
			//makeCube(verts, bones[i].tailPos);

			if (bones[i].id == 0 || bones[i].parent == 0) {
				makeCube(verts, bones[i].headPos);
			}
			makeCube(verts, bones[i].tailPos);
			makeLine(verts, bones[i].headPos, bones[i].tailPos);

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
		gConsole->print("%s\n", (char*)e->data);
	}

	e = currentFile->loadEntry(1);

	if (e) {
		gConsole->print("%s\n", (char*)e->data);
	}

	e = currentFile->loadEntry(2);

	if (e) {
		gConsole->print("%s\n", (char*)e->data);
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
