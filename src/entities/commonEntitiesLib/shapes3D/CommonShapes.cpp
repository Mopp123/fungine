
#include "CommonShapes.h"
#include "components/common/Transform.h"
#include "graphics/Graphics.h"
#include "graphics/Buffers.h"
#include "components/rendering/Mesh.h"

namespace fungine
{
	using namespace components;
	using namespace graphics;

	namespace entities
	{
		namespace commonEntityFactory
		{
			Entity* create_entity__Plane(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Vector3& scale)
			{
				float vertices[12] =
				{
					-1.0f, 1.0f, 0.0f,
					-1.0f, -1.0f, 0.0f,
					1.0f, -1.0f, 0.0f,
					1.0f, 1.0f, 0.0f
				};

				std::vector<unsigned int> indices =
				{
					0,1,3,2
				};


				VertexBufferLayout vbLayout({
					{ 0, ShaderDataType::Float3 }
				});

				VertexBuffer<float>* vb = VertexBuffer<float>::create_vertex_buffer(
					vertices,
					sizeof(float) * 12,
					BufferUsage::StaticDraw,
					vbLayout
				);

				IndexBuffer* ib = IndexBuffer::create_index_buffer(indices);

				std::shared_ptr<Mesh> mesh = Mesh::create_mesh({ vb }, ib, DrawType::TriangleStrips);

				
				Entity* entity = new Entity;
				entity->addComponent(std::make_shared<Transform>(pos, rot, scale));
				entity->addComponent(mesh);
				return entity;
			}
		}
	}
}