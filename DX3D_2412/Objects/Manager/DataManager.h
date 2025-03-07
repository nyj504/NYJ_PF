#pragma once

struct ItemData
{
    UINT key;
    string name;
    string image;
    UINT hp;
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

    const unordered_map<UINT, ItemData>& GetItemDatas() const { return itemDatas; }
    const unordered_map<string, pair<UINT, UINT>>& GetCraftingRecipes() const { return craftingRecipes; }

  
    void LoadItemData();

private:
    unordered_map<UINT, ItemData> itemDatas;
    unordered_map<string, pair<UINT, UINT>> craftingRecipes;
};