#pragma once

#include "utils/myMathLib/MyMathLib.h" 
#include "Framebuffer.h"

#include "graphics/shaders/ShaderProgram.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"

namespace fungine
{
	namespace graphics
	{
		enum class DrawType
		{
			Triangles,
			TriangleStrips,
			Lines,
			LineLoop,
			Points
		};

		enum class CullFace
		{
			None,
			Front,
			Back
		};

		class RendererCommands
		{
		protected:
			mml::Vector4 _clearColor;

			// We allow RenderCommands creation only through the function "create_render_commands".
			//	-> make operator new inaccessable for outsiders
			void* operator new(size_t);

		public:

			RendererCommands() : _clearColor({ 0,0,0,0 }) {}
			virtual ~RendererCommands() {}

			static RendererCommands* create_renderer_commands();

			virtual void init() const = 0;
			virtual void clear() const = 0;

			virtual void bindShader(ShaderProgram* shader) const = 0;
			virtual void unbindShader() const = 0; // kind of pointless?
			virtual void cullFace(CullFace face) const = 0;
			virtual void bindTexture(const Texture* const texture, unsigned int slot) const = 0;
			virtual void unbindTexture(const Texture* const texture, unsigned int slot) const = 0;
			virtual void bindMesh(const components::Mesh* const mesh) const = 0;
			virtual void unbindMesh(const components::Mesh* const mesh) const = 0;
			virtual void drawIndices(const components::Mesh* const mesh) const = 0;
			virtual void drawIndices_instanced(const components::Mesh* const mesh) const = 0;
			virtual void drawIndices_instanced(const components::Mesh* const mesh, unsigned int instanceCount) const = 0;

			virtual void bindMaterial(components::Material* const material) const = 0;
			virtual void unbindMaterial(const components::Material* const material) const = 0;

			virtual void bindFramebuffer(const Framebuffer* const framebuffer) const = 0;
			// *Note: 
			//		If inputted framebuffer was multisampled, this function also resolves the multisampled texture into regular texture!
			virtual void unbindFramebuffer(const Framebuffer* const framebuffer) const = 0;


			inline void setClearColor(const mml::Vector4& color) { _clearColor = color; }
			inline const mml::Vector4& getClearColor() const { return _clearColor; }
		};
	}
}