# Model viewer
Application which allows you to view a textured, phong shaded 3d model and config the rendering of this model. Also, You can load your own models and textures.

![GIF Preview of model-viewer](https://github.com/Lackym/demo-model-viewer/blob/master/model-viewer-preview.gif)

# How to build
To build this demo you have to use CMake with Mingw64 GCC.
Before build, make sure that you have all dependencies installed. You can see all of them in the main CMakeLists.txt file.
To install all of them, you can use pacman package manager.
Once all deps are installed, just make an out of source build by creating new folder in the root folder of the demo, then, inside this new folder,
run <code>cmake ..</code> then <code>cmake --build .</code>. It will build a binary and move all assets to the binary folder.
