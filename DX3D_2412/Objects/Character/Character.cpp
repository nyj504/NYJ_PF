#include "Framework.h"

Character::Character(string path)
{
	Load(path);
    ApplySkinUVMapping();

    boxCollider = new BoxCollider(Vector3(0.8f, 2.0f, 0.6f));
    boxCollider->SetLocalPosition(0, 0.8f, 0);
    boxCollider->SetParent(this);
    boxCollider->UpdateWorld();
}

Character::~Character()
{
    for (Cube* bodyComponent : bodyComponents)
        delete bodyComponent;

    delete boxCollider;
}

void Character::Update()
{
    for (Cube* bodyComponent : bodyComponents)
        bodyComponent->Update();

    boxCollider->UpdateWorld();
}

void Character::Render()
{
    for (Cube* bodyComponent : bodyComponents)
        bodyComponent->Render();

    boxCollider->Render();
}

void Character::Load(string path)
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
            XMConvertToRadians(compTransform->FloatAttribute("RotY") + 180.0f),
            XMConvertToRadians(compTransform->FloatAttribute("RotZ")) });

        newComponent->SetLocalScale({
            compTransform->FloatAttribute("ScaleX"),
            compTransform->FloatAttribute("ScaleY"),
            compTransform->FloatAttribute("ScaleZ") });

        newComponent->GetMaterial()->Load("Resources/Materials/" + editName + tagName + ".mat");
        newComponent->GetCollider()->SetActive(false);
        newComponent->UpdateWorld();
       
        bodyComponents.push_back(newComponent);
    }
}

void Character::ApplySkinUVMapping()
{
    int atlasSize = 128;
    wstring diffuseMap = L"";

    for (Cube* bodyComponent : bodyComponents)
    {
        string tag = bodyComponent->GetEditTag();

        if (bodyComponent->GetTag() == "Head")
        {
            bodyComponent->ApplyCharacterUVMapping({ 16, 16 }, { 16, 16, 16 }, atlasSize);
        }
        else if (bodyComponent->GetTag() == "Body")
        {
            bodyComponent->ApplyCharacterUVMapping({ 40, 40 }, { 16, 24, 8 }, atlasSize);
        }
        else if (bodyComponent->GetTag() == "RightArm")
        {
            bodyComponent->ApplyCharacterUVMapping({ 88, 40 }, { 8, 24, 8 }, atlasSize);
        }
        else if (bodyComponent->GetTag() == "LeftArm")
        {
            bodyComponent->ApplyCharacterUVMapping({ 88, 40 }, { 8, 24, 8 }, atlasSize);
        }
        else if (bodyComponent->GetTag() == "RightLeg")
        {
            bodyComponent->ApplyCharacterUVMapping({ 8, 40 }, { 8, 24, 8 }, atlasSize);
        }
        else if (bodyComponent->GetTag() == "LeftLeg")
        {
            bodyComponent->ApplyCharacterUVMapping({ 8, 40 }, { 8, 24, 8 }, atlasSize);
        }
    }
}