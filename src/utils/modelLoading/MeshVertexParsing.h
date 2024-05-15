#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include <vector>
#include <map>
#include <string>
#include <assimp/scene.h>

/*
	*NOTE
		All this data needs to be put together for the "final buffers" following way:
		1 buffer contains all "basic vertex stuff"
		position, uv, normal, tangent -> and then the next vertex..

		1 buffer contains all the bone ids (in 4d integer vector) which affects this vertex
		1 buffer contains all the bone weights

	- meshBaseIndex means which mesh of the model, we are handling
*/

namespace fungine
{
	namespace graphics
	{
		namespace modelLoading
		{

		// Loads all basic vertex stuff from the assimp mesh (vertex positions, normals, uvs, tangents, indices)
			void load_vertex_data(
				const aiScene* assimpScene,
				aiMesh* assimpMesh,
				std::vector<float>& outVertexData,
				std::vector<unsigned int>& outIndices,
				bool loadTangents,
				int meshBaseIndex
			);


			struct VertexBoneData
			{
				int IDs[4];
				float weights[4];
			};

			// Loads skinning data of the assimp mesh.
			// Also constructs the "bone mapping", and loads all bones, because
			// why the fuck not, since we are looping through our bones..
			/*void load_skinning_data(
				const aiScene* assimpScene,
				aiMesh* assimpMesh,
				std::map<std::string, unsigned int>& outBoneMapping,
				std::vector<animation::Bone*>& bones,
				std::vector<int>& outVertexBoneIDs,
				std::vector<float>& outVertexBoneWeights,
				int meshBaseIndex
			);*/


			// Converts assimp's matrix to our matrix..
			mml::Matrix4 convert_assimp_matrix(const aiMatrix4x4& assimpMatrix);
		}
	}
}
