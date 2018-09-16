#ifndef MESH_FORMAT_HPP
#define MESH_FORMAT_HPP

#include <cstdint>

namespace Format {
	namespace Header {

		const uint16_t INVALID_DATA_TYPE_ID = 0x0000;
		const uint16_t MODEL_DATA_TYPE_ID = 0x0001;

		const uint16_t VERSION_2_0 = 0x0100;
		const uint16_t VERSION_2_1 = 0x0201;

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

	namespace version_2_1 {

		const uint16_t MESH_INFO_FLAG_USE_UV = version_2_0::MESH_INFO_FLAG_USE_UV;
		const uint16_t MESH_INFO_FLAG_USE_NORMALS = version_2_0::MESH_INFO_FLAG_USE_NORMALS;
		const uint16_t MESH_INFO_FLAG_USE_VCOLORS = version_2_0::MESH_INFO_FLAG_USE_VCOLORS;

		const uint16_t MESH_INFO_FLAG_USE_COMPRESSION = 0x0008;

		const uint16_t MESH_INFO_FLAG_COMP_INDEX_REDUCE = 0x0100;
		const uint16_t MESH_INFO_FLAG_COMP_VCOL_MAP = 0x0200;

		struct MeshInfo {
			uint16_t meshFlags;
			uint16_t numBones;
			uint16_t numAnimations;
			uint32_t numVerts;
			uint32_t numTris;
		};

		struct VertexColorMapInfo {
			uint32_t numColors;
		};

		typedef version_2_0::Name Name;
		typedef version_2_0::BoneInfo BoneInfo;
		typedef version_2_0::AnimationInfo AnimationInfo;
		typedef version_2_0::KeyFrame KeyFrame;

	}

}

#endif