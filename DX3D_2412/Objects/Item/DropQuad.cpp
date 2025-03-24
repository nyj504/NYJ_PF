#include "Framework.h"

DropQuad::DropQuad() : DropItem()
{
	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->CullMode(D3D11_CULL_NONE);

	Vector2 size = { 0.1f, 0.1f };
	quad = new Quad(size);
	quad->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	quad->SetParent(this);

	collider = new RectCollider(size);
	collider->SetParent(this);

	isActive = false;
}

DropQuad::~DropQuad()
{
	delete quad;
	delete collider;
	delete rasterizerState[0];
	delete rasterizerState[1];
}

void DropQuad::Update()
{
	if (!isActive) return;

	DropItem::Update();

	UpdateWorld();
	quad->UpdateWorld();
	collider->UpdateWorld();

	if (collider->IsCollision(PLAYER->GetCollider()))
	{
		ItemData data = DataManager::Get()->GetItemData(key);
		INVEN->AddItem(data.dropItemKey, data.dropsAmount);
		isActive = false;
		return;
	}
}

void DropQuad::Render()
{
	if (!isActive) return;

	Environment::Get()->SetAlphaBlend(true);
	rasterizerState[1]->SetState();
	quad->Render();
	rasterizerState[0]->SetState();
	Environment::Get()->SetAlphaBlend(false);

	collider->Render();
}

void DropQuad::SetImage(string path)
{
	quad->GetMaterial()->SetDiffuseMap(Utility::ToWString(path));
}
