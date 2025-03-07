#pragma once
class PlayerSingleton : public Singleton<PlayerSingleton>
{
private:
	friend class Singleton;
	
	PlayerSingleton();
	~PlayerSingleton();

public:
	Player* GetPlayer() { return player; }
	
	void Update();
	void Render();

private:
	Player* player;
};