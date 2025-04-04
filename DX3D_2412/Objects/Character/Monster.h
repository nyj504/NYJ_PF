#pragma once
class Monster : public Character
{
private:
	enum MonsterState
	{
		IDLE, MOVE, ATTACK, DIE
	};
	const int IN_RANGE = 5;
	const float ATK_RANGE = 1.5f;
public:
	Monster(string name);
	~Monster();

	void Update();
	void Render();

	void Move() override;
	void Damaged(float damage);
	void SetMonsterState(MonsterState state);

	void TargetInRange();
	void Spawn(Vector3 pos);
	bool IsAlive() { return isAlive; }

private:
	bool isAlive = false;
	MonsterState monsterState = IDLE;
};