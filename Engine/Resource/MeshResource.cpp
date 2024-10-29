#include "MeshResource.h"

#include "../OpenGL/GLModel.h"

MeshResource::~MeshResource()
{
	UnloadData();
}

void MeshResource::LoadData(const std::string& _filename)
{
	GLModel* mdl = new GLModel;
	mdl->init("Assets/Mesh/" + _filename);
	data = mdl;
}

void MeshResource::ReloadData(const std::string& _filename)
{
	UnloadData();
	LoadData(_filename);
}

void MeshResource::UnloadData()
{
	if (data)
	{
		GLModel* mdl = static_cast<GLModel*>(data);
		mdl->release();
		delete mdl;
	}

	data = nullptr;
}
