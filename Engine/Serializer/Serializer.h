#pragma once
#include <string>
#include <map>

class Serializer
{
private:
	Serializer() = default;
	~Serializer() = default;

	Serializer(const Serializer&) = delete;
	Serializer& operator =(const Serializer&) = delete;
	Serializer(Serializer&&) = delete;
	Serializer& operator =(Serializer&&) = delete;

public:
	static Serializer& GetInstance()
	{
		static Serializer instance;
		return instance;
	}

	void LoadLevel(const std::string& _filename);
	void SaveLevel(const std::string& _filename, std::map<std::string, bool>& _isSaveLevelPrefabComp, bool _isTemp = false);
	void DeleteLevel(const std::string& _filename);
};