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

				OpenglMesh(std::vector<VertexBuffer*> vertexBuffers, IndexBuffer* indexBuffer, graphics::DrawType drawType, const std::string& name = "");
				~OpenglMesh();

				inline const unsigned int& getVaoID() const { return _vaoID; }
			};
		}
	}
}