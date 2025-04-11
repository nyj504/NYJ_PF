#pragma once
class MonsterManager : public Singleton<MonsterManager>
{
private:
	const int MONSTER_COUNT = 3;
	const int SPAWN_INTERVAL = 60.0f;
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

	vector<Character*> GetActiveMonsters();
	void SetAttackingMonster(Monster* monster) { attackMonster = monster; }

private:
	float deadTimer = 0.0f;
	float spawnTimer = 0.0f;
	bool isActiveParticle = false;
	Vector3 deadPosition;
	
	vector<Character*>monsters;
	
	Character* curMonster = nullptr;
	Monster* attackMonster = nullptr;

	ParticleSystem* particle;
};