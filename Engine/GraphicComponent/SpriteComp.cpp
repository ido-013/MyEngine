//#include "SpriteComp.h"
//#include "../EngineComponent/TransformComp.h"
//
//SpriteComp::SpriteComp(GameObject* _owner) : GraphicComponent(_owner), color(), tex(nullptr), mesh(nullptr), textureName(), Alpha(1)
//{
//	SetMesh();
//}
//
//SpriteComp::~SpriteComp()
//{
//	ResourceManager::GetInstance().UnloadResource(textureName);
//
//	AEGfxMeshFree(mesh);
//}
//
//void SpriteComp::Update()
//{
//	if (!mesh)
//		return;
//
//	//Set render mode
//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//
//	//Set color to multiply
//	AEGfxSetColorToMultiply(1, 1, 1, 1);
//
//	//Set blend Mode
//	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//
//	//Set transparency
//	AEGfxSetTransparency(Alpha);
//
//	//Set color to add
//	AEGfxSetColorToAdd(color.r / 255.f, color.g / 255.f, color.b / 255.f, 0.f);
//
//	//Set transform
//	//Get the transform from my owner transfrom comp
//	AEMtx33 transf = owner->GetComponent<TransformComp>()->GetMatrix();
//	AEGfxSetTransform(transf.m);
//
//	//Set texture
//	AEGfxTextureSet(tex, 0, 0);
//
//	//call DrawMesh
//	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
//}
//
//void SpriteComp::SetMesh()
//{
//	//Create quad
//	AEGfxMeshStart();
//
//	if (owner->type == GameObject::LeftTri)
//	{
//		AEGfxTriAdd(
//			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
//			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
//			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
//	}
//
//	else if (owner->type == GameObject::RightTri)
//	{
//		AEGfxTriAdd(
//			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
//			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
//			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
//	}
//
//	else if (owner->type == GameObject::LeftTri_R)
//	{
//		AEGfxTriAdd(
//			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
//			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
//			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
//	}
//
//	else if (owner->type == GameObject::RightTri_R)
//	{
//		AEGfxTriAdd(
//			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
//			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
//			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
//	}
//
//	else
//	{
//		AEGfxTriAdd(
//			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
//			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
//			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
//
//		AEGfxTriAdd(
//			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
//			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
//			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
//	}
//
//	mesh = AEGfxMeshEnd();
//}
//
//void SpriteComp::SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b)
//{
//	color.r = r;
//	color.g = g;
//	color.b = b;
//}
//
//void SpriteComp::SetTexture(std::string name)
//{
//	if (textureName != name)
//		ResourceManager::GetInstance().UnloadResource(textureName);
//
//	textureName = name;
//	tex = ResourceManager::GetInstance().GetResource<AEGfxTexture>(name);
//}
//
//void SpriteComp::SetAlpha(float alpha)
//{
//	Alpha = alpha;
//}
//
//float SpriteComp::GetAlpha()
//{
//	return Alpha;
//}