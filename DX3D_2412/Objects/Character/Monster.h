#pragma once
class Monster : public Character
{
private:
	enum MonsterState
	{
		IDLE, MOVE, ATTACK, DIE
	};
public:
	Monster();
	~Monster();

	void Update();
	void Render();

private:
	MonsterState monsterState = MOVE;
};