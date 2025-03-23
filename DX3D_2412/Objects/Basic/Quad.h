#pragma once

class Quad : public GameObject<VertexUV>
{
public:
    Quad(Vector2 size);
    Quad(wstring texture);
    Quad(string name, Vector2 size);

protected:
    Vector2 size;
};