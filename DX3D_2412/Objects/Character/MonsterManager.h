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
	void GetDamaged();

	void Spawn();
	void ExcuteDie();
	void ExcuteDamaged();

	Monster* GetMonsters();
	void SetAttackingMonster(Monster* monster) { attackMonster = monster; }

private:
	float deadTimer = 0.0f;
	bool isActiveParticle = false;
	Vector3 deadPosition;
	vector<Monster*>monsters;
	
	Monster* curMonster = nullptr;
	Monster* attackMonster = nullptr;

	ParticleSystem* particle;
};