#include "Application.h"
#include <exception>
#include <iostream>

// stb_image requires to define it here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char* argv[])
{
    try
    {
        auto app = new Application();

        app->Init(argc, argv);

        app->Start();

        app->Shutdown();

    }
    catch(std::exception ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}