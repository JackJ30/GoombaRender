cmake_minimum_required(VERSION 3.5.0)
project (GoombaRenderer)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES) # Enforce the C++17 standard

find_package(OpenGL REQUIRED)


set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/fmt/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/assimp/)

set(GOOMBARENDERDIR ${CMAKE_SOURCE_DIR}/goomba_renderer)

set(SOURCE_FILES
	${GOOMBARENDERDIR}/src/main.cpp
)

set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
)

set(SINGLE_HEADER_IMPLEMENTATION
	${CMAKE_SOURCE_DIR}/external/stb/stb_image_implementation.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SINGLE_HEADER_IMPLEMENTATION} ${IMGUI_SOURCES})

# copy resources folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

# link and inculde
target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw assimp glm fmt OpenGL::GL)
target_include_directories(${PROJECT_NAME} PUBLIC src ${CMAKE_SOURCE_DIR}/external/imgui ${CMAKE_SOURCE_DIR}/external/stb/include)