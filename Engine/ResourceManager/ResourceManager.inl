template<typename T>
inline T* ResourceManager::GetResourcePointer(const std::string& filename)
{
	auto it = resources.find(filename);
	Resource* res = nullptr;

	if (it != resources.end())
	{
		res = it->second;
	}
	else
	{
		//IF the resource does not exist, load it
		//Load a resource depends on the EXTENSION of the file 
		std::string ext = filename.substr(filename.find_last_of(".") + 1);
		fileExt e = stringToEnum[ext];
		std::string typeName = typeid(T).name();

		switch (e)
		{
		case png:
		case jpg:
		case jfif:
		{
			res = new TextureResource;
			break;
		}
		case mp3:
		case wav:
		{
			res = new AudioResource;
			break;
		}
		case msh:
		{
			res = new MeshResource;
			break;
		}

		default:
			break;
		}

		if (res == nullptr)
			return nullptr;

		res->LoadData(filename);
		resources.insert({ filename, res });
	}

	res->SetCounter(res->GetCounter() + 1);
	return static_cast<T*>(res->GetData());
}
