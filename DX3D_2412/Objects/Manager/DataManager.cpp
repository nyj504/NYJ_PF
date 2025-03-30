#include "Framework.h"

DataManager::DataManager()
{
    LoadItemData();
    LoadEquipmentData();
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
       
        string typeStr = rowData[3];
        if (typeStr == "SWORD") data.weakType = WeaponType::SWORD;
        else if (typeStr == "AXE") data.weakType = WeaponType::AXE;
        else if (typeStr == "PICKAXE") data.weakType = WeaponType::PICKAXE;
        else if (typeStr == "SHOVEL") data.weakType = WeaponType::SHOVEL;
        else if (typeStr == "BOW") data.weakType = WeaponType::BOW;
        else data.weakType = WeaponType::NONE;
       
        data.particle = rowData[4];
        data.canBuild = stoi(rowData[5]);
        data.canCraft = stoi(rowData[6]);
        data.textureType = rowData[7];
        data.craftingType = rowData[8];
        data.startUV = stoi(rowData[9]);
        data.endUV = stoi(rowData[10]);
        data.dropItemKey = stoi(rowData[11]);
        data.dropsAmount = stoi(rowData[12]);
        data.serialKey = rowData[13];
        data.itemType = rowData[14];
        data.isFuel = stoi(rowData[15]);
        data.isSmeltable = stoi(rowData[16]);
        data.craftAmount = stoi(rowData[17]);

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

