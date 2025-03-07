#pragma once

class Material
{
private:
	enum MaterialType
	{
		DIFFUSE, SPECULAR, NORMAL
	};
public:
	Material(wstring shaderFile = L"Light/Light.hlsl");
	~Material();

	void Set();

	void Edit();
	void Save(string path);
	void Load(string path);

	void SetShader(wstring shaderFile);
	void SetVertexShader(wstring shaderFile);
	void SetPixelShader(wstring shaderFile);

	void SetDiffuseMap(wstring diffuseFile);
	void SetDiffuseMap(Texture* map) { diffuseMap = map; }

	void SetSpecularMap(wstring specularFile);
	void SetSpecularMap(Texture* map) { specularMap = map; }

	void SetNormalMap(wstring normalFile);
	void SetNormalMap(Texture* map) { normalMap = map; }

	Texture* GetDiffuseMap() { return diffuseMap; }
	
	MaterialBuffer::Data* GetData() { return buffer->GetData(); }
	void SetName(string name) { this->name = name; }
	string GetName() { return name; }
	string GetFile() { return file; }

private:
	void SelectMap(string name, MaterialType type);
	void UnselectMap(MaterialType type);

	void SaveDialog();
	void LoadDialog();

private:
	string name;
	string editName;
	string file;
	string projectPath;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	MaterialBuffer* buffer;
	
	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;
};