#pragma once
#include <string>

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

	void LoadLevel(const std::string& filename);
	void SaveLevel(const std::string& filename);
};