#include "ColladaLoader.hpp"

#include <string>
#include <math.h>

int sdahjkahsdkjahsdjkahsdjkahdsjaksdhjkasdhjkwkjshkkajshd_unused() { return 0; };

#if 0

bool ColladaLoader::load(const char* file) {

	colladaFile.LoadFile(file);
	rootNode = colladaFile.FirstChildElement("COLLADA");

	if (rootNode) {
		libGeo = rootNode->FirstChildElement("library_geometries");
		libAni = rootNode->FirstChildElement("library_animations");
		libCon = rootNode->FirstChildElement("library_controllers");
		libVis = rootNode->FirstChildElement("library_visual_scenes");

		return true;
	}

	return false;
}


uint32_t ColladaLoader::numGeomentries() {

	uint32_t count = 0U;

	if (libGeo) {
		tinyxml2::XMLNode* geometry = libGeo->FirstChildElement("geometry");

		while (geometry != nullptr) {
			count++;
			geometry = geometry->NextSibling();
		}
	}

	return count;
}

const char* ColladaLoader::geometryId(uint32_t index) {

	const char* idString = nullptr;

	uint32_t count = 0U;
	uint32_t maxCount = numGeomentries();

	if (maxCount > index) {
		if (libGeo) {
			tinyxml2::XMLNode* geometry = libGeo->FirstChildElement("geometry");

			while (geometry != nullptr) {

				if (count == index) {
					idString = geometry->ToElement()->Attribute("id");
					break;
				}

				count++;
				geometry = geometry->NextSibling();

			}
		}
	}

	return idString;
}

float* ColladaLoader::getVertices(const char* id, uint32_t &count) {

	float* data = nullptr;

	tinyxml2::XMLNode* geoNode = geometryByName(id);

	if (geoNode) {

		tinyxml2::XMLElement* meshElement = geoNode->FirstChildElement("mesh");

		if (meshElement) {

			std::string positionString(id);
			positionString += "-positions";

			tinyxml2::XMLElement* positions = findNodeByElemNameAndIdAttrib(meshElement, "source", positionString.c_str());

			if (positions) {

				positions = positions->FirstChildElement("float_array");
				count = positions->UnsignedAttribute("count");

				const char* text = positions->GetText();

				data = toArray<float>(text, count);
			}
		}
	}

	return data;
}

float* ColladaLoader::getNormals(const char* id, uint32_t &count) {
	float* data = nullptr;

	tinyxml2::XMLNode* geoNode = geometryByName(id);

	if (geoNode) {

		tinyxml2::XMLElement* meshElement = geoNode->FirstChildElement("mesh");

		if (meshElement) {

			std::string positionString(id);
			positionString += "-normals";

			tinyxml2::XMLElement* positions = findNodeByElemNameAndIdAttrib(meshElement, "source", positionString.c_str());

			if (positions) {

				positions = positions->FirstChildElement("float_array");
				count = positions->UnsignedAttribute("count");

				const char* text = positions->GetText();

				data = toArray<float>(text, count);
			}
		}
	}

	return data;
}

float* ColladaLoader::getUvs(const char* id, uint32_t &count) {
	return nullptr;
}

float* ColladaLoader::getColors(const char* id, uint32_t &count) {
	return nullptr;
}

struct JointWeight {
	JointWeight() : joint(-1), weight(-1.0F) {}

	int joint;
	float weight;
};

struct VertexWeights {

	VertexWeights() : numW(0) {}

	JointWeight weights[4];

	int numW;

	int lowestWIndex() {
		float minVal = 1.0F;
		int index = -1;

		for (int i = 0; i < 4; i++) {

			float old = minVal;
			minVal = std::min(minVal, weights[i].weight);

			if (minVal != old) {
				index = i;
			}
		}

		return index;
	}

	void addWeight(JointWeight w) {

		if (numW != 4) {
			weights[numW] = w;
			numW++;
		} else {

			int idx = lowestWIndex();
			weights[idx];

		}
	}

	void finalize() {

		if (numW != 4) {

			for (size_t i = 0; i < 4; i++) {
				if (weights[i].joint == -1) {
					weights[i].joint = 0;
					weights[i].weight = 0.0F;
				}
			}

		}

		float sum = 0.0F;

		for (size_t i = 0; i < 4; i++) {
			sum += weights[i].weight;
		}

		for (size_t i = 0; i < 4; i++) {
			weights[i].weight /= sum;
		}

	}

};

float* ColladaLoader::getWeights(const char* id, uint32_t &count) {
	float* data = nullptr;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerId = controller->ToElement()->Attribute("id");

		tinyxml2::XMLNode* skin = skinByName(controller, id);

		if (skin) {
			//std::string jointString(controllerId);
			//jointString += "-joints";

			std::string weightString(controllerId);
			weightString += "-weights";

			tinyxml2::XMLElement* weights = findNodeByElemNameAndIdAttrib(skin->ToElement(), "source", weightString.c_str());

			float* rawWeights = nullptr;

			if (weights) {

				weights = weights->FirstChildElement("float_array");
				uint32_t wcount = weights->UnsignedAttribute("count");

				const char* text = weights->GetText();

				rawWeights = toArray<float>(text, wcount);
			}

			tinyxml2::XMLElement* vWeights = skin->FirstChildElement("vertex_weights");

			if (rawWeights && vWeights) {

				uint32_t wcount = vWeights->UnsignedAttribute("count");
				tinyxml2::XMLElement* vCount = vWeights->FirstChildElement("vcount");
				int* vCountData = toArray<int>(vCount->GetText(), wcount);
				int sum = 0;

				for (size_t i = 0; i < wcount; i++) {
					sum += vCountData[i];
				}
				count = sum;
				sum *= 2;

				tinyxml2::XMLElement* v = vWeights->FirstChildElement("v");
				int* vData = toArray<int>(v->GetText(), sum);

				// 4 weights per vertex

				VertexWeights* weights = new VertexWeights[wcount];

				count = wcount * 4;
				data = new float[count];

				int largeArrayOffset = 0;

				for (size_t i = 0; i < wcount; i++) {

					VertexWeights curWeight;

					int numValues = vCountData[i];

					for (int j = 0; j < numValues; j++) {

						JointWeight jw;

						int jointIndex = largeArrayOffset + (j * 2);
						int weightIndex = jointIndex + 1;

						jw.joint = vData[jointIndex];
						jw.weight = rawWeights[vData[weightIndex]];

						curWeight.addWeight(jw);

					}

					curWeight.finalize();

					weights[i] = curWeight;

					largeArrayOffset += (numValues * 2);
				}

				for (size_t i = 0; i < wcount; i++) {

					size_t offset = i * 4;

					data[offset + 0] = weights[i].weights[0].weight;
					data[offset + 1] = weights[i].weights[1].weight;
					data[offset + 2] = weights[i].weights[2].weight;
					data[offset + 3] = weights[i].weights[3].weight;

				}

				delete[] vCountData;
				delete[] vData;
				delete[] weights;
			}

			delete[] rawWeights;
		}
	}

	return data;
}

int* ColladaLoader::getWIndex(const char* id, uint32_t &count) {
	int* data = nullptr;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerId = controller->ToElement()->Attribute("id");

		tinyxml2::XMLNode* skin = skinByName(controller, id);

		if (skin) {
			//std::string jointString(controllerId);
			//jointString += "-joints";

			std::string weightString(controllerId);
			weightString += "-weights";

			tinyxml2::XMLElement* weights = findNodeByElemNameAndIdAttrib(skin->ToElement(), "source", weightString.c_str());

			float* rawWeights = nullptr;

			if (weights) {

				weights = weights->FirstChildElement("float_array");
				uint32_t wcount = weights->UnsignedAttribute("count");

				const char* text = weights->GetText();

				rawWeights = toArray<float>(text, wcount);
			}

			tinyxml2::XMLElement* vWeights = skin->FirstChildElement("vertex_weights");

			if (rawWeights && vWeights) {

				uint32_t wcount = vWeights->UnsignedAttribute("count");
				tinyxml2::XMLElement* vCount = vWeights->FirstChildElement("vcount");
				int* vCountData = toArray<int>(vCount->GetText(), wcount);
				int sum = 0;

				for (size_t i = 0; i < wcount; i++) {
					sum += vCountData[i];
				}
				sum *= 2;

				tinyxml2::XMLElement* v = vWeights->FirstChildElement("v");
				int* vData = toArray<int>(v->GetText(), sum);

				// 4 weights per vertex

				VertexWeights* weights = new VertexWeights[wcount];

				count = wcount * 4;
				data = new int[count];

				int largeArrayOffset = 0;

				for (size_t i = 0; i < wcount; i++) {

					VertexWeights curWeight;

					int numValues = vCountData[i];

					for (int j = 0; j < numValues; j++) {

						JointWeight jw;

						int jointIndex = largeArrayOffset + (j * 2);
						int weightIndex = jointIndex + 1;

						jw.joint = vData[jointIndex];
						jw.weight = rawWeights[vData[weightIndex]];

						curWeight.addWeight(jw);

					}

					curWeight.finalize();

					weights[i] = curWeight;

					largeArrayOffset += (numValues * 2);
				}

				for (size_t i = 0; i < wcount; i++) {

					size_t offset = i * 4;

					data[offset + 0] = weights[i].weights[0].joint;
					data[offset + 1] = weights[i].weights[1].joint;
					data[offset + 2] = weights[i].weights[2].joint;
					data[offset + 3] = weights[i].weights[3].joint;

				}

				delete[] vCountData;
				delete[] vData;
				delete[] weights;
			}

			delete[] rawWeights;
		}
	}

	return data;
}

uint32_t numInputHelper(tinyxml2::XMLElement* triangles, const char* name) {

	uint32_t count = 0U;

	tinyxml2::XMLElement* input = triangles->FirstChildElement(name);
	while (input != nullptr) {
		count++;
		input = input->NextSiblingElement(name);
	}

	return count;
}

StrideList semanticToStride(const char* semantic) {

	StrideList s = StrideList::Vertex;

	std::string str(semantic);

	if (str.compare("VERTEX") == 0) {
		s = StrideList::Vertex;
	} else if (str.compare("NORMAL") == 0) {
		s = StrideList::Normal;
	}

	return s;
}

int* ColladaLoader::triangles(const char* id, uint32_t &count, uint32_t &stride, StrideList** strideList) {

	int* data = nullptr;

	tinyxml2::XMLNode* geoNode = geometryByName(id);

	if (geoNode) {

		tinyxml2::XMLElement* meshElement = geoNode->FirstChildElement("mesh");

		if (meshElement) {

			tinyxml2::XMLElement* tris = meshElement->FirstChildElement("triangles");

			if (tris) {
				count = tris->UnsignedAttribute("count");
				stride = numInputHelper(tris, "input");

				*strideList = new StrideList[stride];
				StrideList* p = *strideList;

				tinyxml2::XMLElement* input = tris->FirstChildElement("input");
				while (input != nullptr) {
					int offset = input->UnsignedAttribute("offset");
					const char* semantic = input->Attribute("semantic");

					p[offset] = semanticToStride(semantic);

					input = input->NextSiblingElement("input");
				}

				const char* text = tris->FirstChildElement("p")->GetText();
				data = toArray<int>(text, count * (stride * 3));

			}
		}
	}

	return data;
}

uint32_t ColladaLoader::numBones(const char* id) {

	uint32_t count = 0U;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerId = controller->ToElement()->Attribute("id");

		tinyxml2::XMLNode* skin = skinByName(controller, id);

		if (skin) {
			std::string jointString(controllerId);
			jointString += "-joints";

			tinyxml2::XMLElement* joints = findNodeByElemNameAndIdAttrib(skin->ToElement(), "source", jointString.c_str());

			if (joints) {
				tinyxml2::XMLElement* nameArray = joints->FirstChildElement("Name_array");

				if (nameArray) {
					count = nameArray->UnsignedAttribute("count");
				}
			}
		}
	}

	return count;
}

float* ColladaLoader::getBoneTransform(const char* id, const char* boneName) {

	float* data = nullptr;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerName = controller->ToElement()->Attribute("name");

		if (libVis && controllerName) {

			tinyxml2::XMLElement* skeleton = libVis->FirstChildElement("visual_scene")->FirstChildElement("node");

			const char* nodeId = skeleton->Attribute("id");

			std::string id(nodeId);

			if (id.compare(controllerName) == 0) {

				tinyxml2::XMLElement* bone = findBone(skeleton, boneName);

				if (bone) {

					const char* text = bone->FirstChildElement("matrix")->GetText();

					data = toArray<float>(text, 16);
				}

			}

		}

	}

	return data;
}

int ColladaLoader::numBoneChildren(const char* id, const char* boneName) {

	int numChildren = 0;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerName = controller->ToElement()->Attribute("name");

		if (libVis && controllerName) {

			tinyxml2::XMLElement* skeleton = libVis->FirstChildElement("visual_scene")->FirstChildElement("node");

			const char* nodeId = skeleton->Attribute("id");

			std::string id(nodeId);

			if (id.compare(controllerName) == 0) {

				tinyxml2::XMLElement* bone = findBone(skeleton, boneName);

				tinyxml2::XMLElement* childs = bone->FirstChildElement("node");

				while (childs != nullptr) {
					numChildren++;
					childs = childs->NextSiblingElement("node");
				}
			}
		}
	}

	return numChildren;
}

const char* ColladaLoader::getRootBoneName(const char* id) {

	const char* rootBoneName = nullptr;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerName = controller->ToElement()->Attribute("name");

		if (libVis && controllerName) {

			tinyxml2::XMLElement* skeleton = libVis->FirstChildElement("visual_scene")->FirstChildElement("node");

			const char* nodeId = skeleton->Attribute("id");

			std::string id(nodeId);

			if (id.compare(controllerName) == 0) {

				tinyxml2::XMLElement* rootBone = skeleton->FirstChildElement("node");

				if (rootBone) {
					rootBoneName = rootBone->Attribute("sid");
				}
			}
		}
	}

	return rootBoneName;
}

const char* ColladaLoader::getChildName(const char* id, const char* rootName, uint32_t childIndex) {

	const char* boneName = nullptr;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerName = controller->ToElement()->Attribute("name");

		if (libVis && controllerName) {

			tinyxml2::XMLElement* skeleton = libVis->FirstChildElement("visual_scene")->FirstChildElement("node");

			const char* nodeId = skeleton->Attribute("id");

			std::string id(nodeId);

			if (id.compare(controllerName) == 0) {

				tinyxml2::XMLElement* bone = findBone(skeleton, rootName);

				tinyxml2::XMLElement* childs = bone->FirstChildElement("node");

				uint32_t currentIndex = 0;

				while (childs != nullptr) {

					if (currentIndex == childIndex) {

						boneName = childs->Attribute("sid");

						break;
					}
					childs = childs->NextSiblingElement("node");
				}
			}
		}
	}

	return boneName;
}

uint32_t indexOfString(const char* text, const char* name) {

	uint32_t index = 0U;

	std::string s = "";

	uint32_t c = 0U;

	while (text[c] != '\0') {

		if (text[c] == ' ') {

			if (s.compare(name) == 0) {
				break;
			}

			index++;
			s = "";

		} else {
			s += text[c];
		}

		c++;
	}

	return index;
}

uint32_t ColladaLoader::getBoneIndex(const char* id, const char* boneName) {

	uint32_t index = 0u;;

	tinyxml2::XMLNode* controller = findControllerBySkinName(id);

	if (controller) {

		const char* controllerId = controller->ToElement()->Attribute("id");

		tinyxml2::XMLNode* skin = skinByName(controller, id);

		if (skin) {
			std::string jointString(controllerId);
			jointString += "-joints";

			tinyxml2::XMLElement* joints = findNodeByElemNameAndIdAttrib(skin->ToElement(), "source", jointString.c_str());

			if (joints) {
				tinyxml2::XMLElement* nameArray = joints->FirstChildElement("Name_array");

				const char* text = nameArray->GetText();

				index = indexOfString(text, boneName);
			}
		}
	}

	return index;

}


/******************************************
*										  *
*        Private functions below		  *
*										  *
*******************************************/

tinyxml2::XMLNode* ColladaLoader::geometryByName(const char* name) {

	tinyxml2::XMLNode* node = nullptr;

	if (libGeo) {
		tinyxml2::XMLNode* geometry = libGeo->FirstChildElement("geometry");

		while (geometry != nullptr) {

			const char* attribId = geometry->ToElement()->Attribute("id");

			std::string str(name);

			if (str.compare(attribId) == 0) {
				node = geometry;
				break;
			}
			geometry = geometry->NextSibling();
		}
	}

	return node;
}

tinyxml2::XMLNode* ColladaLoader::findControllerBySkinName(const char* name) {

	tinyxml2::XMLNode* node = nullptr;
	bool found = false;

	if (libCon) {
		tinyxml2::XMLNode* controller = libCon->FirstChildElement("controller");

		while (controller != nullptr) {

			tinyxml2::XMLNode* skin = controller->FirstChildElement("skin");

			while (skin != nullptr) {

				const char* attribId = skin->ToElement()->Attribute("source");

				std::string str("#");
				str += name;

				if (str.compare(attribId) == 0) {
					node = controller;
					found = true;
					break;
				}
				skin = skin->NextSiblingElement("skin");
			}

			if (found == true) {
				break;
			} else {
				controller = controller->NextSiblingElement("controller");
			}
		}
	}

	return node;
}

tinyxml2::XMLNode* ColladaLoader::skinByName(tinyxml2::XMLNode* controller, const char* name) {

	tinyxml2::XMLNode* node = nullptr;

	tinyxml2::XMLNode* skin = controller->FirstChildElement("skin");

	while (skin != nullptr) {

		const char* attribId = skin->ToElement()->Attribute("source");

		std::string str("#");
		str += name;

		if (str.compare(attribId) == 0) {
			node = skin;
			break;
		}
		skin = skin->NextSiblingElement("skin");
	}

	return node;
}

tinyxml2::XMLElement* ColladaLoader::findNodeByElemNameAndIdAttrib(tinyxml2::XMLElement* node, const char* elementName, const char* name) {

	tinyxml2::XMLElement* found = nullptr;

	tinyxml2::XMLElement* search = node->FirstChildElement(elementName);

	std::string str(name);

	while (elementName != nullptr) {

		const char* attribId = search->ToElement()->Attribute("id");

		if (str.compare(attribId) == 0) {
			found = search;
			break;
		}
		search = search->NextSiblingElement(elementName);
	}

	return found;
}

tinyxml2::XMLElement* ColladaLoader::findBone(tinyxml2::XMLElement* skeletonRoot, const char* boneName) {

	tinyxml2::XMLElement* node = nullptr;

	std::string name(boneName);

	tinyxml2::XMLElement* searching = skeletonRoot->FirstChildElement("node");

	while (searching != nullptr) {

		const char* searchName = searching->Attribute("sid");

		if (name.compare(searchName) == 0) {
			node = searching;
			break;
		}

		tinyxml2::XMLElement* rec = findBone(searching, boneName);

		if (rec) {
			node = rec;
			break;
		}

		searching = searching->NextSiblingElement("node");
	}

	return node;
}

#endif