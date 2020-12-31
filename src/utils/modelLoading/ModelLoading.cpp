
#include "ModelLoading.h"
#include "graphics/Graphics.h"
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include "graphics/RendererCommands.h"
#include "components/rendering/Material.h"

#include "MeshVertexParsing.h"
//#include "MeshSkeletonParsing.h"

#include "core\Debug.h"
#include "core\Common.h"

/*
	Limitations :

		For all meshes:
		- We support only 2D texture coordinates (No cube mapping..)
		- Currently there can be only 1 diffuse, specular and normal texture per mesh (we cannot have texture blending at the moment..)
		- Currently we support only 1 set of texture coordinates per vertex! because of the above! (stuff in function: "convert_assimp_mesh(aiMesh*)")

		For skinned meshes:
		- Skinned meshes can have only "one" animation
		- Skinned mesh animations NEED TO HAVE keyframes for each bone always at the same time
*/


#define DISABLE_SKINNED_MESH_LOADING

#define MODEL_LOADING__RESOURCE_FOLDER__TEXTURE		"res/"


namespace fungine
{
	using namespace components;


	namespace graphics
	{
		namespace modelLoading
		{
			// converts assimp mesh to our mesh type
			static std::shared_ptr<Mesh> convert_assimp_mesh(
				const aiScene* assimpScene, 
				aiMesh* assimpMesh, 
				unsigned int meshBaseIndex, 
				bool loadTangents, 
				bool loadBones,
				unsigned int instanceCount
			)
			{
				const bool hasTexCoords = assimpMesh->HasTextureCoords(0);
				const bool hasNormals = assimpMesh->HasNormals();
				const bool hasTangents = loadTangents && assimpMesh->HasTangentsAndBitangents();
				const bool hasBones = loadBones && assimpMesh->HasBones();

				std::string meshName = assimpMesh->mName.C_Str();

				// Basic vertex data
				std::vector<float> vertexData;
				std::vector<unsigned int> indices;

				load_vertex_data(
					assimpScene, assimpMesh,
					vertexData,
					indices,
					hasTangents,
					meshBaseIndex
				);

				// Create layout for this vertex buffer depending on the shit this mesh has..
				// *We will always have vertex position obviously..
				VertexBufferLayout vb_layout(
				{
					{ 0, ShaderDataType::Float3 }
				});
				
				if (hasTexCoords)	vb_layout.add({ 1, ShaderDataType::Float2 });
				if (hasNormals)		vb_layout.add({ 2, ShaderDataType::Float3 });
				if (hasTangents)	vb_layout.add({ 3, ShaderDataType::Float3 });

				// Create the fat "basic stuff" vertex buffer
				VertexBuffer<float>* vertexBuffer = VertexBuffer<float>::create_vertex_buffer(
					&vertexData[0], 
					sizeof(float) * vertexData.size(), 
					BufferUsage::StaticDraw, vb_layout
				);
				IndexBuffer* indexBuffer = IndexBuffer::create_index_buffer(indices);

				std::vector<VertexBuffer<float>*> allVertexBuffers = {vertexBuffer};

				// Then all custom shit.. instanced buffers, skinning stuff.. etc, depending on what kind of mesh this is..
				if (!hasBones)
				{
					return Mesh::create_mesh(allVertexBuffers, indexBuffer, DrawType::Triangles, instanceCount, meshName);
				}
#ifndef DISABLE_SKINNED_MESH_LOADING
				else if (hasBones)
				{
				//.. fancier vertex stuff..
				// Skinning data
					std::vector<int> vertexBoneIds;
					std::vector<float> vertexBoneWeights;
					std::vector<animation::Bone*> bones;

					std::map<std::string, unsigned int> boneMapping;

					if (hasBones)
						load_skinning_data(
							assimpScene, assimpMesh,
							boneMapping,
							bones,
							vertexBoneIds,
							vertexBoneWeights,
							meshBaseIndex
						);

					// Assemble the skeleton..
					aiNode* assimpRootBone = assimpScene->mRootNode;
					animation::Bone* myRootBone = bones[boneMapping[std::string(assimpRootBone->mName.data)]];

					// Find out the total keyframe count of the animation
					unsigned int keyframeCount = 0;
					for (unsigned int i = 0; i < assimpScene->mAnimations[0]->mNumChannels; i++)
						keyframeCount = std::max(keyframeCount, assimpScene->mAnimations[0]->mChannels[i]->mNumPositionKeys);

					// We need to create root bone's keyframes separately!
					create_bone_keyframes(myRootBone, assimpScene, assimpRootBone, keyframeCount);

					animation::Skeleton* skeleton = new animation::Skeleton();
					skeleton->setRootBone(myRootBone);
					skeleton->setBones(bones);
					skeleton->setBoneMapping(boneMapping);

					assemble_bones(assimpScene, assimpRootBone, boneMapping, keyframeCount, *skeleton, bones);


					// For skinned meshes this setup is a bit different.. Currently skinned meshes cannot be instanced!
					VertexBuffer* vb_vertexBoneIds = VertexBuffer::create(&vertexBoneIds[0], vertexBoneIds.size(), BufferUsage::StaticDraw);
					VertexBuffer* vb_vertexBoneWeights = VertexBuffer::create(&vertexBoneWeights[0], vertexBoneWeights.size(), BufferUsage::StaticDraw);

					VertexBufferLayout vb_vertexBoneIdsLayout({ { ShaderDataType::Int4, "boneIDs" } });
					VertexBufferLayout vb_vertexBoneWeightsLayout({ { ShaderDataType::Float4, "boneWeights" } });

					vb_vertexBoneIds->setLayout(vb_vertexBoneIdsLayout);
					vb_vertexBoneWeights->setLayout(vb_vertexBoneWeightsLayout);

					Mesh* skinnedMesh = Mesh::create(ib, { vb, vb_vertexBoneIds, vb_vertexBoneWeights }, PrimitiveDrawType::Triangles);
					materialToUse->setShader(Shader::get_default_skinned_mesh_shader()); // Change default shader to skinned mesh shader!
					skinnedMesh->setMaterial(materialToUse);
					skinnedMesh->setSkeletonPrototype(skeleton);

					return skinnedMesh;
				}
#endif
			}


			// Small helper function that gives us name of an image file (removes all \ and / and other path stuff from it)
			static std::string get_file_name(const std::string& path)
			{
				//		c/images/textures/(NAME START POS)->Test.png
				int nameStartPos = 0;
				for (int i = 0; i < path.length(); i++)
				{
					if (path[i] == '\\' || path[i] == '/')
						nameStartPos = i + 1;
				}

				std::string name;
				for (int i = nameStartPos; i < path.length(); i++)
				{
					if (path[i] != '.')
						name += path[i];
					else
						break;
				}

				return name;
			}

			// Loads texture ment for material's "textureType" slot
			// *Note: Since this creates instance of image data as well, we destroy it here immediately after we have created the texture out of it
			static Texture* load_material_texture(aiMaterial* assimpMaterial, aiTextureType textureType)
			{
				aiString pathToImg;
				assimpMaterial->GetTexture(textureType, 0, &pathToImg); // theres 0 since we currently support only 1 texture per vertex
				if (pathToImg.length == 0)
					return nullptr;

				// Figure out name "extension" for creating a texture and image resource
				std::string nameExtension;
				switch (textureType)
				{
				case aiTextureType::aiTextureType_DIFFUSE:	nameExtension = "_diffuse"; break;
				case aiTextureType::aiTextureType_SPECULAR: nameExtension = "_specular"; break;
				case aiTextureType::aiTextureType_NORMALS:	nameExtension = "_normal"; break;
				case aiTextureType::aiTextureType_HEIGHT:	nameExtension = "_height"; break;
				default:
					break;
				}

				std::string imgName = get_file_name(pathToImg.C_Str());
				std::string resourceName = imgName + nameExtension;
				
				std::string finalImagePath = MODEL_LOADING__RESOURCE_FOLDER__TEXTURE + std::string(pathToImg.C_Str());
				ImageData* imgData = ImageData::load_image(finalImagePath);
				if (!imgData)
				{
					Debug::log(
						"Location: static std::pair<ImageData*, Texture*> load_material_texture(aiMaterial* assimpMaterial, aiTextureType textureType)\n"
						"Failed to load image from: " + finalImagePath,
						DEBUG__ERROR_LEVEL__ERROR
					);
				}
				Texture* texture = Texture::create_texture(imgData, imgData->getWidth(), imgData->getHeight());
				delete imgData;
				return texture;
			}


			// just quick fix so the same material cannot get loaded multiple times..
			static std::vector<std::string> g_s_loaded_materials; // *->TEMP

			// Creates material according to the aiMesh's material
			// Loads the material's textures specified by the aiMesh's material
			// *NOTE Currently we can have only one diffuse, specular and normal texture (no possibility of texture blending at the moment..)
			static void load_assimp_material(
				const aiScene* assimpScene, 
				aiMesh* assimpMesh,

				std::shared_ptr<Material>& outMaterial,

				Texture** diffuseTexture,
				Texture** specularTexture,
				Texture** normalTexture
			)
			{
				
				const unsigned int materialIndex = assimpMesh->mMaterialIndex;
				aiMaterial* assimpMaterial = assimpScene->mMaterials[materialIndex];

				// Make sure we don't load the same material multiple times
				std::string materialName = assimpMaterial->GetName().C_Str();
				if (std::find(g_s_loaded_materials.begin(), g_s_loaded_materials.end(), materialName) != g_s_loaded_materials.end())
					return;
				else
					g_s_loaded_materials.push_back(materialName);

				// *These are pairs, since we need a handle to the texture's image data created on the heap, 
				// to be able to refer to it and destroy it later as well.
				(*diffuseTexture) =	load_material_texture(assimpMaterial, aiTextureType_DIFFUSE);
				(*specularTexture) = load_material_texture(assimpMaterial, aiTextureType_SPECULAR);
				(*normalTexture) = load_material_texture(assimpMaterial, aiTextureType_NORMALS);
				if (!(*normalTexture)) // apparently some file formats has the normal map as "heightmap".. dunno.. just in case.. this propably fucks up at some point..
					(*normalTexture) = load_material_texture(assimpMaterial, aiTextureType_HEIGHT);

				outMaterial = Material::create_material__default3D(
					{ *diffuseTexture, *specularTexture, *normalTexture }
				);

				// notify the material, do we have specular and normap maps specified or not?
				outMaterial->setHasNormalMap((*normalTexture) != nullptr);

				if ((*specularTexture))
				{
					outMaterial->setHasSpecularMap(true);

					float specularStrength = 1.0f;
					float specularShininess = 1.0f;
					aiColor4D assimpSpecularColor = aiColor4D(1,1,1,1);

					std::string materialErrorLog_begin =
						"Location: static void load_assimp_material(\n"
						"const aiScene * assimpScene,\n"
						"aiMesh * assimpMesh,\n"
						"Material * outMaterial,\n"
						"\n"
						"Texture * outDiffuseImage = nullptr,\n"
						"Texture * outSpecularImage = nullptr,\n"
						"Texture * outNormalImage = nullptr,\n"
						"\n"
						"Texture * outDiffuseTexture = nullptr,\n"
						"Texture * outSpecularTexture = nullptr,\n"
						"Texture * outNormalTexture = nullptr\n"
						")\n";

					if (aiGetMaterialFloat(assimpMaterial, AI_MATKEY_SHININESS_STRENGTH, &specularStrength) != aiReturn_SUCCESS)
					{
						Debug::log(materialErrorLog_begin +
							"Failed to get material's 'AI_MATKEY_SHININESS_STRENGTH'\n"
							"(*This may happen because all file formats/exporters don't store this data)",
							DEBUG__ERROR_LEVEL__ERROR
						);
					}
					if (aiGetMaterialFloat(assimpMaterial, AI_MATKEY_SHININESS, &specularShininess) != aiReturn_SUCCESS)
					{
						Debug::log(materialErrorLog_begin +
							"Failed to get material's 'AI_MATKEY_SHININESS'\n"
							"(*This may happen because all file formats/exporters don't store this data)",
							DEBUG__ERROR_LEVEL__ERROR
						);
					}
					if (aiGetMaterialColor(assimpMaterial, AI_MATKEY_COLOR_SPECULAR, &assimpSpecularColor) != aiReturn_SUCCESS)
					{
						Debug::log(materialErrorLog_begin +
							"Failed to get material's 'AI_MATKEY_COLOR_SPECULAR'\n"
							"(*This may happen because all file formats/exporters don't store this data)",
							DEBUG__ERROR_LEVEL__ERROR
						);
					}

					outMaterial->setSpecular_strength(specularStrength);
					outMaterial->setSpecular_shininess(specularShininess);
				}
			}


			// handles an assimp node (handles all child nodes as well 
			//	-> if this is called with root node as "assimpNode" handles every single assimp node we have in here..)
			static void process_assimp_node(
				const aiScene* assimpScene, 
				aiNode* assimpNode,
				bool loadTangents, 
				bool loadMaterial,

				unsigned int instanceCount,

				std::vector<std::shared_ptr<Mesh>>& outMeshes,
				std::vector<Texture*>& outTextures,
				std::vector<std::shared_ptr<Material>>& outMaterials
			)
			{
				for (int i = 0; i < assimpNode->mNumMeshes; i++)
				{
					aiMesh* assimpMesh = assimpScene->mMeshes[assimpNode->mMeshes[i]];
					
					std::shared_ptr<Material> material = nullptr;

					if (loadMaterial)
					{
						Texture* diffuseTexture = nullptr;
						Texture* specularTexture = nullptr;
						Texture* normalTexture = nullptr;

						load_assimp_material(assimpScene, assimpMesh,
							material,
							&diffuseTexture,
							&specularTexture,
							&normalTexture
						);

						if (diffuseTexture)		outTextures.push_back(diffuseTexture);
						if (specularTexture)	outTextures.push_back(specularTexture);
						if (normalTexture)		outTextures.push_back(normalTexture);

						if (material) outMaterials.push_back(material);
					}

					outMeshes.push_back(convert_assimp_mesh(assimpScene, assimpMesh, 0, loadTangents, assimpMesh->HasBones(), instanceCount));
				}

				for (int i = 0; i < assimpNode->mNumChildren; i++)
					process_assimp_node(assimpScene, assimpNode->mChildren[i], loadTangents, loadMaterial, instanceCount, outMeshes, outTextures, outMaterials);
			}


			static unsigned int convert_to_assimp_post_process(unsigned int flags)
			{
				unsigned int outFlags = 0;
				if (flags & ModelLoading_PostProcessFlags::FlipUVs)
					outFlags |= aiPostProcessSteps::aiProcess_FlipUVs;
				if (flags & ModelLoading_PostProcessFlags::Triangulate)
					outFlags |= aiPostProcessSteps::aiProcess_Triangulate;
				if (flags & ModelLoading_PostProcessFlags::CalcTangentSpace)
					outFlags |= aiPostProcessSteps::aiProcess_CalcTangentSpace;
				if (flags & ModelLoading_PostProcessFlags::JoinIdenticalVertices)
					outFlags |= aiPostProcessSteps::aiProcess_JoinIdenticalVertices;

				return outFlags;
			}

			void load_model(
				const std::string& path,

				std::vector<std::shared_ptr<components::Mesh>>& outMeshes,
				std::vector<Texture*>& outTextures,
				std::vector<std::shared_ptr<components::Material>>& outMaterials,

				unsigned int postProcessFlags, 
				bool loadMaterialData,
				unsigned int instanceCount
			)
			{
				unsigned int assimpPostProcessFlags = convert_to_assimp_post_process(postProcessFlags);

				Assimp::Importer assimpImporter;
				const aiScene* assimpScene = nullptr;
				assimpScene = assimpImporter.ReadFile(
					path,
					assimpPostProcessFlags
				);
				if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
				{
					Debug::log(
						"Location: void load_model(\n"
						"const std::string & path,\n"
						"\n"
						"std::vector<components::Mesh*>&outMeshes,\n"
						"std::vector<std::pair<ImageData*, Texture*>>&outImgTexPairs,\n"
						"std::vector<components::Material*>&outMaterials,\n"
						"\n"
						"unsigned int postProcessFlags,\n"
						"bool loadMaterialData\n"
						")\n"
						"Failed to load model from: " + path + "\n"
						"Assimp error message : " + assimpImporter.GetErrorString(),
						DEBUG__ERROR_LEVEL__ERROR
					);
				}

				std::string fileName = get_file_name(path);

				bool loadTangents = assimpPostProcessFlags & aiPostProcessSteps::aiProcess_CalcTangentSpace;
				process_assimp_node(
					assimpScene, 
					assimpScene->mRootNode, 
					loadTangents,
					
					loadMaterialData,
					instanceCount,

					outMeshes, 
					outTextures,
					outMaterials
				);
				//assimpImporter.FreeScene(); // unnecessary, because its called by Importer's destructor?

				g_s_loaded_materials.clear();
			}
		}
	}
}
