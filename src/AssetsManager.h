#pragma once

#include <string>
#include <vector>
#include <filesystem>

class AssetsManager
{
public:

    AssetsManager(std::string rootFolder);
    std::string GetAssetPath(std::vector<std::string> assetPath);

private:
    
    std::string m_rootFolder;
    std::string m_assetsFolder = "assets";

};