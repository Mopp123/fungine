#include "MeshVertexParsing.h"
#include "core/Common.h"

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
			// Loads all basic vertex stuff from the assimp mesh (vertex positions, normals, uvs, tangents, etc)
			void load_vertex_data(
				const aiScene* assimpScene,
				aiMesh* assimpMesh,
				std::vector<float>& outVertexData,
				std::vector<unsigned int>& outIndices,
				bool loadTangents,
				int meshBaseIndex
			)
			{
				const bool hasTexCoords = assimpMesh->mTextureCoords[0] != NULL;
				const bool hasNormals = assimpMesh->HasNormals();
				const bool hasTangents = loadTangents && assimpMesh->HasTangentsAndBitangents();

				outVertexData.reserve(3 * 2 * 3 * 3 * assimpMesh->mNumVertices);

				// Get all reguired stuff (vertex positions, uvs, normals, etc..)
				// Vertex position
				for (int i = 0; i < assimpMesh->mNumVertices; i++)
				{
					const aiVector3D& position = assimpMesh->mVertices[i];
					outVertexData.emplace_back(position.x);
					outVertexData.emplace_back(position.y);
					outVertexData.emplace_back(position.z);

					if (hasTexCoords)
					{
						const aiVector3D& texCoord = assimpMesh->mTextureCoords[0][i]; // we currently support only 1 texture coord per vertex!
						outVertexData.emplace_back(texCoord.x);
						outVertexData.emplace_back(texCoord.y);
					}
					if (hasNormals)
					{
						const aiVector3D& normal = assimpMesh->mNormals[i];
						outVertexData.emplace_back(normal.x);
						outVertexData.emplace_back(normal.y);
						outVertexData.emplace_back(normal.z);

						if (hasTangents)
						{
							const aiVector3D& tangent = assimpMesh->mTangents[i];
							const aiVector3D& bitangent = assimpMesh->mBitangents[i];

							mml::Vector3 myNormal(normal.x, normal.y, normal.z);
							mml::Vector3 myTangent(tangent.x, tangent.y, tangent.z);
							mml::Vector3 myBitangent(bitangent.x, bitangent.y, bitangent.z);

							// Fixing possible "flipped tangents"..
							if ((myNormal.cross(myTangent)).dot(myBitangent) < 0.0f)
								myTangent = myTangent * -1.0f;

							outVertexData.emplace_back(myTangent.x);
							outVertexData.emplace_back(myTangent.y);
							outVertexData.emplace_back(myTangent.z);
						}
					}
				}

				for (int i = 0; i < assimpMesh->mNumFaces; i++)
				{
					const aiFace& assimpFace = assimpMesh->mFaces[i];
					for (int j = 0; j < assimpFace.mNumIndices; j++)
						outIndices.emplace_back(assimpFace.mIndices[j]);
				}
			}


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
			)
			{
				std::vector<VertexBoneData> verticesBoneData(assimpMesh->mNumVertices);

				for (int boneIndex = 0; boneIndex < assimpMesh->mNumBones; boneIndex++)
				{
					aiBone* assimpBone = assimpMesh->mBones[boneIndex];
					std::string boneName = assimpBone->mName.data;

					// insert to the bone mapping only if it doesnt exist yet
					if (outBoneMapping.find(boneName) == outBoneMapping.end())
					{
						outBoneMapping.insert(std::make_pair(boneName, boneIndex));
						// Also add to the "bones list" only if we find a new "untouched bone", since this is done for each vertex..
						animation::Bone* myBone = new animation::Bone(boneName);
						mml::Mat4 bindPoseMatrix = convert_assimp_matrix(assimpBone->mOffsetMatrix);
						//bindPoseMatrix.inverse();
						myBone->setInverseBindPoseMatrix(bindPoseMatrix);
						bones.emplace_back(myBone);
					}

					// Go through each vertex this bone may affect..
					for (int vertexIndex = 0; vertexIndex < assimpBone->mNumWeights; vertexIndex++)
					{
						int vertexId = assimpBone->mWeights[vertexIndex].mVertexId;
						float weight = assimpBone->mWeights[vertexIndex].mWeight;

						for (int i = 0; i < LE__GRAPHICS__MAX_BONES_PER_VERTEX; i++)
						{
							if (verticesBoneData[vertexId].weights[i] < weight)
							{
								verticesBoneData[vertexId].IDs[i] = boneIndex;
								verticesBoneData[vertexId].weights[i] = weight;
								break;
							}
						}
					}
				}
				for (int i = 0; i < assimpMesh->mNumVertices; i++)
				{
					// Make all weights sum be 1
					float sum =
						verticesBoneData[i].weights[0] +
						verticesBoneData[i].weights[1] +
						verticesBoneData[i].weights[2] +
						verticesBoneData[i].weights[3];

					if (sum != 0.0f)
					{
						verticesBoneData[i].weights[0] /= sum;
						verticesBoneData[i].weights[1] /= sum;
						verticesBoneData[i].weights[2] /= sum;
						verticesBoneData[i].weights[3] /= sum;
					}
				}

				for (VertexBoneData& vbd : verticesBoneData)
				{
					for (int i = 0; i < LE__GRAPHICS__MAX_BONES_PER_VERTEX; i++)
					{
						outVertexBoneIDs.emplace_back(vbd.IDs[i]);
						outVertexBoneWeights.emplace_back(vbd.weights[i]);
					}
				}
			}*/


			// Converts assimp's matrix to our matrix..
			mml::Matrix4 convert_assimp_matrix(const aiMatrix4x4& assimpMatrix)
			{
				mml::Matrix4 ourMatrix(1.0f);
				ourMatrix[0 + 0 * 4] = assimpMatrix.a1;
				ourMatrix[0 + 1 * 4] = assimpMatrix.a2;
				ourMatrix[0 + 2 * 4] = assimpMatrix.a3;
				ourMatrix[0 + 3 * 4] = assimpMatrix.a4;

				ourMatrix[1 + 0 * 4] = assimpMatrix.b1;
				ourMatrix[1 + 1 * 4] = assimpMatrix.b2;
				ourMatrix[1 + 2 * 4] = assimpMatrix.b3;
				ourMatrix[1 + 3 * 4] = assimpMatrix.b4;

				ourMatrix[2 + 0 * 4] = assimpMatrix.c1;
				ourMatrix[2 + 1 * 4] = assimpMatrix.c2;
				ourMatrix[2 + 2 * 4] = assimpMatrix.c3;
				ourMatrix[2 + 3 * 4] = assimpMatrix.c4;

				ourMatrix[3 + 0 * 4] = assimpMatrix.d1;
				ourMatrix[3 + 1 * 4] = assimpMatrix.d2;
				ourMatrix[3 + 2 * 4] = assimpMatrix.d3;
				ourMatrix[3 + 3 * 4] = assimpMatrix.d4;

				return ourMatrix;
			}
		}
	}
}
