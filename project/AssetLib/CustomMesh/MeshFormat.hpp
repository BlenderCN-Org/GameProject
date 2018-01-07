#ifndef MESH_FORMAT_HPP
#define MESH_FORMAT_HPP

#include <cstdint>

namespace Format {
	namespace Header {

		const uint16_t INVALID_DATA_TYPE_ID = 0x0000;
		const uint16_t MODEL_DATA_TYPE_ID = 0x0001;

		struct GenericHeader {
			char tag[4];
			uint16_t version;
			uint16_t dataType;
		};
	}

	namespace version_2_0 {

		const uint8_t MESH_INFO_FLAG_USE_UV = 0x01;
		const uint8_t MESH_INFO_FLAG_USE_NORMALS = 0x02;
		const uint8_t MESH_INFO_FLAG_USE_VCOLORS = 0x04;

		struct MeshInfo {
			uint8_t meshFlags;
			uint8_t numBones;
			uint16_t numAnimations;
			uint32_t numVerts;
			uint32_t numTris;
		};

		struct Name {
			uint16_t length;
			char* name;
		};

		struct BoneInfo {
			uint16_t boneId;
			uint16_t nameIndex;
			uint16_t parentBone;
			uint16_t padding;
			float localBindPose[16];
		};

		struct AnimationInfo {
			uint16_t animationId;
			uint16_t numKeyFrames;
		};

		struct KeyFrame {
			uint16_t boneId;
			uint16_t parentBone;
			float transform[16];
		};
	}
}

#endif