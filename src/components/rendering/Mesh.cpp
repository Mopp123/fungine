
#include "Mesh.h"
#include "entities/Entity.h"
#include "graphics/Graphics.h"
#include "graphics/RendererCommands.h"
#include "graphics/opengl/OpenglMesh.h"

#include "core/Debug.h"


namespace fungine
{
	using namespace graphics;

	namespace components
	{

		Mesh::Mesh(std::vector<VertexBuffer*> vertexBuffers, IndexBuffer* indexBuffer, DrawType drawType, entities::Entity* entity) :
			Component(entity), _vertexBuffers(vertexBuffers), _indexBuffer(indexBuffer), _drawType(drawType)
		{}

		Mesh::~Mesh()
		{
			for (VertexBuffer* vb : _vertexBuffers)
				delete vb;

			delete _indexBuffer;
		}

		const size_t Mesh::getSize() const
		{
			return sizeof(*this);
		}


		std::shared_ptr<Mesh> Mesh::create_mesh(std::vector<VertexBuffer*> vertexBuffers, IndexBuffer* indexBuffer, DrawType drawType)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return std::make_shared<opengl::OpenglMesh>(vertexBuffers, indexBuffer, drawType);
			default:
				Debug::log(
					"Mesh* Mesh::create_mesh(std::vector<VertexBuffer*> vertexBuffers, IndexBuffer* indexBuffer)\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}
	}
}