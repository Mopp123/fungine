
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

		Mesh::Mesh(IndexBuffer* indexBuffer, DrawType drawType, unsigned int instanceCount, const std::string& name, bool isStatic) :
			Component(name, isStatic), _instanceCount(instanceCount), _indexBuffer(indexBuffer), _drawType(drawType)
		{}

		Mesh::~Mesh()
		{
			Debug::notify_on_destroy(_name + "(Mesh)");

			for (VertexBuffer<float>* vb : _vertexBuffers)
				delete vb;

			delete _indexBuffer;
		}

		const size_t Mesh::getSize() const
		{
			return sizeof(*this);
		}


		std::shared_ptr<Mesh> Mesh::create_mesh(std::vector<VertexBuffer<float>*> vertexBuffers, IndexBuffer* indexBuffer, DrawType drawType, unsigned int instanceCount, const std::string& name, bool isStatic)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return std::make_shared<opengl::OpenglMesh>(vertexBuffers, indexBuffer, drawType, instanceCount, name, isStatic);
			default:
				Debug::log(
					"Location: Mesh* Mesh::create_mesh(std::vector<VertexBuffer*> vertexBuffers, IndexBuffer* indexBuffer)\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}

	}
}