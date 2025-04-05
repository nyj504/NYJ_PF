#include "Framework.h"

ItemManager::ItemManager()
{
	dropItems.reserve(MAX_DROPITEM_COUNT);
	dropSingleBlocks.reserve(MAX_DROPITEM_COUNT);
	dropMultiBlocks.reserve(MAX_DROPITEM_COUNT);

	for (int i = 0; i < MAX_DROPITEM_COUNT; i++)
	{
		DropQuad* item = new DropQuad();
		dropItems.push_back(item);

		DropCube* singleBlock = new DropCube();
		dropSingleBlocks.push_back(singleBlock);

		DropCube* multiBlock = new DropCube(true);
		dropMultiBlocks.push_back(multiBlock);
	}

}

ItemManager::~ItemManager()
{
	for (DropQuad* item : dropItems)
		delete item;
	for (DropCube* singleBlock : dropSingleBlocks)
		delete singleBlock;
	for (DropCube* multiBlock : dropMultiBlocks)
		delete multiBlock;
}

void ItemManager::Update()
{
	for (DropQuad* item : dropItems)
	{
		item->Update();
	}
	for (DropCube* singleBlock : dropSingleBlocks)
	{
		singleBlock->Update();
	}
	for (DropCube* multiBlock : dropMultiBlocks)
	{
		multiBlock->Update();
	}
}

void ItemManager::Render()
{
	for (DropQuad* item : dropItems)
	{
		item->Render();
	}
	for (DropCube* singleBlock : dropSingleBlocks)
	{
		if (singleBlock->IsActive())
		{
			singleBlock->Render();
		}
	}
	for (DropCube* multiBlock : dropMultiBlocks)
	{
		if (multiBlock->IsActive())
		{
			multiBlock->Render();
		}
	}
}

void ItemManager::DropItem(UINT key, Vector3 pos, UINT amounts)
{
	ItemData data = DataManager::Get()->GetItemData(key);
	
	ItemType type = ItemType::SINGLE;
	if (data.textureType == "Quad")
		type = ItemType::QUAD;
	else if (data.textureType == "Normal")
		type = ItemType::SINGLE;
	else
		type = ItemType::MULTI;

	UINT count = 0;
	float offset = 0.05f;
	string itemPath = "Resources/Textures/Itemx2/" + data.image + ".png";

	switch (type)
	{
	case ItemType::SINGLE:
		for (DropCube* singleBlock : dropSingleBlocks)
		{
			if (!singleBlock->IsActive())
			{
				if (count >= amounts) return;
				singleBlock->SetImage(itemPath);
				singleBlock->SetKey(key);
				singleBlock->SetActive(true);
				singleBlock->SetLocalPosition(Vector3(pos.x + offset * count, pos.y, pos.z));
				singleBlock->UpdateWorld();

				count++;
			}
		}
		break;
	case ItemType::MULTI:
		for (DropCube* multiBlock : dropMultiBlocks)
		{
			if (!multiBlock->IsActive())
			{
				if (count >= amounts) return;
				multiBlock->SetImage(itemPath);
				multiBlock->SetKey(key);
				multiBlock->SetActive(true);
				multiBlock->SetLocalPosition(Vector3(pos.x + offset * count, pos.y, pos.z));
				multiBlock->UpdateWorld();

				count++;
			}
		}
		break;
	case ItemType::QUAD:
		for (DropQuad* item : dropItems)
		{
			if (!item->IsActive())
			{
				if (count >= amounts) return;
				item->SetImage(itemPath);
				item->SetKey(key);
				item->SetActive(true);
				item->SetLocalPosition(Vector3(pos.x + offset * count, pos.y, pos.z));
				item->UpdateWorld();

				count++;
			}
		}
		break;
	case ItemType::MODEL:
		break;
	default:
		break;
	}
}
