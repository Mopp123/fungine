# NOTE: IMPORTANT! This may fuck up since subdirectories CMakeLists.. not sure how that works?
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME "fungine")
set(TARGET_NAME "fungine-engine")

project(${PROJ_NAME})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17 -DGLEW_NO_GLU")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -DGLEW_NO_GLU -Ofast --loop_optimization_level=2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: Have single "combined header file" instead of including the whole shit..
include_directories("./src" "./include")

# NOTE: This may fuck up due to things being in wrong order!
file(
    GLOB SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/src/core/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/core/resourceManaging/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/core/window/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/core/window/input/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/core/window/openglWindow/*.cpp"

    "${PROJECT_SOURCE_DIR}/src/utils/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/fontUtils/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/imgUtils/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/modelLoading/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/myMathLib/*.cpp"

    "${PROJECT_SOURCE_DIR}/src/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/components/common/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/components/rendering/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/components/rendering/lighting/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/components/rendering/lighting/shadows/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/components/rendering/renderers/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/components/guiComponents/*.cpp"

    "${PROJECT_SOURCE_DIR}/src/entities/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/entities/commonEntitiesLib/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/entities/commonEntitiesLib/shapes2D/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/entities/commonEntitiesLib/shapes3D/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/entities/guiEntities/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/entities/terrain/*.cpp"

    "${PROJECT_SOURCE_DIR}/src/graphics/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/graphics/shaders/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/graphics/opengl/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/graphics/opengl/shaders/*.cpp"
)


find_package(OpenGL REQUIRED)

#add_executable(test ${SOURCE_FILES})
add_library(${TARGET_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} "${PROJECT_SOURCE_DIR}/lib/libglfw.so.3")
target_link_libraries(${TARGET_NAME} "${PROJECT_SOURCE_DIR}/lib/libGLEW.so.2.2")
target_link_libraries(${TARGET_NAME} "${PROJECT_SOURCE_DIR}/lib/libfreetype.so.6.20.1")
target_link_libraries(${TARGET_NAME} "${PROJECT_SOURCE_DIR}/lib/libassimp.so.5")
target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARIES})
