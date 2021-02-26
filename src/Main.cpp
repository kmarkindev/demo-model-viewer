#include "Application.h"

// stb_image requires to define it here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char* argv[])
{
    auto app = new Application();

    app->Init(argc, argv);

    app->Start();

    app->Shutdown();

    return 0;
}