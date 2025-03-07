#pragma once

class EventManager : public Singleton<EventManager>
{
private:
    friend class Singleton;

    EventManager() = default;
    ~EventManager() = default;

public:
    void AddEvent(string key, Event event);
    void AddIntParamEvent(string key, IntParamEvent event);    

    void ExcuteEvent(string key);
    void ExcuteIntParamEvent(string key, int param);
    void AddMouseWheelEvent(function<void(int)> callback)
    {
        mouseWheelCallbacks.push_back(callback);
    }

    void TriggerMouseWheelEvent(int delta)
    {
        for (auto& callback : mouseWheelCallbacks)
        {
            callback(delta);
        }
    }
private:
    unordered_map<string, vector<IntParamEvent>> intParamEvents;
    unordered_map<string, vector<Event>> events;
    vector<function<void(int)>> mouseWheelCallbacks;
};