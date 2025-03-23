#include "Framework.h"
#include "HUDBar.h"

void HUDBar::Create(const string& texturePath, Vector3 startPos)
{
	slots.reserve(MAX_SLOTS);
	int offset = 19;

	for (int i = 0; i < MAX_SLOTS; i++)
	{
		Quad* slot = new Quad(Utility::ToWString(texturePath));
		slot->SetLocalPosition(Vector3(startPos.x + (i * offset), startPos.y));
		slot->UpdateWorld();
		slots.push_back(slot);
	}
}

void HUDBar::UpdateArmorBar(int count)
{
	int fullSlot = count / 2;
	int halfSlot = count - (fullSlot * 2);

	for (int i = 0; i < fullSlot; i++)
	{
		slots[i]->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/armor_full.png");
	}

	for (int i = fullSlot; i < MAX_SLOTS; i++)
	{
		slots[i]->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/armor_empty.png");
	}
	if (halfSlot != 0)
	{
		slots[fullSlot]->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/armor_half.png");
	}
}

void HUDBar::Render()
{
	for (Quad* slot : slots)
	{
		slot->Render();
	}
}
