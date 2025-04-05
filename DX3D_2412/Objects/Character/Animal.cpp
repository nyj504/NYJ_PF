#include "Framework.h"

Animal::Animal(string name) : Monster(name)
{
    model = new Model(name);
    model->Load();
    model->SetParent(this);

    collider = new BoxCollider();
    collider->SetTag("chickenCollider");
    collider->Load();
}

Animal::~Animal()
{
}

void Animal::Update()
{
	Monster::Update();

	UpdateWorld();
}

void Animal::Render()
{
	Character::Render();
}

void Animal::TargetOutRange()
{
	idleWanderTimer += DELTA;

	if (idleWanderTimer >= WANDER_DELAY)
	{
		idleWanderTimer -= WANDER_DELAY;

		float offsetX = GameMath::Random(-3.0f, 3.0f);
		float offsetZ = GameMath::Random(-3.0f, 3.0f);

		Vector3 idleTargetPos = idlePosition + Vector3(offsetX, 0.0f, offsetZ);
		idleWanderTimer = 0.0f;

		LookAt(idleTargetPos);

		Vector3 dir = idleTargetPos - GetLocalPosition();
		dir.y = 0.0f;

		dir.Normalize();
		velocity.x = dir.x;
		velocity.z = dir.z;
	}
}
