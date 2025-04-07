#include "Framework.h"

DataManager::DataManager()
{
    LoadItemData();
    LoadEquipmentData();
    LoadCharacterData();
}

DataManager::~DataManager()
{
}

void DataManager::LoadItemData()
{
    string fileName = "Resources/Tables/ItemTable.csv";

    ifstream loadFile(fileName);

    string line;

    getline(loadFile, line);

    while (getline(loadFile, line))
    {
        vector<string> rowData = Utility::SplitString(line, ",");

        ItemData data;
        data.key = stoi(rowData[0]);
        data.name = rowData[1];
        data.image = rowData[2];
        data.soundType = rowData[3];
       
        string typeStr = rowData[4];
        if (typeStr == "SWORD") data.weakType = WeaponType::SWORD;
        else if (typeStr == "AXE") data.weakType = WeaponType::AXE;
        else if (typeStr == "PICKAXE") data.weakType = WeaponType::PICKAXE;
        else if (typeStr == "SHOVEL") data.weakType = WeaponType::SHOVEL;
        else if (typeStr == "BOW") data.weakType = WeaponType::BOW;
        else data.weakType = WeaponType::NONE;
       
        data.particle = rowData[5];
        data.canBuild = stoi(rowData[6]);
        data.canCraft = stoi(rowData[7]);
        data.textureType = rowData[8];
        data.craftingType = rowData[9];
        data.startUV = stoi(rowData[10]);
        data.endUV = stoi(rowData[11]);
        data.dropItemKey = stoi(rowData[12]);
        data.dropsAmount = stoi(rowData[13]);
        data.serialKey = rowData[14];
        data.itemType = rowData[15];
        data.isFuel = stoi(rowData[16]);
        data.isSmeltable = stoi(rowData[17]);
        data.craftAmount = stoi(rowData[18]);

        itemDatas[data.key] = data;

        if (!data.serialKey.empty() && data.serialKey != "None")
        {
            craftingRecipes[data.serialKey] = { data.key, data.craftAmount };
        }
    }
}

void DataManager::LoadEquipmentData()
{
    string fileName = "Resources/Tables/EquipmentTable.csv";

    ifstream loadFile(fileName);

    string line;

    getline(loadFile, line);

    while (getline(loadFile, line))
    {
        vector<string> rowData = Utility::SplitString(line, ",");

        EquipmentData data;
        data.key = stoi(rowData[0]);
        data.equipType = rowData[1];
        data.equipParts = rowData[2];
        data.atk = stoi(rowData[3]);
        data.defense = stoi(rowData[4]);

        equipmentDatas[data.key] = data;
    }
}

void DataManager::LoadCharacterData()
{
    string fileName = "Resources/Tables/CharacterTable.csv";

    ifstream loadFile(fileName);

    string line;

    getline(loadFile, line);

    while (getline(loadFile, line))
    {
        vector<string> rowData = Utility::SplitString(line, ",");

        CharacterData data;
        data.key = stoi(rowData[0]);
        data.name = rowData[1];
        data.maxHp = stoi(rowData[2]);
        data.atk = stoi(rowData[3]);
        data.moveSpeed = stof(rowData[4]);
        data.range = stoi(rowData[5]);
        data.dropItemKey = stoi(rowData[6]);
        data.dropItemAmount = stoi(rowData[7]);

        characterDatas[data.key] = data;
    }
}

