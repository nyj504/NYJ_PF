#pragma once
struct PlayerEquipmentInfo
{
	WeaponType type = WeaponType::NONE;
	float weaponAtk;
	UINT armorDef;
};

class Player : public Character
{
private:
	enum PlayerState
	{
		IDLE, MOVE, JUMP, FALL, LAND, TOUCH
	};

private:
	const float GRAVITY = 9.8f;
	const float JUMP_POWER = 1.5f;
	const float MAX_BLOCK_REACH = 5.0f;
	const float MAX_INTERACT_REACH = 4.5f;

public:
	Player();
	~Player();

	void Update();
	void Render();
	void PostRender();

	void SetPlayerState(PlayerState state);
	
	float GetPlayerReach(bool interationType) { return interationType ? MAX_INTERACT_REACH : MAX_BLOCK_REACH; } 
	ModelAnimator* GetModelAnimator() { return modelAnimator; }

	int GetDamage() { return atk; }
	void SetLand();
	void SetFall();
	bool IsMove() { return isMove; }
	bool IsMining() { return isMining; }

	void SetPlayerEquipInfo(PlayerEquipmentInfo info) { this->equipInfo = info; }
	PlayerEquipmentInfo GetPlayerEquipInfo() { return equipInfo; }

private:
	void Control();
	void Jump();
	void Move();
	void BuildAndMining();

	void SetCursor();
private:
	bool isCreativeMode = false;
	bool isMove = false;
	bool isMining = false;
	int atk = 10;
	float jumpTime = 0.0f;
	float moveSpeed = 5.0f;
	float rotSpeed = 1.0f;

	PlayerEquipmentInfo equipInfo;

	//Armor* armor;

	PlayerState playerState = IDLE;

	Quad* item;
	
	Model* weapon;
	Transform* weaponSocket;
	Vector3 velocity;

	POINT clientCenterPos;
};