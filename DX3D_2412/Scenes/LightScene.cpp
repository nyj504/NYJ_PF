#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
	FOR(1)
	{
		Sphere* sphere = new Sphere(10.0f);
		sphere->SetLocalPosition(i * 20, 0, 0);
		sphere->SetTag("Sphere" + to_string(i));
		spheres.push_back(sphere);
	}
}

LightScene::~LightScene()
{
	for (Sphere* sphere : spheres)
		delete sphere;
}

void LightScene::Update()
{
	for (Sphere* sphere : spheres)
		sphere->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
	for (Sphere* sphere : spheres)
		sphere->Render();
}

void LightScene::PostRender()
{
}

void LightScene::GUIRender()
{
	for (Sphere* sphere : spheres)
	{
		sphere->Edit();
		sphere->GetMaterial()->Edit();
	}
}
