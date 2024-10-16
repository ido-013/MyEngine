#pragma once
#include <string>

class Resource
{
	//pure abstract class
		//IF you inherit from me, you MUST override some fn
private:
	int counter = 0;

	//Use static cast for this to become Texture*, Audio* ... when needed

protected:
	void* data = nullptr;

public:
	//pure virtual fn to LOAD data
	Resource() = default;
	virtual ~Resource() = default;//Otherwise you would NOT be deleting the specialized classes

	virtual void LoadData(const std::string& name) = 0;

	//fn to return the data
	void* GetData();

	const int& GetCounter() const;
	void SetCounter(const int& _counter);
};