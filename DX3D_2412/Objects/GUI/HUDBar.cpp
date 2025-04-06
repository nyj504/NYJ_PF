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

void HUDBar::UpdateBar(string type, int count)
{
	int fullSlot = count / 2;
	int halfSlot = count - (fullSlot * 2);

	for (int i = 0; i < fullSlot; i++)
	{
		string path = "Resources/Textures/UI/" + type + "_full.png";
		slots[i]->GetMaterial()->SetDiffuseMap(Utility::ToWString(path));
	}

	for (int i = fullSlot; i < MAX_SLOTS; i++)
	{
		string path = "Resources/Textures/UI/" + type + "_empty.png";

		slots[i]->GetMaterial()->SetDiffuseMap(Utility::ToWString(path));
	}

	if (halfSlot != 0)
	{
		string path = "Resources/Textures/UI/" + type + "_half.png";

		slots[fullSlot]->GetMaterial()->SetDiffuseMap(Utility::ToWString(path));
	}
}

void HUDBar::Render()
{
	for (Quad* slot : slots)
	{
		slot->Render();
	}
}
