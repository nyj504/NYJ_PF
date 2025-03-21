#pragma once
class Character : public Transform
{
public:
	Character(string name);
	~Character();
	
	virtual void Update();
	virtual void Render();

	BoxCollider* GetCollider() { return collider; }

protected:
	ModelAnimator* modelAnimator;
	BoxCollider* collider;
};