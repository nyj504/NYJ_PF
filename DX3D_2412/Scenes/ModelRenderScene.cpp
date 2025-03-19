#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	Model* model = new Model("steve");
	model->SetLocalPosition(Vector3(1, 0, 0));
	model->UpdateWorld();
	models.push_back(model);
	
}

ModelRenderScene::~ModelRenderScene()
{
	for (Model* model : models)
		delete model;
}

void ModelRenderScene::Update()
{

}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	for (Model* model : models)
	model->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
}
