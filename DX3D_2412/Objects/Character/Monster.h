#pragma once
class Monster : public Character
{
private:
	enum MonsterState
	{
		INRANGE, ATTACK, DIE
	};
	const int IN_RANGE = 5;
public:
	Monster(string name);
	~Monster();

	void Update();
	void Render();

	void Move() override;
	void Attack();
	void SetMonsterState(MonsterState state);

	void TargetInRange();

private:
	MonsterState monsterState = INRANGE;
};