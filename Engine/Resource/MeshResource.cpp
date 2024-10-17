#include "MeshResource.h"

#include "../OpenGL/GLModel.h"

MeshResource::~MeshResource()
{
	if (data)
	{
		GLModel* mdl = static_cast<GLModel*>(data);
		mdl->release();
		delete mdl;
	}
	
	data = nullptr;
}

void MeshResource::LoadData(const std::string& _filename)
{
	GLModel* mdl = new GLModel;
	mdl->init("Assets/Mesh/" + _filename);
	data = mdl;
}