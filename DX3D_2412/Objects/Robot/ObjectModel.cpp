#include "Framework.h"

ObjectModel::ObjectModel()
{
    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

ObjectModel::~ObjectModel()
{
    for (Cube* component : components)
        delete component;
}

void ObjectModel::Update()
{
    ApplySkinUVMapping();
    ApplyObjectUVMapping();
    for (Cube* component : components)
        component->UpdateWorld();
}

void ObjectModel::Render()
{
    for (Cube* component : components)
        component->Render();
}

void ObjectModel::ApplySkinUVMapping()
{
    if (!isCharacter) return; 

    int atlasSize = 128; 
    wstring diffuseMap = L"";

    for (Cube* component : components)
    {
        string tag = component->GetEditTag();

        if (tag == "Head" || component->GetTag() == "Head")
        {
            component->ApplyCharacterUVMapping({ 16, 16 }, { 16, 16, 16 }, atlasSize);
            diffuseMap = component->GetMaterial()->GetDiffuseMap()->GetFile();
        }
        else if (tag == "Body" || component->GetTag() == "Body")
        {
            component->ApplyCharacterUVMapping({ 40, 40 }, { 16, 24, 8 }, atlasSize);
        }
        else if (tag == "RightArm" || component->GetTag() == "RightArm")
        {
            component->ApplyCharacterUVMapping({ 88, 40 }, { 8, 24, 8 }, atlasSize);
        }
        else if (tag == "LeftArm" || component->GetTag() == "LeftArm")
        {
            component->ApplyCharacterUVMapping({ 88, 40 }, { 8, 24, 8 }, atlasSize);
        }
        else if (tag == "RightLeg" || component->GetTag() == "RightLeg")
        {
            component->ApplyCharacterUVMapping({ 8, 40 }, { 8, 24, 8 }, atlasSize);
        }
        else if (tag == "LeftLeg" || component->GetTag() == "LeftLeg")
        {
            component->ApplyCharacterUVMapping({ 8, 40 }, { 8, 24, 8 }, atlasSize);
        }
        component->GetMesh()->UpdateVertices();

        component->GetMaterial()->SetDiffuseMap(diffuseMap);
    }
}

void ObjectModel::ApplyObjectUVMapping()
{
    if (isCharacter || editName == "torch") return;

    int atlasSize = 128;
    wstring diffuseMap = L"";

    for (Cube* component : components)
    {
        string tag = component->GetEditTag();

        if (tag == "Head" || component->GetTag() == "Head")
        {
            component->ApplyCharacterUVMapping({ 28, 28 }, { 28, 9, 28 }, atlasSize);
            diffuseMap = component->GetMaterial()->GetDiffuseMap()->GetFile();

            component->GetMaterial()->SetDiffuseMap(diffuseMap);
        }
        else if (tag == "Body" || component->GetTag() == "Body")
        {
            component->ApplyCharacterUVMapping({ 28, 66 }, { 28, 19, 28 }, atlasSize);

            component->GetMaterial()->SetDiffuseMap(diffuseMap);
        }
      
        component->GetMesh()->UpdateVertices();
        
    }
}


void ObjectModel::Edit()
{
    string title = name + "_Model";

    if (ImGui::TreeNode(title.c_str()))
    {
        char str[128];
        strcpy_s(str, 128, editName.c_str());
        ImGui::InputText("Name", str, 128);
        editName = str;

        static int componentCount = 1;
        ImGui::InputInt("Component Count", &componentCount);
        if (componentCount < 1) componentCount = 1;

        if (ImGui::Button("Add Component"))
        {
            AddComponent(componentCount);
        }

        ImGui::Checkbox("Character Mode (Apply Skin UV)", &isCharacter);
        {
            if (isCharacter)
            {
                ApplySkinUVMapping(); 
            }
        }

        for (Cube* component : components)
        {
            if (ImGui::TreeNode(component->GetTag().c_str()))
            {
                char tagStr[128];
                strcpy_s(tagStr, 128, component->GetEditTag().c_str());
                ImGui::InputText("Tag", tagStr, 128);
                component->SetEditTag(tagStr);
                
                Float3 position = component->GetLocalPosition();
                Float3 rotation = {
                    XMConvertToDegrees(component->GetLocalRotation().x),
                    XMConvertToDegrees(component->GetLocalRotation().y),
                    XMConvertToDegrees(component->GetLocalRotation().z) };
                Float3 scale = component->GetLocalScale();

                ImGui::DragFloat3("Position", (float*)&position, 0.1f);
                ImGui::DragFloat3("Rotation", (float*)&rotation, 0.1f, -180, 180);
                ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);

                component->SetLocalPosition(position);
                component->SetLocalRotation({
                    XMConvertToRadians(rotation.x),
                    XMConvertToRadians(rotation.y),
                    XMConvertToRadians(rotation.z) });
                component->SetLocalScale(scale);

                component->GetMaterial()->SetName(tagStr);

                if (ImGui::CollapsingHeader("Material"))
                {
                    component->GetMaterial()->Edit();
                }
                ImGui::TreePop();

            }
        }
        SaveDialog();
        LoadDialog();

        if (ImGui::Button("Reset Model"))
        {
            ResetModel();
        }

        ImGui::TreePop();
    }
}

void ObjectModel::Save(string path)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
   
    tinyxml2::XMLElement* root = document->NewElement("Components");
    document->InsertFirstChild(root);

    root->SetAttribute("IsPlayer", isCharacter);
    root->SetAttribute("Path", editName.c_str());

    for (Cube* component : components)
    {
        tinyxml2::XMLElement* compTransform = document->NewElement("Component");
        compTransform->SetAttribute("Tag", component->GetEditTag().c_str());
        compTransform->SetAttribute("PosX", component->GetLocalPosition().x);
        compTransform->SetAttribute("PosY", component->GetLocalPosition().y);
        compTransform->SetAttribute("PosZ", component->GetLocalPosition().z);
        compTransform->SetAttribute("RotX", XMConvertToDegrees(component->GetLocalRotation().x));
        compTransform->SetAttribute("RotY", XMConvertToDegrees(component->GetLocalRotation().y));
        compTransform->SetAttribute("RotZ", XMConvertToDegrees(component->GetLocalRotation().z));
        compTransform->SetAttribute("ScaleX", component->GetLocalScale().x);
        compTransform->SetAttribute("ScaleY", component->GetLocalScale().y);
        compTransform->SetAttribute("ScaleZ", component->GetLocalScale().z);
 
        root->InsertEndChild(compTransform);
    }

    document->SaveFile(path.c_str());
}


void ObjectModel::Load(string path)
{
    this->file = path;
    tinyxml2::XMLDocument document;
    if (document.LoadFile(file.c_str()) != tinyxml2::XML_SUCCESS)
        return;

    tinyxml2::XMLElement* root = document.FirstChildElement("Components");

    if (root->Attribute("Path"))
    {
        editName = root->Attribute("Path");
    }

    if (!root) return;

    isCharacter = root->BoolAttribute("IsPlayer", false);
    components.clear();

    for (tinyxml2::XMLElement* compTransform = root->FirstChildElement("Component");
        compTransform != nullptr;
        compTransform = compTransform->NextSiblingElement("Component"))
    {
        Cube* newComponent = new Cube();

        string tagName = "DefaultComponent";

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
        
        components.push_back(newComponent);
    }

    if (isCharacter)
    {
        ApplySkinUVMapping();
    }
}

void ObjectModel::SaveDialog()
{
    string key = "Save";

    if (ImGui::Button("Save"))
    {
        if (file.empty())
        {
            Save("Resources/Models/" + editName + ".model");

            for (Cube* component : components)
            {
                component->GetMaterial()->Save("Resources/Materials/" + editName + component->GetEditTag() + ".mat");
            }
        }
        else
            Save(file);
    }

    ImGui::SameLine();

    if (ImGui::Button("SaveAs"))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".model");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Save(file);

            for (Cube* component : components)
            {
                component->GetMaterial()->Save("Resources/Materials/" + editName + component->GetEditTag() + ".mat");
            }
        }

        DIALOG->Close();
    }
}

void ObjectModel::LoadDialog()
{
    string key = "Load";

    if (ImGui::Button("Load"))
    {
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".model");
    }

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Load(file);
        }

        DIALOG->Close();
    }
}

void ObjectModel::AddComponent(int count)
{
    for (int i = 0; i < count; i++)
    {
        Cube* newComponent = new Cube();

        string defaultName = "Component" + to_string(components.size());
        newComponent->SetTag(defaultName);
    
        newComponent->SetLocalPosition({ 0, 0, 0 });
        newComponent->SetLocalRotation({ 0, 0, 0 });
        newComponent->SetLocalScale({ 1, 1, 1 });

        components.push_back(newComponent);
    }
}

void ObjectModel::ResetModel()
{
    components.clear(); 
    editName = "NewModel";  
    isCharacter = false;
}
