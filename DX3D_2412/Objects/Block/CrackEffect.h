#pragma once
class CrackEffect : public Cube
{
private:
    const float FRAME_INTERVAL = 0.05f;
public:
	CrackEffect();
	~CrackEffect();

	void Update();
	void Render();

    void UpdateFrame();
   
    bool IsMining() { return isMining; }
    
    void SetMining(Block* block);
    void ResetMining();
  
private:
    bool isMining = false;
    float time = 0.0f;
    float intervalSpeed;
    
    float frameX = 8;
    float frameY = 2;

    UINT curFrame = 0;
    POINT maxFrame;
    bool isAdditive;

    SpriteBuffer* spriteBuffer;
};