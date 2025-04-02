#pragma once
class MonsterManager : public Singleton<MonsterManager>
{
private:
	const int MONSTER_COUNT = 5;
private:
	friend class Singleton;
	MonsterManager();
	~MonsterManager();

public:
	void Update();
	void Render();

	void Spawn();

	Monster* GetMonsters();

private:
	vector<Monster*>monsters;
};