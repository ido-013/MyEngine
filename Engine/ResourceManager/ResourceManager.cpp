#include "ResourceManager.h"

ResourceManager::~ResourceManager()
{
    UnloadAllResource();
}

void ResourceManager::UnloadResource(const std::string& filename)
{
    if (resources.empty())
        return;

    auto it = resources.find(filename);

    if (it != resources.end())
    {
        Resource* res = it->second;
        res->SetCounter(res->GetCounter() - 1);

        if (res->GetCounter() == 0)
        {
            delete res;
            resources.erase(it);
        }
    }
}

void ResourceManager::UnloadAllResource()
{
    for (auto it = resources.begin(); it != resources.end(); it++)
    {
        delete it->second;
    }

    resources.clear();
}
