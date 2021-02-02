# Model viewer
Demo app which allows you to view a textured, phong shaded 3d model.
I've created it to get my first experience with 3D graphics and main game programming patterns like game loop.

# How to build
To build this demo you have to use CMake.
Before build, make sure that you have all dependencies installed. You can see all of them in the main CMakeLists.txt file.
<b>*Important note: Assimp should be builded as dll.</b>
Once all deps are installed, just make an out of source build by creating new folder in the root folder of the demo, then, inside this new folder,
run <code>cmake ..</code> then <code>cmake --build .</code>. It will build a binary and move all assets to the binary folder.
