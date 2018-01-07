#ifndef COLLADA_LOADER_HPP
#define COLLADA_LOADER_HPP

#include <TinyXml/tinyxml2.h>

#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

enum class StrideList {
	INVALID,
	Vertex,
	Normal,
	Uv,
	Color,
};

class ColladaLoader {

public:

	bool load(const char* fileName);

	uint32_t numGeomentries();
	const char* geometryId(uint32_t index);

	float* getVertices(const char* id, uint32_t &count);
	float* getNormals(const char* id, uint32_t &count);
	float* getUvs(const char* id, uint32_t &count);
	float* getColors(const char* id, uint32_t &count);
	float* getWeights(const char* id, uint32_t &count);
	int* getWIndex(const char* id, uint32_t &count);

	int* triangles(const char* id, uint32_t &count, uint32_t &stride, StrideList** strideList);

	uint32_t numBones(const char* id);

	float* getBoneTransform(const char* id, const char* boneName);
	int numBoneChildren(const char* id, const char* boneName);
	
	const char* getRootBoneName(const char* id);
	const char* getChildName(const char* id, const char* rootName, uint32_t childIndex);
	uint32_t getBoneIndex(const char* id, const char* boneName);
	
private:

	tinyxml2::XMLNode* geometryByName(const char* name);
	tinyxml2::XMLNode* findControllerBySkinName(const char* name);
	tinyxml2::XMLNode* skinByName(tinyxml2::XMLNode* controller, const char* name);
	tinyxml2::XMLElement* findNodeByElemNameAndIdAttrib(tinyxml2::XMLElement* node, const char* elementName, const char* name);

	tinyxml2::XMLElement* findBone(tinyxml2::XMLElement* skeletonRoot, const char* boneName);

	template<typename T>
	T* toArray(const char* data, size_t dataSize) {

		size_t size = dataSize;

		T* values = new T[size];

		std::string str(data);

		// If possible, always prefer std::vector to naked array
		std::vector<T> v;

		// Build an istream that holds the input string
		std::istringstream iss(str);

		// Iterate over the istream, using >> to grab floats
		// and push_back to store them in the vector
		std::copy(std::istream_iterator<T>(iss),
			std::istream_iterator<T>(),
			std::back_inserter(v));

		memcpy(values, v.data(), size * sizeof(T));

		return values;
	};

	tinyxml2::XMLDocument colladaFile;
	tinyxml2::XMLNode* rootNode;

	tinyxml2::XMLElement* libGeo;
	tinyxml2::XMLElement* libAni;
	tinyxml2::XMLElement* libCon;
	tinyxml2::XMLElement* libVis;

};

#endif