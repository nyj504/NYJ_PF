#pragma once
class Monster : public Character
{
protected:
	enum MonsterState
	{
		IDLE, MOVE, ATTACK, DIE
	};
	const float ATK_RANGE = 1.5f;
	const float EXCUTE_ATTACK = 0.4f;
	const float EXCUTE_DIE = 0.85f;
	
public:
	Monster(string name);
	~Monster();

	void Update();
	void Render();

	void Move() override;
	void Damaged(float damage, Character* target) override;
	void SetMonsterState(MonsterState state);
	void ExcuteAttack();

	void TargetInRange();
	void MoveSideways() override;
	void Spawn(Vector3 pos) override;

protected:
	bool isAlive = false;
	MonsterState monsterState = IDLE;
};