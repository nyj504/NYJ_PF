#include "Framework.h"

void DropItem::Update()
{
	if (!isActive) return;

	Vector3 dir = PLAYER->GetLocalPosition() - GetLocalPosition();
	dir = dir.GetNormalized();

	if (Vector3::Distance(PLAYER->GetLocalPosition(), GetLocalPosition()) <= ABSORB_DISTANCE)
		Translate(dir * DELTA);

	rotationY += XM_PI * 0.1f * DELTA;
	floatTime += DELTA;
	floatOffset = sinf(floatTime * 2.0f) * 0.1f * DELTA;

	SetLocalRotation(Vector3(0, rotationY, 0));

	Translate(0, floatOffset, 0);

	UpdateWorld();
}
