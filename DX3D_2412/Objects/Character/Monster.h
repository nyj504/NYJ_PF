#pragma once
class Monster : public Character
{
private:
	enum MonsterState
	{
		IDLE, INRANGE, ATTACK, DIE
	};
	const int IN_RANGE = 5;
public:
	Monster(string name);
	~Monster();

	void Update();
	void Render();

	void Move() override;
	void SetMonsterState(MonsterState state) { monsterState = state; }

	void TargetInRange();

private:
	MonsterState monsterState = IDLE;
};