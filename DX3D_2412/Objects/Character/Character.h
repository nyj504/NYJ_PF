#pragma once
class Character : public Transform
{
protected:
	enum class CharacterType
	{
		STEVE, ANIMAL, MONSTER
	};
	const float GRAVITY = 4.9f;
	const Vector3 KNOCKBACK_RANGE = { 0.2f, 0.2f, 0.2f };
	const float HIT_DURATION = 0.2f;
	const float SAYING_INTERVAL = 7.5f;

public:
	Character(string name);
	~Character();
	
	virtual void Update();
	virtual void Render();

	virtual void Move();

	void ActivateHitState();
	void DeactivateHitState();
	virtual void Damaged(float damage, Character* target);
	virtual void Spawn(Vector3 pos) {};

	void SetLand() { velocity.y = 0; }
	void SetFall() { velocity.y -= GRAVITY * DELTA; }
	int GetHp() { return curHp; }

	CharacterData GetCharacterData() { return characterData; }

	BoxCollider* GetCollider() { return collider; }

protected:
	bool isHitEffect = false;
	float hitTimer = 0.0f;
	float sayingTimer = 0.0f;
	UINT characterKey;
	int curHp;
	CharacterData characterData;
	float rotSpeed = 1.0f;
	Vector3 velocity;

	CharacterType characterType = CharacterType::STEVE;
	ModelAnimator* modelAnimator= nullptr;
	Model* model = nullptr;
	BoxCollider* collider = nullptr;
};