#pragma once
class Animal : public Character
{
public:
	Animal();
	~Animal();

	void Update();
	void Render();

private:
	Model* model;
};