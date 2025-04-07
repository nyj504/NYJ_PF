#include "Framework.h"

DropCube::DropCube()
{
	cube = new Cube(Vector3(0.1f, 0.1f, 0.1f));
	cube->SetParent(this);

	isActive = false;
}

DropCube::DropCube(bool isMultiBlock)
{
	cube = new Cube(Vector3(0.1f, 0.1f, 0.1f));
	cube->ApplyBlockUVMapping();
	cube->SetParent(this);

	isActive = false;
}

DropCube::~DropCube()
{
	delete cube;
}

void DropCube::Update()
{
	if (!isActive) return;

	DropItem::Update();
	UpdateWorld();
	cube->Update();

	if (cube->GetCollider()->IsCollision(PLAYER->GetCollider()))
	{
		Audio::Get()->Play("pop");

		ItemData data = DataManager::Get()->GetItemData(key);
		INVEN->AddItem(data.dropItemKey, data.dropsAmount);
		isActive = false;
		return;
	}
}

void DropCube::Render()
{
	if (!isActive) return;

	cube->Render();
}

void DropCube::SetImage(string path)
{
	cube->GetMaterial()->SetDiffuseMap(Utility::ToWString(path));
}
