#pragma once
class Animal : public Character
{
private:
	enum AnimalState
	{
		IDLE, MOVE_AROUND, RUN_AWAY, DIE
	};
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
	void MoveSideways() override;

	void Spawn(Vector3 pos) override;

private:
	bool isAlive = false;

	AnimalState animalState = IDLE;
	float deadTimer = 0.0f;
};