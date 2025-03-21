#include "Framework.h"
#include "ModelEquipScene.h"

ModelEquipScene::ModelEquipScene()
{
	modelAnimator = new ModelAnimator("SteveRigged");
	modelAnimator->Load();
	modelAnimator->ReadClip("Dance");
	//modelAnimator->ReadClip("Mining");
	modelAnimator->CreateTexture();

	collider = new BoxCollider();
	collider->SetTag("PlayerCollider");
	collider->SetParent(modelAnimator);
	collider->UpdateWorld();
	collider->Load();

	weapon = new Model("IronSword");
	weapon->Load();

	weaponSocket = new Transform();
	weapon->SetParent(weaponSocket);

	leftBootsSocket = new Transform();
	rightBootsSocket = new Transform();

	leftBoots = new Model("IronBoots_Left");
	leftBoots->SetParent(leftBootsSocket);
	leftBoots->Load();

	rightBoots = new Model("IronBoots_Right");
	rightBoots->SetParent(rightBootsSocket);
	rightBoots->Load();

	leftLegSocket = new Transform();
	rightLegSocket = new Transform();
	
	leftLeg = new Model("IronLeggings_Left");
	leftLeg->SetParent(leftLegSocket);
	leftLeg->Load();

	rightLeg = new Model("DiamondLeggings_Right");
	rightLeg->SetParent(rightLegSocket);
	rightLeg->Load();

	chestPlateSocket = new Transform();
	leftArmSocket = new Transform();
	rightArmSocket = new Transform();
	
	chestPlate = new Model("IronChestPlate");
	chestPlate->SetParent(chestPlateSocket);
	chestPlate->Load();
	
	leftArm = new Model("IronChestplate_LeftArm");
	leftArm->SetParent(leftArmSocket);
	leftArm->Load();
	
	rightArm = new Model("IronChestplate_RightArm");
	rightArm->SetParent(rightArmSocket);
	rightArm->Load();
	
	helmetSocket = new Transform();
	
	helmet = new Model("IronHelmet");
	helmet->SetParent(helmetSocket);
	helmet->Load();
}

ModelEquipScene::~ModelEquipScene()
{
	delete modelAnimator;

	delete weapon;

	delete leftBoots;
	delete rightBoots;

	delete leftLeg;
	delete rightLeg;

	delete chestPlate;
	delete leftArm;
	delete rightArm;

	delete helmet;
	delete collider;

}

void ModelEquipScene::Update()
{
	leftBootsSocket->SetWorld(modelAnimator->GetTransformByNode(5));
	rightBootsSocket->SetWorld(modelAnimator->GetTransformByNode(8));

	leftLegSocket->SetWorld(modelAnimator->GetTransformByNode(3));
	rightLegSocket->SetWorld(modelAnimator->GetTransformByNode(6));

	chestPlateSocket->SetWorld(modelAnimator->GetTransformByNode(10));
	leftArmSocket->SetWorld(modelAnimator->GetTransformByNode(13));
	rightArmSocket->SetWorld(modelAnimator->GetTransformByNode(16));
	
	helmetSocket->SetWorld(modelAnimator->GetTransformByNode(12));

	weaponSocket->SetWorld(modelAnimator->GetTransformByNode(18));

	modelAnimator->UpdateWorld();
	
	weapon->UpdateWorld();
	
	leftBoots->UpdateWorld();
	rightBoots->UpdateWorld();

	leftLeg->UpdateWorld();
	rightLeg->UpdateWorld();

	chestPlate->UpdateWorld();
	leftArm->UpdateWorld();
	rightArm->UpdateWorld();

	helmet->UpdateWorld();

	collider->UpdateWorld();

}

void ModelEquipScene::PreRender()
{
}

void ModelEquipScene::Render()
{
	modelAnimator->Render();
	
	weapon->Render();

	leftBoots->Render();
	rightBoots->Render();

	leftLeg->Render();
	rightLeg->Render();

	chestPlate->Render();
	leftArm->Render();
	rightArm->Render();

	helmet->Render();

	collider->Render();

}

void ModelEquipScene::PostRender()
{
}

void ModelEquipScene::GUIRender()
{
	modelAnimator->Edit();
	
	weapon->Edit();

	leftBoots->Edit();
	rightBoots->Edit();

	leftLeg->Edit();
	rightLeg->Edit();

	chestPlate->Edit();
	leftArm->Edit();
	rightArm->Edit();

	helmet->Edit();

	collider->Edit();
}
