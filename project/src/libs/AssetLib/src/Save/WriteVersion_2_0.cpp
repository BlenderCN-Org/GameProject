
#include "WriteVeresion.hpp"


#include "../CustomMesh/MeshFormat.hpp"

#include <fstream>
#include <cstring>

namespace AssetLib {

	void saveMeshV2_0(const char* fileName, const AssetLib::Model& mdl, const AssetLib::SaveFlags& ) {

		using namespace Format::Header;
		using namespace Format::version_2_0;

		std::ofstream outFile(fileName, std::ios::binary);

		GenericHeader header;

		memcpy(header.tag, "MESH", 4);
		header.version = VERSION_2_0;
		header.dataType = MODEL_DATA_TYPE_ID;

		outFile.write((char*)&header, sizeof(header));

		MeshInfo meshInfo;
		meshInfo.meshFlags = 0;
		meshInfo.meshFlags |= mdl.hasColors ? MESH_INFO_FLAG_USE_VCOLORS : 0;
		meshInfo.meshFlags |= mdl.hasUv ? MESH_INFO_FLAG_USE_UV : 0;
		meshInfo.meshFlags |= mdl.hasNormals ? MESH_INFO_FLAG_USE_NORMALS : 0;

		meshInfo.numAnimations = static_cast<uint16_t>(mdl.numAnimations);
		meshInfo.numBones = static_cast<uint8_t>(mdl.numBones);
		meshInfo.numTris = mdl.numFaces;
		meshInfo.numVerts = mdl.numVertices;

		outFile.write((char*)&meshInfo, sizeof(MeshInfo));

		outFile.write((char*)mdl.verices, sizeof(Vector3f) * mdl.numVertices);

		if (mdl.hasNormals) {
			outFile.write((char*)mdl.normals, sizeof(Vector3f) * mdl.numVertices);
		}

		if (mdl.hasColors) {
			outFile.write((char*)mdl.vertexColors, sizeof(Color4f) * mdl.numVertices);
		}

		if (mdl.hasUv) {
			outFile.write((char*)mdl.uvs, sizeof(Vector2f) * mdl.numVertices);
		}
		if (mdl.hasWeights) {
			outFile.write((char*)mdl.vertexWeights, sizeof(VertexBoneWeight) * mdl.numVertices);
			outFile.write((char*)mdl.vertexIndex, sizeof(VertexBoneIndex) * mdl.numVertices);
		}

		outFile.write((char*)mdl.faces, sizeof(Triangle) * mdl.numFaces);

		if (mdl.numBones > 0) {

			//Name* boneNames = new Name[mdl.numBones];
			

			//mdl.skeleton

		}
	}

}