#pragma once

class Enemy : public SphereCollider
{
public:
	Enemy();
	~Enemy();

	void Update();
	void Render();

private:
	Transform* target;

	Button* cube;
};
