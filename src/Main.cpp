#include "Application.h"
#include <iostream>
#include <exception>
#include "Exceptions.h"
#include "InputParams.h"

// stb_image required to define it here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

InputParams g_inputParams;
InputParams parseParams(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    g_inputParams = parseParams(argc, argv);

    auto app = new Application(g_inputParams.GetModelPath());

    app->Init();

    app->Start();

    app->Shutdown();

    return 0;
}

InputParams parseParams(int argc, char* argv[])
{
    std::string appPath = std::string(argv[0]);

    InputParams params;

    params.modelPath = "/assets/models/derringer/derringer.fbx";
    params.rootFolder = appPath.substr(0, appPath.find_last_of("\\"));

    return params;
}