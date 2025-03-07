#pragma once
class ObjectModel : public Transform
{
public:
    ObjectModel();
    ~ObjectModel();

    void Update();
    void Render();
    void ApplySkinUVMapping();
    void ApplyObjectUVMapping();

    void Edit();

    void Save(string path);
    void Load(string path);

    void SaveDialog();
    void LoadDialog();

    void AddComponent(int count);
    void ResetModel();

private:
    bool isCharacter = false;
    string atlasTexturePath = "";
    string name;
    string editName;
    string editTag;
    string file;
    string projectPath;

    vector<Cube*>components;
};