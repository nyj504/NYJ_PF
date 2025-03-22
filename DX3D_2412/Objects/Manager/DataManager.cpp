#include "Framework.h"

DataManager::DataManager()
{
    LoadItemData();
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
        data.hp = stoi(rowData[3]);
        data.canBuild = stoi(rowData[4]);
        data.canCraft = stoi(rowData[5]);
        data.textureType = rowData[6];
        data.craftingType = rowData[7];
        data.startUV = stoi(rowData[8]);
        data.endUV = stoi(rowData[9]);
        data.dropItemKey = stoi(rowData[10]);
        data.dropsAmount = stoi(rowData[11]);
        data.serialKey = rowData[12];
        data.itemType = rowData[13];
        data.isFuel = stoi(rowData[14]);
        data.isSmeltable = stoi(rowData[15]);
        data.craftAmount = stoi(rowData[16]);

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

