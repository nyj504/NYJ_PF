#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Update();
	void Render();
	void InitAudio();

private:
	void Create();
	void Release();

};