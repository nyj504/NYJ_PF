#pragma once
struct PlayerEquipmentInfo
{
	WeaponType type = WeaponType::NONE;
	float weaponAtk = 10.0f;
	UINT armorDef;
};
enum PlayerState
{
	IDLE, WALK, RUN, MINING, DYING, DANCE
};

class Player : public Character
{
private:
	const float JUMP_POWER = 4.5f;
	const float MAX_BLOCK_REACH = 5.0f;
	const float MAX_INTERACT_REACH = 4.5f;

public:
	Player(string name);
	~Player();

	void Update();
	void Render();

	void Damaged(float damage, Character* target) override;
	void SetPlayerState(PlayerState state);
	Vector3 GetPlayerVelocity() { return velocity; }
	
	float GetPlayerReach(bool interationType) { return interationType ? MAX_INTERACT_REACH : MAX_BLOCK_REACH; } 
	ModelAnimator* GetModelAnimator() { return modelAnimator; }

	bool IsMove() { return isMove; }
	//bool IsMining() { return isMining; }

	void SetPlayerEquipInfo(PlayerEquipmentInfo info) { this->equipInfo = info; }
	PlayerEquipmentInfo GetPlayerEquipInfo() { return equipInfo; }

private:
	void Control();
	void Move() override;
	void BuildAndMining();

	void SetCursor();
private:
	bool isMove = false;
	bool isMining = false;

	PlayerEquipmentInfo equipInfo;

	PlayerState playerState = IDLE;

	POINT clientCenterPos;
};