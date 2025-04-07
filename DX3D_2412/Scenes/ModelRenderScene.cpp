#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	test = new Transform();

	model = new Model("chicken");
	model->Load();
	model->SetParent(test);

	collider = new BoxCollider();
	collider->SetTag("chickenCollider");
	collider->Load();
	collider->UpdateWorld();
	collider->SetParent(test);
}

ModelRenderScene::~ModelRenderScene()
{
	delete test;
	delete model;
	delete collider;
}

void ModelRenderScene::Update()
{
	test->UpdateWorld();
	model->UpdateWorld();
	collider->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	model->Render();
	collider->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
	test->Edit();
	model->Edit();
	collider->Edit();
}
