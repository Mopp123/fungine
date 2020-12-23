
#include "Terrain.h"
#include "graphics/Graphics.h"

namespace fungine
{
	using namespace graphics;
	using namespace components;

	namespace entities
	{

		Terrain::Terrain(
			float tileWidth,
			ImageData* heightmapImage,
			graphics::Texture* blendmapTexture,
			graphics::Texture* black_diffuse, graphics::Texture* black_specular, graphics::Texture* black_normal,
			graphics::Texture* red_diffuse, graphics::Texture* red_specular, graphics::Texture* red_normal,
			graphics::Texture* green_diffuse, graphics::Texture* green_specular, graphics::Texture* green_normal,
			graphics::Texture* blue_diffuse, graphics::Texture* blue_specular, graphics::Texture* blue_normal,
			const std::shared_ptr<components::rendering::Renderer>& rendererToUse,
			graphics::ShaderProgram* shaderToUse
		) :
			_tileWidth(tileWidth)
		{

			_material = Material::create_material__default3D(
				shaderToUse,
				{
					{"texture_blendmap", blendmapTexture},

					{"texture_black_diffuse", black_diffuse },	{"texture_black_specular", black_specular },{"texture_black_normal", black_normal },
					{"texture_red_diffuse", red_diffuse },		{"texture_red_specular", red_specular },	{"texture_red_normal", red_normal},
					{"texture_green_diffuse", green_diffuse },	{"texture_green_specular", green_specular },{"texture_green_normal", green_normal},
					{"texture_blue_diffuse", blue_diffuse },	{"texture_blue_specular", blue_specular },	{"texture_blue_normal", blue_normal}
				}
			);

			_verticesPerRow = heightmapImage->getWidth();

			for (unsigned int y = 0; y < heightmapImage->getHeight(); ++y)
			{
				for (unsigned int x = 0; x < heightmapImage->getWidth(); ++x)
				{
					float height = (float)(heightmapImage->getColorChannel(x, y, 0)) / 255.0f * _heightModifier;
					_heightmap.push_back(height);
				}
			}

			generateMesh();

			_transform = std::make_shared<Transform>(mml::Vector3(0, 0, 0), mml::Quaternion({0,1,0}, 0), mml::Vector3(1,1,1));
			addComponent(_transform);
			addComponent(_material);
			addComponent(_mesh);
			addComponent(rendererToUse);
		}

		Terrain::~Terrain()
		{
		}


		void Terrain::generateMesh()
		{
			size_t vertexFloatCount = 3 + 2 + 3 + 3; // how many floats per vertex (pos + uv + normal + tangent)
			std::vector<float> vertexData;
			std::vector<mml::Vector3> positions;
			std::vector<mml::Vector2> uvs;
			std::vector<mml::Vector3> normals;
			std::vector<mml::Vector3> tangents;

			for (int z = 0; z < _verticesPerRow; ++z)
			{
				for (int x = 0; x < _verticesPerRow; ++x)
				{
					float height = _heightmap[x + z * _verticesPerRow];
					float vertexPos_x = x * _tileWidth;
					float vertexPos_z = z * _tileWidth;
					positions.push_back({ vertexPos_x, height, vertexPos_z });
					uvs.push_back({vertexPos_x / (_verticesPerRow * _tileWidth), vertexPos_z / (_verticesPerRow * _tileWidth)});
					
					// Calc normal
					float left = 0;
					float right = 0;
					float down = 0;
					float up = 0;

					int heightmapX = x;
					int heightmapY = z;

					if (heightmapX - 1 >= 0)
						left = _heightmap[(heightmapX - 1) + heightmapY * _verticesPerRow];

					if (heightmapX + 1 < _verticesPerRow)
						right = _heightmap[(heightmapX + 1) + heightmapY * _verticesPerRow];

					if (heightmapY + 1 < _verticesPerRow)
						up = _heightmap[heightmapX + (heightmapY + 1) * _verticesPerRow];

					if (heightmapY - 1 >= 0)
						down = _heightmap[heightmapX + (heightmapY - 1) * _verticesPerRow];

					
					mml::Vector3 normal((left - right), _heightModifier * 0.1f, (down - up)); // this is fucking dumb...
					normal.normalize();
					normals.push_back(normal);
				}
			}

			// Figure out tangents..
			for (int z = 0; z < _verticesPerRow; z++)
			{
				for (int x = 0; x < _verticesPerRow; x++)
				{
					mml::Vector3 vertex = positions[x + z * _verticesPerRow];
					mml::Vector3 vertRight(0, 0, 0);
					if(x + 1 < _verticesPerRow)
						vertRight = positions[(x + 1) + z * _verticesPerRow];

					mml::Vector3 tangent(1, 0, 0);
					if (vertRight.magnitude() > 0.0f)
						tangent = vertRight - vertex;

					tangent.normalize();
					tangents.push_back(tangent);
				}
			}

			// Combine all into one big fat vertex buffer..
			for (int i = 0; i < _verticesPerRow * _verticesPerRow; ++i)
			{
				vertexData.push_back(positions[i].x);
				vertexData.push_back(positions[i].y);
				vertexData.push_back(positions[i].z);

				vertexData.push_back(uvs[i].x);
				vertexData.push_back(uvs[i].y);

				vertexData.push_back(normals[i].x);
				vertexData.push_back(normals[i].y);
				vertexData.push_back(normals[i].z);

				vertexData.push_back(tangents[i].x);
				vertexData.push_back(tangents[i].y);
				vertexData.push_back(tangents[i].z);
			}
			VertexBufferLayout vbLayout(
				{
					{0, ShaderDataType::Float3},
					{1, ShaderDataType::Float2},
					{2, ShaderDataType::Float3},
					{3, ShaderDataType::Float3}
				}
			);

			VertexBuffer<float>* vb = VertexBuffer<float>::create_vertex_buffer(&vertexData[0], sizeof(float) * vertexData.size(), BufferUsage::StaticDraw, vbLayout);

			// Figure out indices..
			std::vector<unsigned int> indices;

			for (unsigned int z = 0; z < _verticesPerRow - 1; ++z)
			{
				for (unsigned int x = 0; x < _verticesPerRow - 1; ++x)
				{
					unsigned int topRight = (x + 1) + (z + 1) * _verticesPerRow;
					unsigned int topLeft = x + (z + 1) * _verticesPerRow;
					unsigned int bottomLeft = x + z * _verticesPerRow;
					unsigned int bottomRight = (x + 1) + z * _verticesPerRow;

					indices.push_back(topRight);
					indices.push_back(topLeft);
					indices.push_back(bottomLeft);
				
					indices.push_back(bottomLeft);
					indices.push_back(bottomRight);
					indices.push_back(topRight);
					
				}
			}

			IndexBuffer* ib = IndexBuffer::create_index_buffer(indices);

			// And then we can finally create the mesh..
			_mesh = Mesh::create_mesh({ vb }, ib, DrawType::Triangles);
		}

		float Terrain::getHeightAt(const float& x, const float& z)
		{
			// Pos relative to terrain
			const mml::Vector3& worldPos = _transform->getPosition();
			float terrainX = x - worldPos.x;
			float terrainZ = z - worldPos.z;

			// Get the current tile we are standing on..
			int gridX = (int)std::floor(terrainX / _tileWidth);
			int gridZ = (int)std::floor(terrainZ / _tileWidth);

			if (gridX < 0 || gridX + 1 >= _verticesPerRow || gridZ < 0 || gridZ + 1 >= _verticesPerRow)
			{
				return 0.0f;
			}



			// Coordinates in relation to the current tile, in range 0 to 1
			float tileSpaceX = std::fmod(terrainX, _tileWidth) / _tileWidth;
			float tileSpaceZ = std::fmod(terrainZ, _tileWidth) / _tileWidth;

			// Check which triangle of the tile we are standing on..
			if (tileSpaceX <= tileSpaceZ) {
				return mml::get_triangle_height_barycentric(
					mml::Vector3(0, _heightmap[gridX + gridZ * _verticesPerRow], 0),
					mml::Vector3(0, _heightmap[gridX + (gridZ + 1) * _verticesPerRow], 1),
					mml::Vector3(1, _heightmap[(gridX + 1) + (gridZ + 1) * _verticesPerRow], 1),
					mml::Vector2(tileSpaceX, tileSpaceZ));
			}
			else {
				return mml::get_triangle_height_barycentric(
					mml::Vector3(0, _heightmap[gridX + gridZ * _verticesPerRow], 0),
					mml::Vector3(1, _heightmap[(gridX + 1) + (gridZ + 1) * _verticesPerRow], 1),
					mml::Vector3(1, _heightmap[(gridX + 1) + gridZ * _verticesPerRow], 0),
					mml::Vector2(tileSpaceX, tileSpaceZ));
			}

		}

	}
}