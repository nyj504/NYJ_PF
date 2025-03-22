#include "Framework.h"
#include "HUDBar.h"

void HUDBar::Create(const string& texturePath)
{
	slots.reserve(MAX_SLOTS);
	Vector3 startPos = { CENTER };
	int offset = 18;

	for (int i = 0; i < MAX_SLOTS; i++)
	{
		Quad* slot = new Quad(Utility::ToWString(texturePath));
		slot->SetLocalPosition(Vector3(startPos.x + (i * offset), startPos.y));
		slot->SetLocalScale(Vector3(2, 2));
		slot->UpdateWorld();
		slots.push_back(slot);
	}
}

void HUDBar::UpdateBar(int currentCount)
{
}

void HUDBar::Render()
{
	for (Quad* slot : slots)
	{
		slot->Render();
	}
}
