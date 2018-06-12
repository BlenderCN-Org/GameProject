/// Internal Includes
#include "Animation.hpp"

/// External Includes

/// Std Includes
#include <functional>
#include <vector>


namespace Engine {
	namespace Graphics {
		namespace Mesh {

			Animation::Animation() :
				localBindPose(nullptr),
				inverseBindPose(nullptr),
				currentAnimation(nullptr),
				rawAnim(nullptr),
				numBones(0U) {}

			Animation::~Animation() {

				if (localBindPose) {
					delete[] localBindPose;
				}
				if (inverseBindPose) {
					delete[] inverseBindPose;
				}
				if (currentAnimation) {
					delete[] currentAnimation;
				}
				if (rawAnim) {
					delete[] rawAnim;
				}

				numBones = 0;

				localBindPose = nullptr;
				inverseBindPose = nullptr;
				currentAnimation = nullptr;
				rawAnim = nullptr;
			}

			void Animation::setBindPoseData(BindPoseInfo* bones, uint8_t _numBones) {
				numBones = _numBones;

				localBindPose = new Bone[numBones];
				inverseBindPose = new Bone[numBones];
				rawAnim = new Bone[numBones];
				currentAnimation = new glm::mat4[numBones];

				std::function<void(BindPoseInfo*, glm::mat4)> calcInverse = [&](BindPoseInfo* bone, glm::mat4 parent)
				{
					uint16_t index = bone->boneIndex;

					glm::mat4 lbp = bone->transformMatrix * parent;
					
					for (size_t i = 0; i < bone->numChildren; i++) {
						calcInverse(&bone->children[i], lbp);
					}

					localBindPose[index].matrix = lbp;
					inverseBindPose[index].matrix = glm::inverse(lbp);

				};

				calcInverse(bones, glm::mat4());

				//std::vector<Bone2> boneArr;
				//
				//Bone2 bindPose;
				//
				//for (uint8_t i = 0; i < numBones; i++) {
				//	Formats::ModelFormat::version_2_0::BoneInfo &bone = bones[i];
				//	uint16_t id = bone.boneId;
				//	
				//	// local bindPose Model space
				//	localBindPose[i].id = (uint8_t)id;
				//	localBindPose[i].parent = (uint8_t)bone.parentBone;
				//	localBindPose[i].matrix = *(glm::mat4*)bone.localBindPose;
				//
				//	inverseBindPose[i] = localBindPose[i];
				//
				//	// inverted in model space
				//	inverseBindPose[i].matrix = glm::inverse(inverseBindPose[i].matrix);
				//
				//	Bone2 b((uint8_t)id, (uint8_t)bone.parentBone, *(glm::mat4*)bone.localBindPose);					
				//	
				//	boneArr.push_back(b);
				//}
				//
				//for (size_t i = 0; i < boneArr.size(); i++) {
				//	if (boneArr[i].id == boneArr[i].parentId) {
				//		bindPose = boneArr[i];
				//		std::vector<Bone2>::iterator it = boneArr.begin();
				//		it += i;
				//		boneArr.erase(it);
				//		break;
				//	}
				//}
				//
				//while (!boneArr.empty()) {
				//	for (size_t i = 0; i < boneArr.size(); i++) {
				//		if (bindPose.addBone(boneArr[i])) {
				//			std::vector<Bone2>::iterator it = boneArr.begin();
				//			it += i;
				//			boneArr.erase(it);
				//			break;
				//		}
				//	}
				//}
				//bindPose.printRecursive("|-");
				//bindPose.calcInverseBindPose(glm::mat4());
				//bindPose.printRecursive("|-");
				//
				//for (uint8_t i = 0; i < numBones; i++) {
				//	bool found = false;
				//	inverseBindPose[i].matrix = bindPose.getInverseBindPose(i, found);
				//	if (found == false) {
				//		throw("Fatal Animation Error!\n", i, " not found!");
				//	}
				//}
				//
				//bindPose.free();

			}

			void Animation::setRawAnim(BindPoseInfo* bones) {

				//for (uint8_t i = 0; i < numBones; i++) {
				//	uint16_t id = data[i].boneId;
				//
				//	rawAnim[i].id = (uint8_t)id;
				//	rawAnim[i].parent = (uint8_t)data[i].parentBone;
				//	rawAnim[i].matrix = *(glm::mat4*)data[i].transform;
				//}

				std::function<void(BindPoseInfo*, glm::mat4)> raw = [&](BindPoseInfo* bone, glm::mat4 parent) {
					uint16_t index = bone->boneIndex;

					glm::mat4 lbp = bone->transformMatrix * parent;

					for (size_t i = 0; i < bone->numChildren; i++) {
						raw(&bone->children[i], lbp);
					}
					rawAnim[index].matrix = lbp;
				};

				raw(bones, glm::mat4());

			}

			void Animation::updateAnimation(float dt) {

				//std::vector<Bone2> boneArr;
				//
				//Bone2 bindPose;
				//
				//for (uint8_t i = 0; i < numBones; i++) {
				//	Bone b = rawAnim[i];
				//	Bone2 b2((uint8_t)b.id, (uint8_t)b.parent, b.matrix);
				//
				//	boneArr.push_back(b2);
				//}
				//
				//for (size_t i = 0; i < boneArr.size(); i++) {
				//	if (boneArr[i].id == boneArr[i].parentId) {
				//		bindPose = boneArr[i];
				//		std::vector<Bone2>::iterator it = boneArr.begin();
				//		it += i;
				//		boneArr.erase(it);
				//		break;
				//	}
				//}
				//
				//while (!boneArr.empty()) {
				//	for (size_t i = 0; i < boneArr.size(); i++) {
				//		if (bindPose.addBone(boneArr[i])) {
				//			std::vector<Bone2>::iterator it = boneArr.begin();
				//			it += i;
				//			boneArr.erase(it);
				//			break;
				//		}
				//	}
				//}
				//
				////bindPose.calcBindTransform(glm::mat4());
				//
				//glm::mat4 bind;
				//
				//for (uint8_t i = 0; i < numBones; i++) {
				//	bool found = false;
				//
				//	//bind = bind * rawAnim[i].matrix;
				//
				//	//currentAnimation[i] = bindPose.getMatPose(i, found) * inverseBindPose[i].matrix;
				//	currentAnimation[i] = localBindPose[i].matrix;
				//	//currentAnimation[i] = localBindPose[i].matrix * inverseBindPose[i].matrix;
				//	//currentAnimation[i] = rawAnim[i].matrix * inverseBindPose[i].matrix;
				//}
				//
				//bindPose.free();

				glm::mat4 g;

				for (uint8_t i = 0; i < numBones; i++) {

					//g = rawAnim[i].matrix *g;

					currentAnimation[i] = inverseBindPose[i].matrix * rawAnim[i].matrix;
					currentAnimation[i] = glm::transpose(currentAnimation[i]);

					int b = 0;

				}

			}

			void* Animation::getCurrentAnimation(unsigned int &numJoints) {
				numJoints = numBones;
				return currentAnimation;
			}

		}
	}
}