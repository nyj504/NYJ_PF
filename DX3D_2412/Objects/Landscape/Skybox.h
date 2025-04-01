#pragma once

class Skybox : public Sphere
{
public:
    Skybox(wstring textureFile);
    ~Skybox();

    void Render();
    void Edit();

private:
    Texture* cubeMap;

    RasterizerState* rasterizerState[2];
    DepthStencilState* depthStencilState[2];
};