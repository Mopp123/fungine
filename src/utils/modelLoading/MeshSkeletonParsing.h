#pragma once

#include "assimp\scene.h"
#include "animations\skeletalAnimation\Skeleton.h"

namespace fungine
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
			);

			// Creates bone's keyframes for animating it..
			void create_bone_keyframes(
				animation::Bone* bone,
				const aiScene* assimpScene,
				const aiNode* node,
				const unsigned int& totalKeyFrameCount
			);


			// Returns all keyframes of a bone's animation
			// *NOTE We currently support only "1 animation channel(mAnimations[0])"
			const aiNodeAnim* get_node_anim(
				const std::string& boneName,
				const aiScene* assimpScene
			);

			// Converts "assimp key frame" to our format..
			void convert_assimp_keyframe(
				const aiNodeAnim* nodeAnim, unsigned int frame,
				const aiVector3D& assimpPos, const aiQuaternion& assimpRot, const aiVector3D& assimpScale,
				mml::Vec3& outPos, mml::Quat& outRot, mml::Vec3& outScale
			);
		}
	}
}