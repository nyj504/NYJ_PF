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

	Monster* GetMonsters();

private:
	float deadTimer = 0.0f;
	bool isActiveParticle = false;
	Vector3 deadPosition;
	vector<Monster*>monsters;

	ParticleSystem* particle;
};