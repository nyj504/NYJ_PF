#pragma once
class HUDBar
{
private:
	const int MAX_SLOTS = 10;
public:
	void Create(const string& texturePath, Vector3 startPos);
	void UpdateBar(string type, int count);
	void UpdateHungerBar();
	void Render();

private:
	vector<Quad*> slots;
};