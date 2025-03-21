#include "Framework.h"
#include "ModelEquipScene.h"

ModelEquipScene::ModelEquipScene()
{
	modelAnimator = new ModelAnimator("SteveRigged");
	modelAnimator->Load();
	modelAnimator->ReadClip("Idle");
	//modelAnimator->ReadClip("Run");
	modelAnimator->CreateTexture();

	weapon = new Model("DiamondHelmet");
	weapon->Load();

	weaponSocket = new Transform();
	weapon->SetParent(weaponSocket);

	arrow = new Model("DiamondChestplate_LeftArm");
	arrow->Load();
	
	arrowSocket = new Transform();
	arrow->SetParent(arrowSocket);
}

ModelEquipScene::~ModelEquipScene()
{
	delete modelAnimator;
	delete weapon;
	delete weaponSocket;
}

void ModelEquipScene::Update()
{
	weaponSocket->SetWorld(modelAnimator->GetTransformByNode(2));
	arrowSocket->SetWorld(modelAnimator->GetTransformByNode(3));

	modelAnimator->UpdateWorld();
	weapon->UpdateWorld();
	arrow->UpdateWorld();
}

void ModelEquipScene::PreRender()
{
}

void ModelEquipScene::Render()
{
	modelAnimator->Render();
	weapon->Render();
	arrow->Render();
}

void ModelEquipScene::PostRender()
{
}

void ModelEquipScene::GUIRender()
{
	modelAnimator->Edit();
	weapon->Edit();
	arrow->Edit();
}
