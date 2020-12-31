
#include "TerrainRenderer.h"
#include "core/window/Window.h"
#include "graphics/Graphics.h"
#include "graphics/RendererCommands.h"

#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"

#include "core/Debug.h"

namespace fungine
{
	using namespace entities;
	using namespace graphics;

	namespace components
	{
		namespace rendering
		{

			TerrainRenderer::TerrainRenderer()
			{
				if (!s_framebuffer)
					s_framebuffer = Framebuffer::create_framebuffer(core::Window::get_width(), core::Window::get_height(), true);
			}

			TerrainRenderer::~TerrainRenderer()
			{}


			void TerrainRenderer::submit(Entity* e)
			{
				_renderList.push_back(e);
			}

			void TerrainRenderer::render(
				const mml::Matrix4& projectionMatrix,
				const mml::Matrix4& viewMatrix,
				unsigned int renderFlags
			)
			{
				const RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				Camera* camera = Camera::get_current_camera();
				DirectionalLight* directionalLight = DirectionalLight::get_directional_light();

				rendererCommands->cullFace(CullFace::Back);

				bool renderGeometry = true;
				bool renderMaterial = renderFlags & RenderFlags::RenderMaterial;
				bool renderLighting = renderFlags & RenderFlags::RenderLighting;
				bool renderShadows =  renderFlags & RenderFlags::RenderShadows;

#ifdef DEBUG__MODE_FULL
				if (!camera)
				{
					Debug::log(
						"Location: void TerrainRenderer::flush(\n"
						"const mml::Matrix4 & projectionMatrix,\n"
						"const mml::Matrix4 & viewMatrix,\n"
						"unsigned int renderFlags,\n"
						"RenderPass renderPass\n"
						")\n"
						"Tried to flush renderer, but the current camera was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return;
				}
#endif

				for (Entity* e : _renderList)
				{
					Material* material = e->getComponent<Material>().get();
					ShaderProgram* shader = material->getShader();
					Mesh* mesh = e->getComponent<Mesh>().get();
					
#ifdef DEBUG__MODE_FULL
					if (!mesh)
					{
						Debug::log(
							"Location: void TerrainRenderer::flush(\n"
							"const mml::Matrix4 & projectionMatrix,\n"
							"const mml::Matrix4 & viewMatrix,\n"
							"unsigned int renderFlags,\n"
							"RenderPass renderPass\n"
							")\n"
							"Mesh of a batch was nullptr!",
							DEBUG__ERROR_LEVEL__ERROR
						);
						continue;
					}
#endif
					// Either bind the material or just plain shader
					// (RendererCommands::bindMaterial(Material*) also binds the material's shader!)
					rendererCommands->bindShader(shader);
					if(renderMaterial)
						rendererCommands->bindMaterial(material);

					// common uniforms
					shader->setUniform("projectionMatrix", projectionMatrix);
					shader->setUniform("viewMatrix", viewMatrix);
					shader->setUniform("cameraPos", camera->getEntity()->getComponent<Transform>()->getPosition());

					if (renderLighting)
					{
						shader->setUniform("directionalLight_direction", directionalLight->getDirection());
						shader->setUniform("directionalLight_ambientColor", directionalLight->getAmbientColor());
						shader->setUniform("directionalLight_color", directionalLight->getColor());
					}
					if (renderShadows)
					{
						const ShadowCaster& shadowCaster = directionalLight->getShadowCaster();

						shader->setUniform("shadowProjMat", shadowCaster.getProjectionMatrix());
						shader->setUniform("shadowViewMat", shadowCaster.getViewMatrix());

						shader->setUniform("shadowProperties.shadowmapWidth", shadowCaster.getFramebuffer()->getWidth());
						shader->setUniform("shadowProperties.pcfCount", 1);

						shader->setUniform("texture_shadowmap", 14);

						rendererCommands->bindTexture(shadowCaster.getShadowmapTexture(), 14);
					}

					// per entity uniforms
					shader->setUniform("transformationMatrix", e->getComponent<Transform>()->getTransformationMatrix());

					// Finally drawing..
					rendererCommands->bindMesh(mesh);
					rendererCommands->drawIndices(mesh);
					rendererCommands->unbindMesh(mesh);
					
					// Unbind the batch's shit..
					if(renderMaterial) 
						rendererCommands->unbindMaterial(material);
					
					rendererCommands->unbindShader();
				}
			}

			void TerrainRenderer::clear()
			{
				_renderList.clear();
			}

			const size_t TerrainRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}