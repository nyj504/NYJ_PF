#pragma once

struct ItemData
{
    UINT key;
    string name;
    string image;
    WeaponType weakType;
    string particle;
    bool canBuild;
    bool canCraft;
    string textureType;
    string craftingType;
    UINT startUV;
    UINT endUV;
    UINT dropItemKey;
    UINT dropsAmount;
    string serialKey;
    string itemType;
    bool isFuel;
    bool isSmeltable;
    UINT craftAmount;
};

struct EquipmentData
{
    UINT key;
    string equipType;
    string equipParts;
    UINT atk;
    UINT defense;
};

struct CharacterData
{
    UINT key;
    string name;
    UINT maxHp;
    UINT atk;
    float moveSpeed;
    UINT range;
    UINT dropItemKey;
    UINT dropItemAmount;
};

class DataManager : public Singleton<DataManager>
{
private:
    friend class Singleton;

    DataManager();
    ~DataManager();

public:
    ItemData GetItemData(UINT key)
    {
        if (itemDatas.count(key) == 0)
            assert(false);

        return itemDatas[key];
    }
    EquipmentData GetEquipmentData(UINT key)
    {
        if (equipmentDatas.count(key) == 0)
            assert(false);

        return equipmentDatas[key];
    }
    CharacterData GetCharacterData(UINT key)
    {
        if (characterDatas.count(key) == 0)
            assert(false);

        return characterDatas[key];
    }

    const unordered_map<UINT, ItemData>& GetItemDatas() const { return itemDatas; }
    const unordered_map<UINT, EquipmentData>& GetEquipmentDatas() const { return equipmentDatas; }
    const unordered_map<UINT, CharacterData>& GetCharacterDatas() const { return characterDatas; }
    const unordered_map<string, pair<UINT, UINT>>& GetCraftingRecipes() const { return craftingRecipes; }

  
    void LoadItemData();
    void LoadEquipmentData();
    void LoadCharacterData();

private:
    unordered_map<UINT, ItemData> itemDatas;
    unordered_map<UINT, EquipmentData> equipmentDatas;
    unordered_map<UINT, CharacterData> characterDatas;

    unordered_map<string, pair<UINT, UINT>> craftingRecipes;
};