#include "Framework.h"

Bullet::Bullet() : SphereCollider(0.2f)
{
	sphere = new Sphere(0.2f);
	sphere->SetParent(this);
	sphere->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Stones.png");
}

Bullet::~Bullet()
{
	delete sphere;
}

void Bullet::Update()
{
	if (!IsActive()) return;

	lifeTime += DELTA;

	if (lifeTime > LIFE_TIME)
		SetActive(false);

	Translate(velocity * speed * DELTA);

	UpdateWorld();
	sphere->UpdateWorld();
}

void Bullet::Render()
{
	Collider::Render();
	sphere->Render();
}

void Bullet::Fire(Vector3 pos, Vector3 dir)
{
	localPosition = pos;
	velocity = dir;
	lifeTime = 0.0f;

	SetActive(true);
}
