cmake_minimum_required(VERSION 3.5.0)
project (GoombaRender)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES) # Enforce the C++17 standard

find_package(OpenGL REQUIRED)


set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/assimp/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/spdlog/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/libassert/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/sdl/)

set(GOOMBARENDERDIR ${CMAKE_SOURCE_DIR}/goomba_render)

set(SOURCE_FILES
	${GOOMBARENDERDIR}/src/main.cpp
	${GOOMBARENDERDIR}/src/engine/engine.cpp
	${GOOMBARENDERDIR}/src/engine/application.cpp
	${GOOMBARENDERDIR}/src/engine/window/sdl_window.cpp
	${GOOMBARENDERDIR}/src/engine/imgui_layer.cpp
	${GOOMBARENDERDIR}/src/engine/log.cpp
	${GOOMBARENDERDIR}/src/engine/graphics_context.cpp
	${GOOMBARENDERDIR}/src/renderer/renderer_application.cpp
	${GOOMBARENDERDIR}/src/renderer/shader.cpp
	${GOOMBARENDERDIR}/src/renderer/vertex_array.cpp
	${GOOMBARENDERDIR}/src/renderer/vertex_buffer.cpp
	${GOOMBARENDERDIR}/src/renderer/index_buffer.cpp
	${GOOMBARENDERDIR}/src/renderer/mesh.cpp
	${GOOMBARENDERDIR}/src/renderer/camera.cpp
	${GOOMBARENDERDIR}/src/renderer/perspective_camera.cpp
	${GOOMBARENDERDIR}/src/engine/input.cpp
)

set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl3.cpp
)

set(SINGLE_HEADER_IMPLEMENTATION
	${CMAKE_SOURCE_DIR}/external/stb/stb_image_implementation.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SINGLE_HEADER_IMPLEMENTATION} ${IMGUI_SOURCES})

# copy resources folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

# link and inculde
target_link_libraries(${PROJECT_NAME} PUBLIC glad assimp glm spdlog libassert::assert SDL3::SDL3 OpenGL::GL)

# copy DLLS
if(${WIN32})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${GOOMBARENDERDIR}/src ${CMAKE_SOURCE_DIR}/external/imgui ${CMAKE_SOURCE_DIR}/external/stb/include)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${GOOMBARENDERDIR}/src/goombapch.h)

# add debug definition
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")