#pragma once
class Character : public Transform
{
private:
	enum class CharacterType
	{
		STEVE, ANIMAL, MONSTER
	};
	const float GRAVITY = 4.9f;
public:
	Character(string name);
	~Character();
	
	virtual void Update();
	virtual void Render();

	virtual void Move();

	void SetLand() { velocity.y = 0; }
	void SetFall() { velocity.y -= GRAVITY * DELTA; }
	int GetHp() { return curHp; }

	BoxCollider* GetCollider() { return collider; }

protected:
	UINT characterKey;
	int curHp;
	CharacterData characterData;
	float rotSpeed = 1.0f;
	Vector3 velocity;

	CharacterType characterType = CharacterType::STEVE;
	ModelAnimator* modelAnimator;
	Model* model;
	BoxCollider* collider;
};