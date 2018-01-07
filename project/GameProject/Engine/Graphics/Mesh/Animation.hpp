#ifndef ANIMATION_HPP
#define ANIMATION_HPP

/// Internal Includes
#include "../../Interfaces/ICAnimation.hpp"
#include "../../Formats/Formats.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes
#include <iostream>
#include <string>


namespace Engine {
	namespace Graphics {
		namespace Mesh {

			class Animation : public Interfaces::ICAnimation {

				struct Bone {
					uint8_t id;
					uint8_t parent;
					glm::mat4 matrix;
				};

				struct Bone2 {

					Bone2() {
						id = 0;
						parentId = 0;
						numChilds = 0;
						matrix = glm::mat4();
						children = nullptr;
					}

					Bone2(uint8_t _id, uint8_t _parentId, glm::mat4 _matrix) {
						id = _id;
						parentId = _parentId;
						numChilds = 0;
						matrix = _matrix;
						children = nullptr;
					}

					uint8_t id;
					uint8_t parentId;
					uint8_t numChilds;
					glm::mat4 matrix;
					glm::mat4 inverseBindPose;
					Bone2* children;

					void free() {
						for (uint8_t i = 0; i < numChilds; i++) {
							children[i].free();
						}
						delete [] children;

					}

					void addChild(Bone2 bone) {

						numChilds++;

						if (children) {
							Bone2* tmp = children;
							children = new Bone2[numChilds];

							memcpy(children, tmp, sizeof(Bone2) * (numChilds - 1));

							delete[] tmp;

						} else {
							children = new Bone2[numChilds];
						}

						children[numChilds - 1] = bone;
					}

					bool addBone(Bone2 bone) {
						bool added = false;
						if (bone.parentId == id) {
							addChild(bone);
							added = true;
						} else {
							for (uint8_t i = 0; i < numChilds; i++) {
								if (children[i].addBone(bone)) {
									added = true;
									break;
								}
							}
						}
						return added;
					}

					void printRecursive(std::string str) {
						std::cout << str << (int)id << std::endl;

						std::cout << inverseBindPose[0][0] << ", ";
						std::cout << inverseBindPose[0][1] << ", ";
						std::cout << inverseBindPose[0][2] << ", ";
						std::cout << inverseBindPose[0][3] << "\n";
						std::cout << inverseBindPose[1][0] << ", ";
						std::cout << inverseBindPose[1][1] << ", ";
						std::cout << inverseBindPose[1][2] << ", ";
						std::cout << inverseBindPose[1][3] << "\n";
						std::cout << inverseBindPose[2][0] << ", ";
						std::cout << inverseBindPose[2][1] << ", ";
						std::cout << inverseBindPose[2][2] << ", ";
						std::cout << inverseBindPose[2][3] << "\n";
						std::cout << inverseBindPose[3][0] << ", ";
						std::cout << inverseBindPose[3][1] << ", ";
						std::cout << inverseBindPose[3][2] << ", ";
						std::cout << inverseBindPose[3][3] << "\n";

						for (uint8_t i = 0; i < numChilds; i++) {
							children[i].printRecursive("|" + str);
						}
					}

					void calcInverseBindPose(glm::mat4 parentBindPose) {
						glm::mat4 bindTransform = parentBindPose * matrix;
						inverseBindPose = glm::inverse(bindTransform);
						for (uint8_t i = 0; i < numChilds; i++) {
							children[i].calcInverseBindPose(bindTransform);
						}
					}

					glm::mat4 getInverseBindPose(uint8_t _id, bool &found) {
						glm::mat4 res;
						if (id == _id) {
							res = inverseBindPose;
							found = true;
						} else {
							for (uint8_t i = 0; i < numChilds; i++) {
								res = children[i].getInverseBindPose(_id, found);
								if (found) {
									break;
								}
							}
						}
						return res;
					}

					void calcBindTransform(glm::mat4 parentBindPose) {
						glm::mat4 bindTransform = parentBindPose * matrix;
						for (uint8_t i = 0; i < numChilds; i++) {
							children[i].calcBindTransform(bindTransform);
						}
						matrix = bindTransform;
					}

					glm::mat4 getMatPose(uint8_t _id, bool &found) {
						glm::mat4 res;
						if (id == _id) {
							res = matrix;
							found = true;
						} else {
							for (uint8_t i = 0; i < numChilds; i++) {
								res = children[i].getMatPose(_id, found);
								if (found) {
									break;
								}
							}
						}
						return res;
					}

				};

			public:

				struct BindPoseInfo {

					uint16_t boneIndex;
					glm::mat4 transformMatrix;
					glm::mat4 bindTransform;

					uint16_t numChildren;
					BindPoseInfo* children;
				};

				Animation();
				virtual ~Animation();

				void setBindPoseData(BindPoseInfo* bones, uint8_t numBones);

				void setRawAnim(BindPoseInfo* bones);

				virtual void updateAnimation(float dt);
				virtual void* getCurrentAnimation(unsigned int &numJoints);

			private:

				uint8_t numBones;

				Bone* localBindPose;
				Bone* inverseBindPose;

				Bone* rawAnim;

				glm::mat4* currentAnimation;

			};

		}
	}
}
#endif