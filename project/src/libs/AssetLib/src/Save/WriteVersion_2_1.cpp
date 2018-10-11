#include "WriteVeresion.hpp"

#include "../CustomMesh/MeshFormat.hpp"

#include <fstream>

#include <vector>
#include <cassert>
#include <cstring>
#include <cmath>
#include <cfloat>

namespace AssetLib {

	bool floatCompare(float f1, float f2, float epsilon) {

		float dif = f1 - f2;
		dif = (float)fabs((float)dif);

		if (dif <= epsilon) {
			return true;
		}
		return false;
	}

	bool isInVector(const std::vector < AssetLib::Color4f>& vec, const AssetLib::Color4f& value, uint32_t ) {

		for (size_t i = 0; i < vec.size(); i++) {
			if (floatCompare(vec[i].b, value.b, FLT_EPSILON) &&
				floatCompare(vec[i].r, value.r, FLT_EPSILON) &&
				floatCompare(vec[i].g, value.g, FLT_EPSILON) &&
				floatCompare(vec[i].a, value.a, FLT_EPSILON)) {
				return true;
			}
		}
		return false;
	}
	// @ TODO this could get very slow
	bool compressVertexColors(AssetLib::Color4f* colors, uint32_t numVerts, std::vector<AssetLib::Color4f> newColors, std::vector<uint32_t> colorIndex) {

		uint32_t orgSize = sizeof(AssetLib::Color4f) * numVerts;
		bool compressOk = true;

		uint32_t index = 0;

		for (uint32_t i = 0; i < numVerts; i++) {

			if (isInVector(newColors, colors[i], index)) {
				colorIndex.push_back(index);
			} else {
				newColors.push_back(colors[i]);
				colorIndex.push_back((uint32_t)newColors.size() - 1);
			}
		}

		assert(numVerts == colorIndex.size() && "Input vertices does not match output vertices!");

		uint32_t newSize = (uint32_t)(sizeof(AssetLib::Color4f) * newColors.size());
		newSize += sizeof(uint32_t) * (uint32_t)colorIndex.size(); // can be reduced further by different index type

		if (newSize > orgSize) {
			compressOk = false;
			newColors.clear();
			colorIndex.clear();
		}

		return compressOk;
	}

	void saveMeshV2_1(const char* fileName, const AssetLib::Model& mdl, const  AssetLib::SaveFlags& ) {

		std::ofstream outFile(fileName, std::ios::binary);

		Format::Header::GenericHeader header;

		memcpy(header.tag, "MESH", 4);
		header.version = Format::Header::VERSION_2_1;
		header.dataType = Format::Header::MODEL_DATA_TYPE_ID;

		outFile.write((char*)&header, sizeof(header));

		Format::version_2_1::MeshInfo meshInfo;
		meshInfo.meshFlags = 0;
		meshInfo.meshFlags |= mdl.hasColors ? Format::version_2_1::MESH_INFO_FLAG_USE_VCOLORS : 0;
		meshInfo.meshFlags |= mdl.hasUv ? Format::version_2_1::MESH_INFO_FLAG_USE_UV : 0;
		meshInfo.meshFlags |= mdl.hasNormals ? Format::version_2_1::MESH_INFO_FLAG_USE_NORMALS : 0;

		meshInfo.numAnimations = static_cast<uint16_t>(mdl.numAnimations);
		meshInfo.numBones = static_cast<uint16_t>(mdl.numBones);
		meshInfo.numTris = mdl.numFaces;
		meshInfo.numVerts = mdl.numVertices;


		outFile.write((char*)mdl.verices, sizeof(Vector3f) * mdl.numVertices);

		if (mdl.hasNormals) {
			outFile.write((char*)mdl.normals, sizeof(Vector3f) * mdl.numVertices);
		}

		if (mdl.hasColors) {
			std::vector<AssetLib::Color4f> newColors;
			std::vector<uint32_t> colorIndex;
			/*if (false && compressVertexColors(mdl.vertexColors, mdl.numVertices, newColors, colorIndex)) {
				meshInfo.meshFlags |= (Format::version_2_1::MESH_INFO_FLAG_USE_COMPRESSION | Format::version_2_1::MESH_INFO_FLAG_COMP_VCOL_MAP);
				Format::version_2_1::VertexColorMapInfo vColorInfo;
				vColorInfo.numColors = (uint32_t)newColors.size();

				outFile.write((char*)&vColorInfo, sizeof(Format::version_2_1::VertexColorMapInfo));
				outFile.write((char*)newColors.data(), sizeof(AssetLib::Color4f) * newColors.size());
				outFile.write((char*)colorIndex.data(), sizeof(uint32_t) * colorIndex.size());

			} else */ {
				outFile.write((char*)mdl.vertexColors, sizeof(Color4f) * mdl.numVertices);
			}
		}

		if (mdl.hasUv) {
			outFile.write((char*)mdl.uvs, sizeof(Vector2f) * mdl.numVertices);
		}
		if (mdl.hasWeights) {
			outFile.write((char*)mdl.vertexWeights, sizeof(VertexBoneWeight) * mdl.numVertices);
			outFile.write((char*)mdl.vertexIndex, sizeof(VertexBoneIndex) * mdl.numVertices);
		}

		outFile.write((char*)mdl.faces, sizeof(Triangle) * mdl.numFaces);


	}
}