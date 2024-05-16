
#include "Buffers.h"
#include "graphics/Graphics.h"
#include "core/Debug.h"

#include "opengl/OpenglBuffers.h"

namespace fungine
{
	namespace graphics
	{
		// instantiate templates for buffers explicitly
		template class VertexBuffer<float>;

		template<typename T>
		VertexBuffer<T>::VertexBuffer(T* data, size_t dataSize, BufferUsage usage, const VertexBufferLayout& layout) :
			_dataSize(dataSize), _layout(layout), _usage(usage)
		{
			_data = new T[_dataSize];
			memcpy(_data, data, _dataSize);
		}

		template<typename T>
		VertexBuffer<T>::~VertexBuffer()
		{
			if (_data)
			{
				delete[] _data;
				_data = nullptr;
			}
		}

		template<typename T>
		VertexBuffer<T>* VertexBuffer<T>::create_vertex_buffer(T* data, size_t dataSize, BufferUsage usage, const VertexBufferLayout& layout)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglVertexBuffer<T>(data, dataSize, usage, layout);
			default:
				Debug::log(
					"Location: VertexBuffer* VertexBuffer::create_vertex_buffer(const std::vector<float>& data, BufferUsage usage)\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}

		IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices) :
			_data(indices)
		{}

		IndexBuffer::~IndexBuffer()
		{}

		IndexBuffer* IndexBuffer::create_index_buffer(const std::vector<unsigned int>& indices)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglIndexBuffer(indices);
			default:
				Debug::log(
					"Location: IndexBuffer* IndexBuffer::create_index_buffer(const std::vector<unsigned int>& indices)\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}



		VertexBufferLayout::VertexBufferLayout(const std::vector<VertexBufferElement>& attribs)
		{
			for (const VertexBufferElement& vbElem : attribs)
			{
				/*size_t elemSize = 0;
				get_shader_data_type_size(vbElem.getShaderDataType(), elemSize);
				_stride += elemSize;*/

				add(vbElem); // problems?? not tested!
			}
		}

		VertexBufferLayout::~VertexBufferLayout()
		{}

		void VertexBufferLayout::add(const VertexBufferElement& element)
		{
			_elements.push_back(element);
			size_t elemSize = 0;
			get_shader_data_type_size(element.getShaderDataType(), elemSize);
			_stride += elemSize;
		}
	}
}
