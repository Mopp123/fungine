#pragma once

#include "graphics/RendererCommands.h"
#include "graphics/Texture.h"

#define convert_to_GLenum__buffer_usage(usage, storage) switch(usage){			\
case graphics::BufferUsage::StaticDraw :	storage = GL_STATIC_DRAW;	break;	\
case graphics::BufferUsage::DynamicDraw :	storage = GL_DYNAMIC_DRAW;	break;	\
case graphics::BufferUsage::StreamDraw :	storage = GL_STREAM_DRAW;	break;	\
default :									storage = GL_NONE;			break;	\
}

#define convert_to_GLenum__shader_data_type(dataType, storage) switch(dataType){\
case graphics::ShaderDataType::Float :	storage = GL_FLOAT;	break;	\
case graphics::ShaderDataType::Float2 :	storage = GL_FLOAT;	break;	\
case graphics::ShaderDataType::Float3 :	storage = GL_FLOAT;	break;	\
case graphics::ShaderDataType::Float4 :	storage = GL_FLOAT;	break;	\
case graphics::ShaderDataType::Int :	storage = GL_INT;	break;	\
case graphics::ShaderDataType::Int2 :	storage = GL_INT;	break;	\
case graphics::ShaderDataType::Int3 :	storage = GL_INT;	break;	\
case graphics::ShaderDataType::Int4 :	storage = GL_INT;	break;	\
default :								storage = GL_NONE;	break;	\
}

#define convert_to_GLenum__draw_type(drawType, storage) switch(drawType){\
case graphics::DrawType::Triangles :		storage = GL_TRIANGLES;			break;	\
case graphics::DrawType::TriangleStrips :	storage = GL_TRIANGLE_STRIP;	break;	\
case graphics::DrawType::Lines :			storage = GL_LINES;				break;	\
case graphics::DrawType::LineLoop :			storage = GL_LINE_LOOP;			break;	\
case graphics::DrawType::Points :			storage = GL_POINTS;			break;	\
default :									storage = GL_NONE;				break;	\
}

#define convert_to_GLenum__texture_format(format, storage) switch(format){\
case graphics::TextureFormat::RGB :		storage = GL_RGB;				break;	\
case graphics::TextureFormat::RGBA :	storage = GL_RGBA;				break;	\
case graphics::TextureFormat::Red :		storage = GL_RED;				break;	\
case graphics::TextureFormat::Green :	storage = GL_GREEN;				break;	\
case graphics::TextureFormat::Blue :	storage = GL_BLUE;				break;	\
case graphics::TextureFormat::BGR :		storage = GL_BGR;				break;	\
case graphics::TextureFormat::Depth :	storage = GL_DEPTH_COMPONENT;	break;	\
default :								storage = GL_NONE;				break;	\
}

#define convert_to_GLenum__texture_data_type(type, storage) switch(type){\
case graphics::TextureDataType::UnsignedByte :		storage = GL_UNSIGNED_BYTE;	break;	\
case graphics::TextureDataType::Float :				storage = GL_FLOAT;			break;	\
case graphics::TextureDataType::UnsignedInteger :	storage = GL_UNSIGNED_INT;	break;	\
default :											storage = GL_NONE;			break;	\
}
#define convert_to_GLint__texture_wrap_mode(mode, storage) switch(mode){\
case graphics::TextureWrapping::Repeat :		storage = GL_REPEAT;			break;	\
case graphics::TextureWrapping::ClampToBorder :	storage = GL_CLAMP_TO_BORDER;	break;	\
case graphics::TextureWrapping::ClampToEdge :	storage = GL_CLAMP_TO_EDGE;		break;	\
default :										storage = GL_NONE;				break;	\
}
#define convert_to_GLint__texture_filtering_mode(mode, storage) switch(mode){\
case graphics::TextureFiltering::Linear :			storage = GL_LINEAR;				break;	\
case graphics::TextureFiltering::Nearest :			storage = GL_NEAREST;				break;	\
case graphics::TextureFiltering::LinearMipmap :		storage = GL_LINEAR_MIPMAP_LINEAR;	break;	\
case graphics::TextureFiltering::NearestMipmap :	storage = GL_NEAREST_MIPMAP_NEAREST;break;	\
default :											storage = GL_NONE;					break;	\
}

#define convert_to_GLenum__framebuffer_color_attachment(attachment, storage) switch(attachment){\
case 0 : storage = GL_COLOR_ATTACHMENT0;	break;	\
case 1 : storage = GL_COLOR_ATTACHMENT1;	break;	\
case 2 : storage = GL_COLOR_ATTACHMENT2;	break;	\
case 3 : storage = GL_COLOR_ATTACHMENT3;	break;	\
case 4 : storage = GL_COLOR_ATTACHMENT4;	break;	\
default : storage = GL_NONE;				break;	\
}

#define convert_to_GLenum__texture_slot(slotNumber, storage) switch(slotNumber){\
case 0 : storage = GL_TEXTURE0;		break;	\
case 1 : storage = GL_TEXTURE1;		break;	\
case 2 : storage = GL_TEXTURE2;		break;	\
case 3 : storage = GL_TEXTURE3;		break;	\
case 4 : storage = GL_TEXTURE4;		break;	\
case 5 : storage = GL_TEXTURE5;		break;	\
case 6 : storage = GL_TEXTURE6;		break;	\
case 7 : storage = GL_TEXTURE7;		break;	\
case 8 : storage = GL_TEXTURE8;		break;	\
case 9 : storage = GL_TEXTURE9;		break;	\
case 10 : storage = GL_TEXTURE10;	break;	\
case 11 : storage = GL_TEXTURE11;	break;	\
case 12 : storage = GL_TEXTURE12;	break;	\
case 13 : storage = GL_TEXTURE13;	break;	\
case 14 : storage = GL_TEXTURE14;	break;	\
case 15 : storage = GL_TEXTURE15;	break;	\
case 16 : storage = GL_TEXTURE16;	break;	\
default : storage = GL_NONE;		break;	\
}

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglRendererCommands : public RendererCommands
			{
			public:

				OpenglRendererCommands();
				~OpenglRendererCommands();

				virtual void init() const override;
				virtual void clear() const override;

				virtual void bindShader(ShaderProgram* shader) const override;
				virtual void unbindShader() const override; // kind of pointless?
				virtual void cullFace(CullFace face) const override;
				virtual void bindTexture(const Texture* texture, unsigned int slot) const override;
				virtual void unbindTexture(const Texture* texture, unsigned int slot) const override;
				virtual void bindMesh(const components::Mesh* const mesh) const override;
				virtual void unbindMesh(const components::Mesh* const mesh) const override;
				virtual void drawIndices(const components::Mesh* const mesh) const override;
				virtual void drawIndices_instanced(const components::Mesh* const mesh) const override;

				virtual void bindMaterial(components::Material* const material) const override;
				virtual void unbindMaterial(const components::Material* const material) const override;


				virtual void bindFramebuffer(const Framebuffer* const framebuffer) const override;
				// *Note: 
				//		If inputted framebuffer was multisampled, this function also resolves the multisampled texture into regular texture!
				virtual void unbindFramebuffer(const Framebuffer* const framebuffer) const override;
			};
		}
	}
}