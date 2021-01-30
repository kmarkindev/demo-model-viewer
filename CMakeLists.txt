cmake_minimum_required(VERSION 3.17)

project(demo-model-viewer)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Cannot build in source dir")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_executable(ModelViewer)
add_subdirectory(src)

set(CXX_STANDARD_REQUIRED 20)

# copy assets to the dist folder after build
add_custom_command(TARGET ModelViewer POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:ModelViewer>/assets/
)

# 3rdparty
find_package(GLFW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Assimp REQUIRED)
find_package(GLM REQUIRED)
add_subdirectory(3rdparty)

# link everything
target_link_libraries(ModelViewer PRIVATE 
	${GLFW_LIBRARIES}
	GLAD
	imgui
	${ASSIMP_LIBRARIES}
)

target_include_directories(ModelViewer PRIVATE
	${GLFW_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS}
)