#pragma once

#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"
#include "utils/imgUtils/Image.h"
#include <string>
#include <memory>

namespace fungine
{
	namespace graphics
	{

		namespace modelLoading
		{
			enum ModelLoading_PostProcessFlags
			{
				None = 0x0,
				Triangulate = 0x1,
				FlipUVs = 0x2,
				CalcTangentSpace = 0x4,
				JoinIdenticalVertices = 0x8
			};


			/*
				*NOTE Sometimes you need to flip uvs for textures to be correctly..

				* If "loadMaterialData" == true
					- For each mesh this model has...
						- We load all textures this mesh has stored as its' diffuse, specular and normal maps.
						We load all material data of this mesh and create a new "Material resource" for it.
			*/
			void load_model(
				const std::string& path, 

				std::vector<std::shared_ptr<components::Mesh>>& outMeshes,
				std::vector<Texture*>& outTextures,
				std::vector< std::shared_ptr<components::Material>>& outMaterials,

				unsigned int postProcessFlags, 
				bool loadMaterialData
			);
		}
	}
}