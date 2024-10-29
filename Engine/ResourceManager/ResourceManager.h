#pragma once

#include <iostream>
#include <map>
#include <string>

#include "../Resource/Resources.h"

class ResourceManager
{
public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	//Get<T>(name) fn that returns a T* to the data allocated in the resource with that name
	template <typename T>
	T* GetResourcePointer(const std::string& _filename);

	void ReloadResource(const std::string& _filename);

	//Unload(name) fn that removes 1 from the counter of the resource.
		//If the counter is 0, unload the resource, delete it, and remove from map
	void UnloadResource(const std::string& _filename);

	//Fn to unload ALL resources
	void UnloadAllResource();

private:
	ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	const ResourceManager& operator=(const ResourceManager&) = delete;

	~ResourceManager();

	std::map<std::string, Resource*> resources;

	enum fileExt
	{
		unvalid,
		png,
		jpg,
		jfif,
		wav,
		mp3,
		msh,
		shd,
		prefab,
	};

	std::map<std::string, fileExt> stringToEnum =
	{
		{"png", png},
		{"jpg", jpg},
		{"jfif", jfif},
		{"wav", wav},
		{"mp3", mp3},
		{"msh", msh},
		{"shd", shd},
		{"prefab", prefab},
	};
};

#include "ResourceManager.inl"