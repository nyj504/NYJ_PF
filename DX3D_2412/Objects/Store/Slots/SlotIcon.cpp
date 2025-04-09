#include "Framework.h"

SlotIcon::SlotIcon() : EventQuad(L"Resources/Textures/Item/diamond.png")
{
	SetActive(false);
}

SlotIcon::~SlotIcon()
{
}

void SlotIcon::Render()
{
	if (!isActive || itemCounts.first == 0) return;

	Vector3 offset = { 5, -5 };
	EventQuad::Render();
	
	Font::Get()->RenderText(to_string(itemCounts.second), GetGlobalPosition() + offset);
}

void SlotIcon::UpdateFromSlot(InventorySlot* slot)
{
	if (slot->GetKey() <= 0 && slot->GetCount() <= 0)
	{
		itemCounts.first = 0;
		itemCounts.second = 0;
		isActive = false;
		return;
	}

	ItemData data = DataManager::Get()->GetItemData(slot->GetKey());
	string path = "Resources/Textures/Item/" + data.image + "_block.png";

	if(!data.canBuild)
		path = "Resources/Textures/Item/" + data.image + ".png";

	material->SetDiffuseMap(Utility::ToWString(path));
	
	itemCounts.first = slot->GetKey();
	itemCounts.second = slot->GetCount();

	if (itemCounts.first != 0 && itemCounts.second != 0)
		isActive = true;
	else
		isActive = false;
}

void SlotIcon::Clear()
{
	itemCounts = { 0, 0 }; 

	SetActive(false);
}

void SlotIcon::SetItem(UINT key, UINT count)
{
	itemCounts.first = key;
	itemCounts.second = count;

	ItemData data = DataManager::Get()->GetItemData(itemCounts.first);
	string path = "Resources/Textures/Item/" + data.image + "_block.png";

	if (!data.canBuild)
		path = "Resources/Textures/Item/" + data.image + ".png";

	material->SetDiffuseMap(Utility::ToWString(path));

	SetActive(true);
}
