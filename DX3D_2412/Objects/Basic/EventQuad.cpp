#include "Framework.h"

EventQuad::EventQuad(const wstring& texturePath)
	: Quad(texturePath)
{
}

EventQuad::~EventQuad()
{
}

void EventQuad::TriggerEvent()
{
	if (event) event();
}

void EventQuad::TriggerIntParamEvent()
{
	if (intParamEvent) intParamEvent(intParam);
}

void EventQuad::TriggerObjParamEvent()
{
	if (objParamEvent) objParamEvent(objParam);
}
