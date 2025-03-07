#include "Framework.h"

ComplexBlock::ComplexBlock(UINT key) : Block(key)
{
    if (key == 38)
    {
        for (Cube* component : components)
        {
            if (component->GetTag() == "Head")
            {
                component->ApplyCharacterUVMapping({ 28, 28 }, { 28, 9, 28 }, 128);
            }
            else if (component->GetTag() == "Body")
            {
                component->ApplyCharacterUVMapping({ 28, 66 }, { 28, 19, 28 }, 128);
            }
        }
    }
    boundingBoxCollider = new BoxCollider();
    boundingBoxCollider->SetParent(this);
    boundingBoxCollider->UpdateWorld();
}

ComplexBlock::~ComplexBlock()
{
    for (Cube* component : components)
        delete component;

    delete boundingBoxCollider;
}


void ComplexBlock::Load(string path)
{
    tinyxml2::XMLDocument document;
    if (document.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS)
        return;

    tinyxml2::XMLElement* root = document.FirstChildElement("Components");

    string editName = "";

    if (root->Attribute("Path"))
    {
        editName = root->Attribute("Path");
    }

    if (!root) return;

    for (tinyxml2::XMLElement* compTransform = root->FirstChildElement("Component");
        compTransform != nullptr;
        compTransform = compTransform->NextSiblingElement("Component"))
    {
        Cube* newComponent = new Cube();

        string tagName = "DefaultComponent";

        newComponent->SetParent(this);

        if (compTransform->Attribute("Tag"))
        {
            tagName = compTransform->Attribute("Tag");
            newComponent->SetTag(tagName);
        }

        newComponent->SetLocalPosition({
            compTransform->FloatAttribute("PosX"),
            compTransform->FloatAttribute("PosY"),
            compTransform->FloatAttribute("PosZ") });

        newComponent->SetLocalRotation({
            XMConvertToRadians(compTransform->FloatAttribute("RotX")),
            XMConvertToRadians(compTransform->FloatAttribute("RotY")),
            XMConvertToRadians(compTransform->FloatAttribute("RotZ")) });

        newComponent->SetLocalScale({
            compTransform->FloatAttribute("ScaleX"),
            compTransform->FloatAttribute("ScaleY"),
            compTransform->FloatAttribute("ScaleZ") });

        newComponent->GetMaterial()->Load("Resources/Materials/" + editName + tagName + ".mat");
        newComponent->UpdateWorld();

        components.push_back(newComponent);
    }
}
