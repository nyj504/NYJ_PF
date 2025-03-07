#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	Model* model = new Model("Diamond_Sword");
	model->SetLocalPosition(Vector3(1, 0, 0));
	model->UpdateWorld();
	models.push_back(model);
	
	Model* model1 = new Model("Golden_Axe");
	model1->SetLocalPosition(Vector3(1.5, 0, 0));
	model1->UpdateWorld();
	models.push_back(model1);
	Model* model2 = new Model("Shears");
	model2->SetLocalPosition(Vector3(2, 0, 0));
	model2->UpdateWorld();
	models.push_back(model2);
	Model* model3 = new Model("Bow");
	model3->SetLocalPosition(Vector3(2.5, 0, 0));
	model3->UpdateWorld();
	models.push_back(model3);
	Model* model4 = new Model("Stone_Axe");
	model4->SetLocalPosition(Vector3(3, 0, 0));
	model4->UpdateWorld();
	models.push_back(model4);
	Model* model5 = new Model("Fishing_Rod");
	model5->SetLocalPosition(Vector3(3.5, 0, 0));
	model5->UpdateWorld();
	models.push_back(model5);
	Model* model6 = new Model("Wooden_Sword");
	model6->SetLocalPosition(Vector3(4, 0, 0));
	model6->UpdateWorld();
	models.push_back(model6);
	Model* model7 = new Model("Wooden_Shovel");
	model7->SetLocalPosition(Vector3(4.5, 0, 0));
	model7->UpdateWorld();
	models.push_back(model7);
	Model* model8 = new Model("Wooden_Sword");
	model8->SetLocalPosition(Vector3(5, 0, 0));
	model8->UpdateWorld();


	models.push_back(model8);
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
