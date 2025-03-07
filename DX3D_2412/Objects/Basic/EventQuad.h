#pragma once
class EventQuad : public Quad
{
public:
    EventQuad(const wstring& texturePath);
    ~EventQuad();

    void SetEvent(function<void()> event) { this->event = event; }
    void SetIntParamEvent(function<void(int)> event) { intParamEvent = event; }
    void SetObjParamEvent(function<void(void*)> event) { objParamEvent = event; }

    void SetIntParam(int param) { intParam = param; }
    void SetObjParam(void* param) { objParam = param; }

    void TriggerEvent();
    void TriggerIntParamEvent();
    void TriggerObjParamEvent();

private:
    function<void()> event = nullptr;                
    function<void(int)> intParamEvent = nullptr;     
    function<void(void*)> objParamEvent = nullptr;   

    int intParam = 0;        
    void* objParam = nullptr; 
};