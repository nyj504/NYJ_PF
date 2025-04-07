#include "Framework.h"

Button::Button(wstring textureFile)
    : Quad(textureFile)
{
}

Button::Button(Vector2 size) : Quad(size)
{
}

Button::~Button()
{
}

void Button::Render()
{
    if (!IsActive()) return;

    Quad::Render();

    if (text.size() == 0) return;

    FONT->SetColor(textColor);
    FONT->SetStyle(textStyle);
    FONT->RenderText(text, GetGlobalPosition());
}


void Button::Update()
{
    if (!IsActive()) return;
    
    ClickEvent();
   
    Quad::UpdateWorld();
}

bool Button::IsCollision(const Vector3& point)
{
    float left = GetGlobalPosition().x - size.x * 0.5f * GetGlobalScale().x;
    float right = GetGlobalPosition().x + size.x * 0.5f * GetGlobalScale().x;
    float top = GetGlobalPosition().y + size.y * 0.5f * GetGlobalScale().y;
    float bottom = GetGlobalPosition().y - size.y * 0.5f * GetGlobalScale().y;

    if (point.x > left && point.x < right)
    {
        if (point.y > bottom && point.y < top)
            return true;
    }

    return false;
}

void Button::ClickEvent()
{
    if (IsCollision(mousePos))
    {
        if (KEY->Down(VK_LBUTTON))
        {
            isDownCheck = true;
            Audio::Get()->Play("click");
        }
            
        if (KEY->Press(VK_LBUTTON))
            state = DOWN;
        else
            state = OVER;

        if (isDownCheck && KEY->Up(VK_LBUTTON))
        {
            if (event)
                event();
            if (intParamEvent)
                intParamEvent(intParam);
            if (objParamEvent)
                objParamEvent(objParam);

            isDownCheck = false;
        }
    }
    else
    {
        state = NONE;

        if (KEY->Up(VK_LBUTTON))
            isDownCheck = false;
    }

    switch (state)
    {
    case Button::NONE:
        material->GetData()->diffuse = NONE_COLOR;
        break;
    case Button::DOWN:
        material->GetData()->diffuse = DOWN_COLOR;
        break;
    case Button::OVER:
        material->GetData()->diffuse = OVER_COLOR;
        break;
    }
}
