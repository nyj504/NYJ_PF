#pragma once
class Animal : public Character
{
private:
	enum AnimalState
	{
		IDLE, MOVE_AROUND, RUN_AWAY, DIE
	};
	const float WANDER_DELAY = 2.0f;
	const float ACTIVATE_PARTICLE = 0.4f;
public:
	Animal(string name);
	~Animal();

	void Update();
	void Render();

	void Move() override;
	void Damaged(float damage, Character* target) override;
	void SetAnimalState(AnimalState animalState) { this->animalState = animalState; }
	void TargetOutRange();
	void MoveSideways();

	void Spawn(Vector3 pos) override;

private:
	bool isAlive = false;

	AnimalState animalState = IDLE;
	float idleWanderTimer = 0.0f;
	float deadTimer = 0.0f;

	Vector3 idlePosition;
};