cmake_minimum_required(VERSION 3.17)

project(imguiFileDialog)

add_library(imguiFileDialog STATIC)

target_link_libraries(imguiFileDialog PRIVATE imgui)

target_sources(imguiFileDialog PRIVATE 
	ImGuiFileDialog.cpp
    ImGuiFileDialog.h
    ImGuiFileDialogConfig.h
    dirent/dirent.h
)

target_include_directories(imguiFileDialog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)