#pragma once

class Button : public Quad
{
protected:
    enum State
    { 
        NONE, OVER, DOWN
    };

    const Float4 NONE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 1.0f };
    const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 1.0f };

public:
    Button(wstring textureFile);
    Button(Vector2 size);
    ~Button();

    void Update();
    void Render();    

    void SetEvent(function<void()> event) { this->event = event; }   
    void SetIntParamEvent(function<void(int)> event) { intParamEvent = event; }
    void SetObjParamEvent(function<void(void*)> event) { objParamEvent = event; }

    void SetIntParam(int value) { intParam = value; }
    void SetObjParam(void* value) { objParam = value; }

    int GetIntParam() { return intParam; }

    void SetText(string text) { this->text = text; }
    void SetTextColor(string colorKey) { textColor = colorKey; }
    void SetTextStyle(string styleKey) { textStyle = styleKey; }

    Vector2 GetTextSize() const { return textSize; }
    bool IsCollision(const Vector3& point);
private:
    void ClickEvent();

protected:
    State state = NONE;
    bool isDownCheck = false;

    string text;
    string textColor = "Black";
    string textStyle = "Button";

    Vector2 textSize = { 10, 10 };
    Vector2 textPos;
    
    function<void()> event = nullptr;
    function<void(int)> intParamEvent = nullptr;
    function<void(void*)> objParamEvent = nullptr;

    int intParam = 0;
    void* objParam = nullptr;
};