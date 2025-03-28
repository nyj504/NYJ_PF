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

	EventQuad::Render();
	
	Font::Get()->RenderText(to_string(itemCounts.second), GetGlobalPosition());
}

void SlotIcon::UpdateFromSlot(InventorySlot* slot)
{
	if (slot->GetKey() != 0)
	{
		ItemData data = DataManager::Get()->GetItemData(slot->GetKey());
		string path = "Resources/Textures/Item/" + data.image + "_block.png";

		if(!data.canBuild)
			path = "Resources/Textures/Item/" + data.image + ".png";

		material->SetDiffuseMap(Utility::ToWString(path));
	}

	itemCounts.first = slot->GetKey();
	itemCounts.second = slot->GetCount();

	if (itemCounts.first != 0)
		SetActive(true);
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
}
