#pragma once

#include "components/rendering/Mesh.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglMesh : public components::Mesh
			{
			public:

				OpenglMesh(std::vector<VertexBuffer<float>*> vertexBuffers, IndexBuffer* indexBuffer, graphics::DrawType drawType, unsigned int instanceCount, const std::string& name = "", bool isStatic = false);
				~OpenglMesh();

				virtual void addVertexBuffer(graphics::VertexBuffer<float>* buffer) override;

				inline const unsigned int& getVaoID() const { return _vaoID; }
			};
		}
	}
}