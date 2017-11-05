#ifndef I_ASSET_DATA
#define I_ASSET_DATA

// Internal Includes

// External Includes

// Std Includes
#include <cstdint>

namespace Engine {
	namespace Interfaces {

		enum class DataType {
			INVALID,
			MESH_POINTS,
			MESH_TRIANGLES,
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_CUBEMAP,
		};

		enum class DataFormat {
			INVALID,
			VERTEX,
			VERTEX_UV,
			VERTEX_UV_NORMALS,
			VERTEX_UV_COLORS,
			VERTEX_UV_COLORS_NORMALS,
			TEXTURE_RGBA,
			TEXTURE_RGB,
			TEXTURE_8BIT
		};

		enum class CollisionType {
			NONE,
			AABB,
			OOBB,
			SPHERE,
			MESH_CONVEX,
			MESH_TRIS
		};

		class IAssetData {

		public:

			virtual ~IAssetData() {};

			virtual void setDataType(DataType dataType) = 0;
			virtual void setData(void* data, uint32_t dataSize) = 0;
			virtual void setDataFormat(DataFormat dataFormat) = 0;
			virtual void setCollisionType(CollisionType collisionType) = 0;
			virtual void setCollisionData(void* data, uint32_t dataSize) = 0;
		};
	}
}


#endif