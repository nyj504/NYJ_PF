#include "Framework.h"

void DropItem::Update()
{
	if (!isActive) return;

	Vector3 dir = PLAYER->GetLocalPosition() - GetLocalPosition();
	dir = dir.GetNormalized();

	if (Vector3::Distance(PLAYER->GetLocalPosition(), GetLocalPosition()) <= ABSORB_DISTANCE)
		Translate(dir * DELTA);  //아이템 흡수

	rotationY += XM_PI * FLOAT_AMPLITUDE * DELTA;
	floatTime += DELTA; // 부유 시간
	floatOffset = sinf(floatTime * FLOAT_FREQUENCY) * FLOAT_AMPLITUDE * DELTA;

	SetLocalRotation(Vector3(0, rotationY, 0));

	Translate(0, floatOffset, 0);

	UpdateWorld();
}
