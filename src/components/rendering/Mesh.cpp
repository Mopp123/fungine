
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
			for (VertexBuffer<float>* vb : _vertexBuffers)
				delete vb;
			
			_vertexBuffers.clear();

			delete _indexBuffer;

			Debug::notify_on_destroy(_name + "(Mesh)");
		}

		const size_t Mesh::getSize() const
		{
			return sizeof(*this);
		}


		std::shared_ptr<Mesh> Mesh::create_mesh(std::vector<VertexBuffer<float>*> vertexBuffers, IndexBuffer* indexBuffer, DrawType drawType, unsigned int instanceCount, const std::string& name, bool isStatic)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: 
				return std::make_shared<opengl::OpenglMesh>(vertexBuffers, indexBuffer, drawType, instanceCount, name, isStatic);
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

		std::shared_ptr<Mesh> Mesh::create_copy(const std::shared_ptr<Mesh>& m)
		{
#ifdef DEBUG__MODE_FULL
			if (m->_indexBuffer->getData().empty())
			{
				Debug::log(
					"Location: std::shared_ptr<Mesh> Mesh::create_copy(const Mesh& m)\n"
					"Failed to copy mesh. Original mesh's index buffer's data was empty!",
					DEBUG__ERROR_LEVEL__ERROR
				);
				return nullptr;
			}
#endif

			// Copy vertex and index buffers from "m"
			IndexBuffer* copiedIndexBuffer;
			std::vector<VertexBuffer<float>*> copiedVertexBuffers;

			copiedIndexBuffer = IndexBuffer::create_index_buffer(m->_indexBuffer->getData());

			for (VertexBuffer<float>* b : m->_vertexBuffers)
			{
				const float* b_data = b->getData();
#ifdef DEBUG__MODE_FULL
				if (!b_data)
				{
					Debug::log(
						"Location: std::shared_ptr<Mesh> Mesh::create_copy(const Mesh& m)\n"
						"Failed to copy mesh. Original mesh's vertex buffer's data was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return nullptr;
				}
#endif
				copiedVertexBuffers.push_back(VertexBuffer<float>::create_vertex_buffer(const_cast<float*>(b_data), b->getDataSize(), b->getUsage(), b->getLayout()));
			}

			return create_mesh(copiedVertexBuffers, copiedIndexBuffer, m->_drawType, m->_instanceCount, m->_name, m->_isStatic);
		}
	}
}