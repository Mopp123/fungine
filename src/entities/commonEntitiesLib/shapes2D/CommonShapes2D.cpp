
#include "CommonShapes2D.h"
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
			Entity* create_entity__Plane2D(const mml::Vector2& pos, const mml::Vector2& scale, unsigned int instanceCount)
			{
				// *Offset is made to be top left corner!
				// *Coordinates in 2d: top left is 0,0 bottom right is windowWidth, windowHeight (thats why theres no -1 on y coord here!!!)
				float vertices[8] =
				{
					0, 0,
					0, 1.0f,
					1.0f, 1.0f,
					1.0f, 0
				};

				std::vector<unsigned int> indices =
				{
					0,1,3,2
				};

				VertexBufferLayout vbLayout({
					{ 0, ShaderDataType::Float2 }
				});

				VertexBuffer<float>* vb = VertexBuffer<float>::create_vertex_buffer(
					vertices,
					sizeof(float) * 8,
					BufferUsage::StaticDraw,
					vbLayout
				);

				IndexBuffer* ib = IndexBuffer::create_index_buffer(indices);

				std::shared_ptr<Mesh> mesh = Mesh::create_mesh({ vb }, ib, DrawType::TriangleStrips, instanceCount);

				Entity* entity = new Entity;
				entity->addComponent(std::make_shared<Transform>(mml::Vector3(pos.x, pos.y, 0), mml::Quaternion({0,1,0}, 0 ), mml::Vector3(scale.x, scale.y, 1.0f)));
				entity->addComponent(mesh);
				return entity;
			}
		}
	}
}