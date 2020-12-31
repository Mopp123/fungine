#pragma once

#include "shaders/ShaderProgram.h"
#include <vector>
#include <memory.h>

#define get_shader_data_type_component_count(dataType, storage) switch(dataType){\
case graphics::ShaderDataType::Float :	storage = 1;	break;	\
case graphics::ShaderDataType::Float2 :	storage = 2;	break;	\
case graphics::ShaderDataType::Float3 :	storage = 3;	break;	\
case graphics::ShaderDataType::Float4 :	storage = 4;	break;	\
case graphics::ShaderDataType::Int :	storage = 1;	break;	\
case graphics::ShaderDataType::Int2 :	storage = 2;	break;	\
case graphics::ShaderDataType::Int3 :	storage = 3;	break;	\
case graphics::ShaderDataType::Int4 :	storage = 4;	break;	\
default :								storage = 0;	break;	\
}

// Returns shader data type's size in bytes
#define get_shader_data_type_size(dataType, storage) switch(dataType){			\
case graphics::ShaderDataType::Float :	storage = sizeof(float);		break;	\
case graphics::ShaderDataType::Float2 :	storage = sizeof(float) * 2;	break;	\
case graphics::ShaderDataType::Float3 :	storage = sizeof(float) * 3;	break;	\
case graphics::ShaderDataType::Float4 :	storage = sizeof(float) * 4;	break;	\
case graphics::ShaderDataType::Int :	storage = sizeof(int);			break;	\
case graphics::ShaderDataType::Int2 :	storage = sizeof(int) * 2;		break;	\
case graphics::ShaderDataType::Int3 :	storage = sizeof(int) * 3;		break;	\
case graphics::ShaderDataType::Int4 :	storage = sizeof(int) * 4;		break;	\
default :								storage = 0;					break;	\
}


namespace fungine
{
	namespace graphics
	{
		enum class BufferUsage
		{
			StaticDraw,
			StreamDraw,
			DynamicDraw
		};

		struct VertexBufferElement
		{
		private:
			unsigned int _slot = 0;
			ShaderDataType _shaderDataType;
			bool _instanced = false;

		public:
			VertexBufferElement(unsigned int slot, ShaderDataType dataType, bool instanced = false) :
				_slot(slot), _shaderDataType(dataType), _instanced(instanced)
			{}

			virtual ~VertexBufferElement() {}
			inline unsigned int getSlot() const { return _slot; }
			inline ShaderDataType getShaderDataType() const { return _shaderDataType; }
			inline bool isInstanced() const { return _instanced; }
		};

		class VertexBufferLayout
		{
		private:
			std::vector<VertexBufferElement> _elements;
			unsigned int _stride = 0;

		public:
			VertexBufferLayout() {}
			VertexBufferLayout(const std::vector<VertexBufferElement>& attribs);
			~VertexBufferLayout();

			void add(const VertexBufferElement& element);

			inline const std::vector<VertexBufferElement>& getElements() const { return _elements; }
			inline const unsigned int& getStride() const { return _stride; }
		};


		
		template<typename T>
		class VertexBuffer
		{
		protected:
			unsigned int _id = 0;
			T* _data;
			size_t _dataSize;
			VertexBufferLayout _layout;

			// We allow VertexBuffer creation only through the function "create_vertex_buffer".
			//	-> make operator new inaccessable for outsiders
			void* operator new(size_t);

		public:
			VertexBuffer(T* data, size_t dataSize, BufferUsage usage, const VertexBufferLayout& layout);
			virtual ~VertexBuffer();

			static VertexBuffer* create_vertex_buffer(T* data, size_t dataSize, BufferUsage usage, const VertexBufferLayout& layout);

			// Updates the buffer starting from "offset" with new data
			// Mesh which owns this buffer, must be bound when calling this.
			virtual void update(int offset, size_t dataSize, const void* data) = 0;

			inline const unsigned int& getID() const { return _id; }
			inline const T* getData() const { return  _data; }
			inline const VertexBufferLayout& getLayout() const { return  _layout; }
		};

		class IndexBuffer
		{
		protected:
			unsigned int _id = 0;
			std::vector<unsigned int> _data;

		public:
			IndexBuffer(const std::vector<unsigned int>& indices);
			virtual ~IndexBuffer();

			static IndexBuffer* create_index_buffer(const std::vector<unsigned int>& indices);

			inline const unsigned int& getID() const { return _id; }
			inline const std::vector<unsigned int>& getData() const { return  _data; }
			inline const size_t getCount() const { return _data.size(); }
		};
	}
}