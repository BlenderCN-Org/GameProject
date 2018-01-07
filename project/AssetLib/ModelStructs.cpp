#include "ModelStucts.hpp"

#include <memory>

#include <functional>

namespace AssetLib {


	SkeletonBone deepCopy(SkeletonBone* org) {

		SkeletonBone b;

		b.boneIndex = org->boneIndex;

		size_t len = strlen(org->boneName);

		b.boneName = new char[len + 1];
		memcpy((void*)b.boneName, org->boneName, len + 1);

		b.transform = org->transform;
		b.numChildren = org->numChildren;

		if (org->children) {

			b.children = new SkeletonBone[b.numChildren];

			for (size_t i = 0; i < b.numChildren; i++) {
				b.children[i] = deepCopy(&org->children[i]);
			}
		} else {
			b.children = nullptr;
		}

		return b;
	}

	Animation deepCopy(Animation* org) {

		Animation a;
		a.animationName = org->animationName;
		a.numJoints = org->numJoints;

		a.joint = new JointKeyFrames[a.numJoints];

		for (uint32_t i = 0; i < a.numJoints; i++) {

			a.joint[i] = org->joint[i];

			a.joint[i].transforms = new JointTransform[a.joint[i].numTransforms];

			for (uint32_t j = 0; j < a.joint[i].numTransforms; j++) {
				a.joint[i].transforms[j] = org->joint[i].transforms[j];
			}
		}

		return a;
	}

	bool SkeletonBone::addChild(SkeletonBone newChild, unsigned short childOf) {

		bool success = false;

		if (childOf == boneIndex) {
			SkeletonBone* old = children;

			numChildren++;
			children = new SkeletonBone[numChildren];

			if (old) {
				for (uint16_t i = 0; i < (numChildren - 1); i++) {
					children[i] = old[i];
				}
			}
			children[numChildren - 1] = newChild;
			success = true;

			delete[] old;

		} else {
			for (uint16_t i = 0; i < numChildren; i++) {
				success = children[i].addChild(newChild, childOf);
				if (success) {
					break;
				}
			}
		}
		return success;
	}

	Model::Model()
		: numVertices(0U)
		, hasUv(false)
		, hasNormals(false)
		, hasColors(false)
		, hasWeights(false)
		, verices(nullptr)
		, uvs(nullptr)
		, normals(nullptr)
		, vertexColors(nullptr)
		, vertexWeights(nullptr)
		, vertexIndex(nullptr)
		, numFaces(0U)
		, faces(nullptr)
		, numBones(0U)
		, skeleton(nullptr)
		, numAnimations(0U)
		, animations(nullptr) {}

	Model::Model(const Model &mdl) {
		numVertices = mdl.numVertices;
		hasUv = mdl.hasUv;
		hasNormals = mdl.hasNormals;
		hasColors = mdl.hasColors;
		hasWeights = mdl.hasWeights;

		if (mdl.verices) {
			verices = new Vector3f[numVertices];
			memcpy(verices, mdl.verices, sizeof(Vector3f) * numVertices);
		} else {
			verices = nullptr;
		}
		if (mdl.uvs) {
			uvs = new Vector2f[numVertices];
			memcpy(uvs, mdl.uvs, sizeof(Vector2f) * numVertices);
		} else {
			uvs = nullptr;
		}
		if (mdl.normals) {
			normals = new Vector3f[numVertices];
			memcpy(normals, mdl.normals, sizeof(Vector3f) * numVertices);
		} else {
			normals = nullptr;
		}
		if (mdl.vertexColors) {
			vertexColors = new Color4f[numVertices];
			memcpy(vertexColors, mdl.vertexColors, sizeof(Color4f) * numVertices);
		} else {
			vertexColors = nullptr;
		}
		if (mdl.vertexWeights) {
			vertexWeights = new VertexBoneWeight[numVertices];
			memcpy(vertexWeights, mdl.vertexWeights, sizeof(VertexBoneWeight) * numVertices);
		} else {
			vertexWeights = nullptr;
		}
		if (mdl.vertexIndex) {
			vertexIndex = new VertexBoneIndex[numVertices];
			memcpy(vertexIndex, mdl.vertexIndex, sizeof(VertexBoneIndex) * numVertices);
		} else {
			vertexIndex = nullptr;
		}

		numFaces = mdl.numFaces;
		if (mdl.faces) {
			faces = new Triangle[numFaces];
			memcpy(faces, mdl.faces, sizeof(Triangle) * numFaces);
		} else {
			faces = nullptr;
		}

		numBones = mdl.numBones;
		if (mdl.skeleton) {
			//skeleton 
			skeleton = new SkeletonBone;
			*skeleton = deepCopy(mdl.skeleton);
		} else {
			skeleton = nullptr;
		}

		numAnimations = mdl.numAnimations;

		if (mdl.animations) {
			animations = new Animation[numAnimations];
			for (uint32_t i = 0; i < numAnimations; i++) {
				animations[i] = deepCopy(&mdl.animations[i]);
			}
		} else {
			animations = nullptr;
		}

	}

	void deleteChilds(SkeletonBone* skel) {

		for (unsigned short i = 0; i < skel->numChildren; i++) {

			deleteChilds(&skel->children[i]);
		}

		delete[] skel->boneName;
		delete[] skel->children;
	}

	Model::~Model() {

		delete[] verices;
		delete[] uvs;
		delete[] normals;
		delete[] vertexColors;
		delete[] vertexWeights;
		delete[] vertexIndex;
		delete[] faces;

		if (skeleton) {

			deleteChilds(skeleton);

			delete skeleton;
		}
		
		if (animations) {

			for (uint32_t i = 0; i < numAnimations; i++) {

				for (uint32_t j = 0; j < animations[i].numJoints; j++) {
					delete[] animations[i].joint[j].transforms;
				}
				delete[] animations[i].joint;
			}

			delete[] animations;
		}
	}

}