#pragma once
class MonsterManager : public Singleton<MonsterManager>
{
private:
	const int MONSTER_COUNT = 3;
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

	Character* GetMonsters();
	void SetAttackingMonster(Monster* monster) { attackMonster = monster; }

private:
	float deadTimer = 0.0f;
	bool isActiveParticle = false;
	Vector3 deadPosition;
	
	vector<Character*>monsters;
	
	Character* curMonster = nullptr;
	Monster* attackMonster = nullptr;

	ParticleSystem* particle;
};