#pragma once

#include "components/Component.h"
#include "graphics/Buffers.h"
#include <memory>

namespace fungine
{
	namespace graphics
	{
		enum class DrawType;
	}

	namespace components
	{
		class Mesh : public Component
		{
		protected:

			std::vector<graphics::VertexBuffer*> _vertexBuffers;
			graphics::IndexBuffer* _indexBuffer;
			graphics::DrawType _drawType;

			int _instanceCount = 1;

			// This is kind of a "necessary evil" when using opengl.. 
			// We dont want to downcast a Mesh into OpenglMesh in the future, so we need to know this vaoID somewhere.
			// Direct3d for example doesn't have vertex array objects, so this kinds of graphics apis just have this hanging uselessly here..
			unsigned int _vaoID = 0;

		public:

			Mesh(std::vector<graphics::VertexBuffer*> vertexBuffers, graphics::IndexBuffer* indexBuffer, graphics::DrawType drawType, entities::Entity* entity = nullptr);
			virtual ~Mesh();
			virtual const size_t getSize() const override;

			static std::shared_ptr<Mesh> create_mesh(std::vector<graphics::VertexBuffer*> vertexBuffers, graphics::IndexBuffer* indexBuffer, graphics::DrawType drawType);

			inline const std::vector<graphics::VertexBuffer*>& getVertexBuffers() const { return _vertexBuffers; }
			inline const graphics::IndexBuffer* getIndexBuffer() const { return _indexBuffer; }

			inline const graphics::DrawType& getDrawType() const { return _drawType; }
			inline const int& getInstanceCount() const { return _instanceCount; }
			inline const unsigned int& getAPISpecific_vaoID() const { return _vaoID; }
		};
	}
}