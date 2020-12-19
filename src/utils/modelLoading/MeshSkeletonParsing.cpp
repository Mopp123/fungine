
#include "MeshSkeletonParsing.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace modelLoading
		{
			// Loads and assembles a "prototype skeleton" from assimp mesh's bones
			// When calling this yourself to assemble a skeleton, the parent node should be the root node/bone (Bones of the mesh are aiNodes in assimp)!
			// This also creates all animations' keyframes for the bones!
			// *NOTE This DOESN'T create the keyframes for the root bone!!!
			void assemble_bones(
				const aiScene* assimpScene,
				const aiNode* parentNode,
				std::map<std::string, unsigned int>& boneMapping,
				const unsigned int totalKeyFrameCount,
				animation::Skeleton& outSkeleton,
				std::vector<animation::Bone*>& inoutBones
			)
			{
				unsigned int boneIndex = boneMapping[std::string(parentNode->mName.data)];

				// if firstIteration == true, it means that we inputted root node
				// we want to create 

				// Go through each of this node's children
				for (unsigned int i = 0; i < parentNode->mNumChildren; i++)
				{
					const aiNode* childNode = parentNode->mChildren[i];
					// If the bone mapping doesn't contain this bone
					//	-> don't touch it and ESPECIALLY DONT use the bone mapping's [] operator so we dont add any new elements in the map!
					if (boneMapping.find(std::string(childNode->mName.data)) == boneMapping.end())
					{
						assemble_bones(assimpScene, childNode, boneMapping, totalKeyFrameCount, outSkeleton, inoutBones);
						continue;
					}
					unsigned int childBoneIndex = boneMapping[std::string(childNode->mName.data)];

					// If the child bone was the parent bone, ..its kind of fucked, try make this not to crash..
					// (For some reason the bone mapping may get funky looking, and this may actually happen, because fucking assimp.. or then im just fucking stupid..)
					if (childBoneIndex == boneIndex)
					{
						Debug::log(R"(
WARNING: Location: void modelLoading::assemble_bones(
const aiScene*,
const aiNode*,
std::map<std::string, unsigned int>&,
const unsigned int,
animation::Skeleton&,
std::vector<animation::Bone*>&
)
When assembling skeleton, child bone's index was equal to its' parent's index.
This may be normal, ..or not.. 
Parent = )" + std::string(parentNode->mName.data) + " Child = " + std::string(childNode->mName.data)
);
						assemble_bones(assimpScene, childNode, boneMapping, totalKeyFrameCount, outSkeleton, inoutBones);
						continue;
					}

					// Add the bone to the skeleton(and also tell which bone is the parent of this bone)
					outSkeleton.addBone(boneIndex, childBoneIndex);

					// Create keyframes for this child bone
					create_bone_keyframes(inoutBones[childBoneIndex], assimpScene, childNode, totalKeyFrameCount);

					// Then assemble the child bones of this child bone in the skeleton...
					assemble_bones(assimpScene, childNode, boneMapping, totalKeyFrameCount, outSkeleton, inoutBones);
				}
			}


			void create_bone_keyframes(
				animation::Bone* bone,
				const aiScene* assimpScene,
				const aiNode* node,
				const unsigned int& totalKeyFrameCount
			)
			{
				// Get all keyframes for this bone (if it is saved in the file)..
				const aiNodeAnim* nodeAnim = get_node_anim(node->mName.data, assimpScene);

				if (nodeAnim != nullptr)
				{
					// A single keyframe's pos rot and scale
					mml::Vec3 pos;
					mml::Quat rot;
					mml::Vec3 scale;
					unsigned int frameIndex = 0;
					unsigned int assimpNodeKeyCount = nodeAnim->mNumPositionKeys;
					

					// We want each bone to have matching number of keyframes
					for (frameIndex = 0; frameIndex < totalKeyFrameCount; frameIndex++)
					{
						printf("keyframe added!\n");
						// If you can find a keyframe from the assimp node -> add it
						if (frameIndex < assimpNodeKeyCount)
						{
							convert_assimp_keyframe(
								nodeAnim, frameIndex,
								nodeAnim->mPositionKeys[frameIndex].mValue,
								nodeAnim->mRotationKeys[frameIndex].mValue,
								nodeAnim->mScalingKeys[frameIndex].mValue,
								pos, rot, scale
							);
							bone->addKeyframe(animation::Keyframe(pos, rot, scale));
						}
						else
						{
							// If we didn't get all the keyframes from the assimp node
							//	-> make the remaining last keyframes to be the last loaded keyframe..
							bone->addKeyframe(animation::Keyframe(pos, rot, scale));
						}
					}

				}
				else
				{
					// If you cant get the keyframe from the file
					//	-> create a keyframe from the node's bind pose transformation matrix
					mml::Vec3 pos;
					mml::Quat rot(0, 0, 0, 1);
					mml::Vec3 scale(1, 1, 1);

					mml::Mat4 bindMatrix = bone->getInverseBindPoseMatrix();
					bindMatrix.inverse();

					if (bone->getParentBone() != nullptr)
					{
						mml::Mat4 inverseBindMatrix = bone->getInverseBindPoseMatrix();

						mml::Mat4 inverseParentBindMatrix = bone->getParentBone()->getInverseBindPoseMatrix();
						mml::Mat4 parentBindMatrix = bone->getParentBone()->getInverseBindPoseMatrix();
						parentBindMatrix.inverse();

						mml::Mat4 m = inverseParentBindMatrix * bindMatrix;

						mml::disassemble_transformation_matrix(m, pos, rot, scale);
					}
					else
					{
						mml::disassemble_transformation_matrix(bindMatrix, pos, rot, scale);
					}


					for (unsigned int frameIndex = 0; frameIndex < totalKeyFrameCount; frameIndex++)
						bone->addKeyframe(animation::Keyframe(pos, rot, scale));
				}
			}


			// Returns all keyframes of a bone's animation
			// *NOTE We currently support only "1 animation channel(mAnimations[0])"
			const aiNodeAnim* get_node_anim(
				const std::string& boneName,
				const aiScene* assimpScene
			)
			{
				if (assimpScene->mAnimations == nullptr)
					return nullptr;

				// Each bone's animation in the model
				for (unsigned int i = 0; i < assimpScene->mAnimations[0]->mNumChannels; i++)
				{
					if (std::string(assimpScene->mAnimations[0]->mChannels[i]->mNodeName.data) == boneName)
						return assimpScene->mAnimations[0]->mChannels[i];
				}
				return nullptr;
			}


			// Converts "assimp key frame" to our format..
			void convert_assimp_keyframe(
				const aiNodeAnim* nodeAnim, unsigned int frame,
				const aiVector3D& assimpPos, const aiQuaternion& assimpRot, const aiVector3D& assimpScale,
				mml::Vec3& outPos, mml::Quat& outRot, mml::Vec3& outScale
			)
			{
				outPos = mml::Vec3(
					nodeAnim->mPositionKeys[frame].mValue.x,
					nodeAnim->mPositionKeys[frame].mValue.y,
					nodeAnim->mPositionKeys[frame].mValue.z
				);
				outRot = mml::Quat(
					nodeAnim->mRotationKeys[frame].mValue.x,
					nodeAnim->mRotationKeys[frame].mValue.y,
					nodeAnim->mRotationKeys[frame].mValue.z,
					nodeAnim->mRotationKeys[frame].mValue.w
				);
				outScale = mml::Vec3(
					nodeAnim->mScalingKeys[frame].mValue.x,
					nodeAnim->mScalingKeys[frame].mValue.y,
					nodeAnim->mScalingKeys[frame].mValue.z
				);
			}
		}
	}
}
