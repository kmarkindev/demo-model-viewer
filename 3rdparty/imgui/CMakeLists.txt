cmake_minimum_required(VERSION 3.17)

project(imgui)

add_library(imgui STATIC)

target_sources(imgui PRIVATE 
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_impl_glfw.cpp
	imgui_impl_opengl3.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
)

find_package(GLFW REQUIRED)

target_include_directories(imgui PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
	${GLFW_INCLUDE_DIR}
)

target_link_libraries(imgui PRIVATE 
	GLAD
	${GLFW_LIBRARIES}
)