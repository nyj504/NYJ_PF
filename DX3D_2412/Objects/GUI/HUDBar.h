#pragma once
class HUDBar
{
private:
	const int MAX_SLOTS = 20;
public:
	void Create(const string& texturePath);
	void UpdateBar(int currentCount);  
	void Render();

private:
	vector<Quad*> slots;
};