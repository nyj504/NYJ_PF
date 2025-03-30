#pragma once
class Character : public Transform
{
private:
	enum class CharacterType
	{
		STEVE, ANIMAL, MONSTER
	};
public:
	Character(string name);
	~Character();
	
	virtual void Update();
	virtual void Render();

	BoxCollider* GetCollider() { return collider; }

protected:
	CharacterType characterType = CharacterType::STEVE;
	ModelAnimator* modelAnimator;
	Model* model;
	BoxCollider* collider;
};