#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	model = new Model("chicken");
	model->Load();

	collider = new BoxCollider();
	collider->SetTag("chickenCollider");
	collider->SetParent(model);
	collider->Load();
	collider->UpdateWorld();

}

ModelRenderScene::~ModelRenderScene()
{
	delete model;
	delete collider;
}

void ModelRenderScene::Update()
{
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
	model->Edit();
	collider->Edit();
}
