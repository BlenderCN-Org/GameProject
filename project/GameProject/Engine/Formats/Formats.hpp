#ifndef FORMATS_HPP
#define FORMATS_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <cstdint>

namespace Engine {
	namespace Formats {

		const uint16_t INVALID_DATA_TYPE_ID = 0x0000;
		const uint16_t MODEL_DATA_TYPE_ID = 0x0001;

		struct GenericHeader {
			char tag[4];
			uint16_t version;
			uint16_t dataType;
		};

		struct Triangle {
			uint32_t v1;
			uint32_t v2;
			uint32_t v3;
		};

		namespace ModelFormat {

			/** Structure example version_2_0
			---- Mesh Info ----
				flags
				numBones
				numAnimations
				numVerts
				numTris
			---- data ----
				verts < list
				normals < list < & flags
				colors < list < & flags
				uv < list < & flags
				weitghts[4] < list < & bones
				weitghtsInd[4] < list < & bones
			--- bone name list --- < list
				nameLength
				name[nameLength]
			---- bone info ---- < list
				boneId
				boneNameId
				parentBone
				localBindPose
			---- anim info ---- < list
				animId
				numKeyFrames
				---- key frame ---- < list
					position
					rotation
					scale
			---- end mesh ----
			*/
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

				struct BoneInfo {
					uint16_t boneId;
					uint16_t parentBone;
					float localBindPose[16];
				};

				struct AnimationInfo {
					uint16_t animationId;
					uint8_t numKeyFrames;
					uint8_t padding;
				};

				struct KeyFrame {
					uint16_t boneId;
					uint16_t parentBone;
					float transform[16];
				};

			}
		}
	}
}

#endif