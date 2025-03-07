#pragma once

class InstancingScene : public Scene
{
private:
    const UINT COUNT = 2000;

    struct Data
    {
        Matrix transform;
        Float4 color;
    };

public:
    InstancingScene();
    ~InstancingScene();
        
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    //vector<Quad*> quads;
    Quad* quad;

    vector<Data> instanceData;
    VertexBuffer* instanceBuffer;
};