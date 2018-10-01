#ifndef MODEL_STRUCTS_HPP
#define MODEL_STRUCTS_HPP

namespace AssetLib {

	struct Vector2f {
		float u, v;
	};

	struct Vector3f {
		float x, y, z;
	};

	struct Color4f {
		float r, g, b, a;
	};

	struct Quaternion {
		float x, y, z, w;
	};

	struct VertexBoneWeight {
		float w1, w2, w3, w4;
	};

	struct VertexBoneIndex {
		int w1, w2, w3, w4;
	};

	struct Matrix4x4 {
		float m11, m12, m13, m14;
		float m21, m22, m23, m24;
		float m31, m32, m33, m34;
		float m41, m42, m43, m44;
	};

	struct Triangle {
		unsigned int v1;
		unsigned int v2;
		unsigned int v3;
	};

	struct SkeletonBone {
		
		unsigned short boneIndex;
		const char* boneName;
		Matrix4x4 transform;
		unsigned short numChildren;
		SkeletonBone* children;

		bool addChild(SkeletonBone newChild, unsigned short childOf);

	};

	struct JointTransform {
		Vector3f position;
		Quaternion rotation;
		Matrix4x4 transform;
	};

	struct JointKeyFrames {
		int boneIndex;
		unsigned int numTransforms;
		JointTransform* transforms;
		float* triggerTime;
	};

	struct Animation {
		const char* animationName;
		unsigned int numJoints;
		JointKeyFrames* joint;
	};

	class Model {
	public:
		Model();
		Model(const Model &mdl);
		~Model();
		unsigned int numVertices;

		bool hasUv;
		bool hasNormals;
		bool hasColors;
		bool hasWeights;

		Vector3f* verices;
		Vector2f* uvs;
		Vector3f* normals;
		Color4f* vertexColors;
		VertexBoneWeight* vertexWeights;
		VertexBoneIndex* vertexIndex;

		unsigned int numFaces;
		Triangle* faces;

		unsigned int numBones;
		SkeletonBone* skeleton;

		unsigned int numAnimations;
		Animation* animations;
	};

	template<typename T>
	T toType(Vector2f &v) {
		return T(v.u, v.v);
	}

	template<typename T>
	T toType(Vector3f &v) {
		return T(v.x, v.y, v.z);
	}

	template<typename T>
	T toType(Color4f &c) {
		return T(c.r, c.g, c.b, c.a);
	}

	template<typename T>
	T toType(Quaternion &q) {
		return T(q.x, q.y, q.z, q.w);
	}

	template<typename T>
	T toType(VertexBoneWeight &w) {
		return T(w.w1, w.w2, w.w3, w.w4);
	}

	template<typename T>
	T toType(VertexBoneIndex &w) {
		return T(w.w1, w.w2, w.w3, w.w4);
	}

	template<typename T>
	T toType(Matrix4x4 &m) {
		return T(
			m.m11, m.m12, m.m13, m.m14,
			m.m21, m.m22, m.m23, m.m24,
			m.m31, m.m32, m.m33, m.m34,
			m.m41, m.m42, m.m43, m.m44);
	}

}

#endif