#pragma once
class DropItem : public Transform
{
protected:
    const int ABSORB_DISTANCE = 3;
    const float FLOAT_FREQUENCY = 2.0f;
    const float FLOAT_AMPLITUDE = 0.1f;

public:
    void Update();
    virtual void Render() {};

    void SetKey(UINT key) { this->key = key; }
    
    virtual void SetImage(string path) {};

protected:
    UINT key;

    float rotationY = 0.0f;
    float floatOffset = 0.0f;
    float floatTime = 0.0f;
};