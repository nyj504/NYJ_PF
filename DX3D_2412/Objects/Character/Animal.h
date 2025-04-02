#pragma once
class Animal : public Monster
{
public:
	Animal(string name);
	~Animal();

	void Update();
	void Render();
};