#include "AssetsManager.h"

AssetsManager::AssetsManager(std::string rootFolder)
    : m_rootFolder(rootFolder)
{
    
}

std::string AssetsManager::GetAssetPath(std::vector<std::string> assetPath) 
{
    std::filesystem::path systemPath = m_rootFolder;
    systemPath /= m_assetsFolder;

    for(std::string segment : assetPath)
    {
        systemPath /= segment;
    }

    return systemPath.string();
}
