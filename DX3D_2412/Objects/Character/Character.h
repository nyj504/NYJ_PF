#pragma once
class Character : public Transform
{
public:
	Character(string path = "Resources/Models/zombie.model");
	~Character();
	
	virtual void Update();
	virtual void Render();

	void ApplySkinUVMapping();

	void Load(string path);

	BoxCollider* GetCollider() { return boxCollider; }

private:
	vector<Cube*>bodyComponents;
	BoxCollider* boxCollider;
};