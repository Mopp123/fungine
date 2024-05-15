
#include <GL/glew.h>
#include "OpenglRendererCommands.h"
#include "OpenglTexture.h"
#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{

			OpenglTexture::OpenglTexture(
				const ImageData* imgData,
				unsigned int width, unsigned int height, int samples,
				TextureFormat internalFormat,
				TextureFormat format,
				TextureDataType texDataType,
				TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,
				TextureFiltering texFilteringMode_min, TextureFiltering texFilteringMode_mag
			) :
				Texture(imgData)
			{


				// *allow creating opengl texture without any pixel data as well
				byte* pixelData = nullptr;
				if (imgData)
				{
					size_t imgSize = sizeof(byte) * width * height * imgData->getChannels(); // size of imgData in bytes
					pixelData = new byte[imgSize];
					memcpy(pixelData, imgData->getData(), imgSize);

					// force using format the image data has..
					switch (imgData->getChannels())
					{
					case 1: internalFormat = (format = TextureFormat::Red); break;
					case 3: internalFormat = (format = TextureFormat::RGB); break;
					case 4: internalFormat = (format = TextureFormat::RGBA); break;
					default:break;
					}
				}

				GLenum glTextureTarget = samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

				GLint glInternalFormat = 0;
				GLenum glFormat = 0;
				GLenum glDataType = 0;

				GLint glTexWrap_s = 0;
				GLint glTexWrap_t = 0;
				GLint glTexFiltering_min = 0;
				GLint glTexFiltering_mag = 0;

				convert_to_GLenum__texture_format(internalFormat, glInternalFormat);
				convert_to_GLenum__texture_format(format, glFormat);
				convert_to_GLenum__texture_data_type(texDataType, glDataType);
				convert_to_GLint__texture_wrap_mode(texWrapMode_s, glTexWrap_s);
				convert_to_GLint__texture_wrap_mode(texWrapMode_t, glTexWrap_t);
				convert_to_GLint__texture_filtering_mode(texFilteringMode_min, glTexFiltering_min);
				convert_to_GLint__texture_filtering_mode(texFilteringMode_mag, glTexFiltering_mag);

				_id = 0;
				GL_FUNC(glGenTextures(1, &_id));
				GL_FUNC(glBindTexture(glTextureTarget, _id));

				if (glTextureTarget == GL_TEXTURE_2D_MULTISAMPLE)
				{
					GL_FUNC(glTexImage2DMultisample(glTextureTarget, samples, glInternalFormat, width, height, GL_TRUE));
				}
				else
				{
					GL_FUNC(glTexImage2D(glTextureTarget, 0, glInternalFormat, width, height, 0, glFormat, glDataType, pixelData));

					GL_FUNC(glTexParameteri(glTextureTarget, GL_TEXTURE_WRAP_S, glTexWrap_s));
					GL_FUNC(glTexParameteri(glTextureTarget, GL_TEXTURE_WRAP_T, glTexWrap_t));

					GL_FUNC(glTexParameteri(glTextureTarget, GL_TEXTURE_MIN_FILTER, glTexFiltering_min));
					GL_FUNC(glTexParameteri(glTextureTarget, GL_TEXTURE_MAG_FILTER, glTexFiltering_mag));

					if (glTexFiltering_min == GL_LINEAR_MIPMAP_LINEAR ||
						glTexFiltering_min == GL_NEAREST_MIPMAP_NEAREST ||
						glTexFiltering_min == GL_NEAREST_MIPMAP_LINEAR ||
						glTexFiltering_min == GL_LINEAR_MIPMAP_NEAREST
						)
					{
						GL_FUNC(glGenerateMipmap(glTextureTarget));
					}
				}

				GL_FUNC(glBindTexture(glTextureTarget, 0));

				if (pixelData)
					delete[] pixelData;
			}

			OpenglTexture::~OpenglTexture()
			{
				GL_FUNC(glDeleteTextures(1, &_id));
				Debug::notify_on_destroy("OpenglTexture");
			}

			// Updates the texture to look like its' image data
			// If texture's underlying image data changes, the texture wont look any different, until this function is called!
			void OpenglTexture::syncWithImgData() const
			{

			}
		}
	}
}
