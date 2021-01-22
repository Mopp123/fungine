#pragma once

#include "components/Component.h"
#include "graphics/Buffers.h"
#include <memory>
#include <string>

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

			std::vector<graphics::VertexBuffer<float>*> _vertexBuffers;
			graphics::IndexBuffer* _indexBuffer = nullptr;
			graphics::DrawType _drawType;

			int _instanceCount = 1;
			bool _isStatic = false;

			bool _enableShadows = true;

			// This is kind of a "necessary evil" when using opengl.. 
			// We dont want to downcast a Mesh into OpenglMesh in the future, so we need to know this vaoID somewhere.
			// Direct3d for example doesn't have vertex array objects, so this kinds of graphics apis just have this hanging uselessly here..
			unsigned int _vaoID = 0;


		public:
			Mesh(graphics::IndexBuffer* indexBuffer, graphics::DrawType drawType, unsigned int instanceCount = 1, const std::string& name = "", bool isStatic = false);
			virtual ~Mesh();
			virtual const size_t getSize() const override;

			virtual void addVertexBuffer(graphics::VertexBuffer<float>* buffer) {}

			static std::shared_ptr<Mesh> create_mesh(std::vector<graphics::VertexBuffer<float>*> vertexBuffers, graphics::IndexBuffer* indexBuffer, graphics::DrawType drawType, unsigned int instanceCount = 1, const std::string& name = "", bool isStatic = false);
			static std::shared_ptr<Mesh> create_copy(const std::shared_ptr<Mesh>& m);

			inline const std::vector<graphics::VertexBuffer<float>*>& getVertexBuffers() const { return _vertexBuffers; }
			inline graphics::IndexBuffer* getIndexBuffer() { return _indexBuffer; }
			inline const graphics::IndexBuffer* getIndexBuffer() const { return _indexBuffer; }

			inline void enableShadows(bool arg) { _enableShadows = arg; }

			inline const graphics::DrawType& getDrawType() const { return _drawType; }
			inline bool isStatic() const { return _isStatic; }
			inline bool hasShadows() const { return _enableShadows; }
			inline const int& getInstanceCount() const { return _instanceCount; }
			inline const unsigned int& getAPISpecific_vaoID() const { return _vaoID; }
		};
	}
}