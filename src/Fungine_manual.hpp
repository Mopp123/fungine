#pragma once

#include "core/Common.h"
#include "core/Debug.h"
#include "core/Program.h"
#include "core/resourceManaging/ResourceManager.h"
#include "core/window/Window.h"
#include "core/window/input/CursorPosEvent.h"
#include "core/window/input/InputHandler.h"
#include "core/window/input/KeyEvent.h"
#include "core/window/input/MouseButtonEvent.h"
#include "core/window/openglWindow/OpenglWindow.h"

#include "components/Component.h"
#include "components/common/Transform.h"
#include "components/guiComponents/GUIImage.h"
#include "components/guiComponents/GUIPanel.h"
#include "components/guiComponents/GUIText.h"


#include "components/rendering/Camera.h"
#include "components/rendering/Material.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/lighting/Lights.h"
#include "components/rendering/lighting/shadows/Shadows.h"
#include "components/rendering/renderers/Renderer.h"
#include "components/rendering/renderers/GUIRenderer.h"
#include "components/rendering/renderers/GUITextRenderer.h"
#include "components/rendering/renderers/NatureRenderer.h"
#include "components/rendering/renderers/TerrainRenderer.h"

#include "entities/Entity.h"
#include "entities/commonEntitiesLib/CommonEntityFactory.h"
#include "entities/commonEntitiesLib/shapes2D/CommonShapes2D.h"
#include "entities/commonEntitiesLib/shapes3D/CommonShapes.h"
#include "entities/guiEntities/InputField.h"
#include "entities/guiEntities/Panel.h"
#include "entities/guiEntities/Text.h"
#include "entities/terrain/Terrain.h"

#include "graphics/Buffers.h"
#include "graphics/Framebuffer.h"
#include "graphics/Graphics.h"
#include "graphics/RendererCommands.h"
#include "graphics/Texture.h"
#include "graphics/shaders/ShaderProgram.h"
#include "graphics/shaders/ShaderStage.h"
#include "graphics/shaders/ShaderUniforms.h"
#include "graphics/opengl/OpenglBuffers.h"
#include "graphics/opengl/OpenglFramebuffer.h"
#include "graphics/opengl/OpenglMesh.h"
#include "graphics/opengl/OpenglRendererCommands.h"
#include "graphics/opengl/OpenglTexture.h"
#include "graphics/opengl/shaders/OpenglShaderProgram.h"
#include "graphics/opengl/shaders/OpenglShaderStage.h"

#include "utils/Time.h"
#include "utils/fontUtils/Font.h"
#include "utils/imgUtils/stb_image.h"
#include "utils/imgUtils/Image.h"
#include "utils/modelLoading/MeshVertexParsing.h"
#include "utils/modelLoading/ModelLoading.h"
#include "utils/myMathLib/MyMathLib.h"
#include "utils/myMathLib/Matrix4.h"
#include "utils/myMathLib/Vector2.h"
#include "utils/myMathLib/Vector3.h"
#include "utils/myMathLib/Vector4.h"
#include "utils/myMathLib/Quaternion.h"
