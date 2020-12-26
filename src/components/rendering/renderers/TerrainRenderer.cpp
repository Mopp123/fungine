
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

			TerrainRenderer::TerrainRenderer(bool renderShadows) : 
				Renderer(renderShadows)
			{
				if (!s_framebuffer)
					s_framebuffer = Framebuffer::create_framebuffer(core::Window::get_width(), core::Window::get_height(), true);
			}

			TerrainRenderer::~TerrainRenderer()
			{}


			void TerrainRenderer::submit(Entity* e)
			{
				std::shared_ptr<Material> entityMaterial = e->getComponent<Material>();
				for (std::pair<std::shared_ptr<Material>, std::vector<Entity*>>& batch : _batches)
				{
					if (batch.first == entityMaterial)
					{
						batch.second.push_back(e);
						return;
					}
				}

				std::vector<Entity*> newBatch;
				newBatch.push_back(e);
				_batches.push_back(std::make_pair(entityMaterial, newBatch));
			}

			void TerrainRenderer::flush(
				const mml::Matrix4& projectionMatrix,
				const mml::Matrix4& viewMatrix,
				unsigned int renderFlags
			)
			{
				const RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				Camera* camera = Camera::get_current_camera();
				DirectionalLight* directionalLight = DirectionalLight::get_directional_light();

				bool renderGeometry = true;
				bool renderMaterial = renderFlags & RenderFlags::RenderMaterial;
				bool renderLighting = renderFlags & RenderFlags::RenderLighting;
				bool renderShadows =  renderFlags & RenderFlags::RenderShadows;

#ifdef DEBUG__MODE_FULL
				if (!camera)
				{
					Debug::log(
						"Location: void TerrainRenderer::flush()"
						"Tried to flush renderer, but the current camera was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return;
				}
#endif
				for (std::pair<std::shared_ptr<Material>, std::vector<Entity*>>& batch : _batches)
				{
					Material* material = batch.first.get();
					ShaderProgram* shader = material->getShader();

					// Either bind the material or just plain shader
					// (RendererCommands::bindMaterial(Material*) also binds the material's shader!)
					renderMaterial ? setMaterialUniforms(rendererCommands, material, shader) : shader->bind();

					// common uniforms
					shader->setUniform("projectionMatrix", projectionMatrix);
					shader->setUniform("viewMatrix", viewMatrix);
					shader->setUniform("cameraPos", camera->getEntity()->getComponent<Transform>()->getPosition());

					if (renderLighting) setLightingUniforms(shader, directionalLight);
					if (renderShadows)	setShadowUniforms(rendererCommands, shader, directionalLight->getShadowCaster());

					// per entity inside the batch
					for (Entity* e : batch.second)
					{
						std::shared_ptr<Mesh> mesh = e->getComponent<Mesh>();
						// per entity uniforms
						shader->setUniform("transformationMatrix", e->getComponent<Transform>()->getTransformationMatrix());

						// Finally drawing..
						rendererCommands->bindMesh(mesh.get());
						rendererCommands->drawIndices(mesh.get());
						rendererCommands->unbindMesh(mesh.get());
					}

					// Unbind the batch's shit..
					renderMaterial ? rendererCommands->unbindMaterial(material) : shader->unbind();
				}
			}


			void TerrainRenderer::clear()
			{
				_batches.clear();
			}


			void TerrainRenderer::setMaterialUniforms(
				const RendererCommands* rendererCommands, 
				Material* material,
				graphics::ShaderProgram* shader
			) const
			{
				rendererCommands->bindMaterial(material);
			}

			void TerrainRenderer::setLightingUniforms(
				graphics::ShaderProgram* shader, 
				const DirectionalLight* directionalLight
			) const
			{
				shader->setUniform("directionalLight_direction", directionalLight->getDirection());
				shader->setUniform("directionalLight_ambientColor", directionalLight->getAmbientColor());
				shader->setUniform("directionalLight_color", directionalLight->getColor());
			}

			void TerrainRenderer::setShadowUniforms(
				const RendererCommands* rendererCommands,
				graphics::ShaderProgram* shader, 
				ShadowCaster& shadowCaster
			) const
			{
				shader->setUniform("shadowProjMat", shadowCaster.getProjectionMatrix());
				shader->setUniform("shadowViewMat", shadowCaster.getViewMatrix());

				shader->setUniform("texture_shadowmap", 14);

				rendererCommands->bindTexture(shadowCaster.getShadowmapTexture(), 14);
			}

			const size_t TerrainRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}