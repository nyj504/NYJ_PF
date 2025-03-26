#include "Framework.h"

EquipManager::EquipManager()
{
	vector<string> types = { "Wood", "Stone", "Iron", "Gold", "Diamond" };

	vector<string> names = 
	{
		"Boots_Left", "Boots_Right",
		"Leggings_Left", "Leggings_Right",
		"ChestPlate", "Chestplate_LeftArm", "Chestplate_RightArm",
		"Helmet",
		"Sword", "Axe", "PickAxe", "Hoe", "Shovel"
	};

	leftBootsSocket = new Transform(); 
	rightBootsSocket = new Transform();

	leftLegSocket = new Transform();
	rightLegSocket = new Transform();

	chestPlateSocket = new Transform();
	leftArmSocket = new Transform();
	rightArmSocket = new Transform();

	helmetSocket = new Transform();

	weaponSocket = new Transform();

	singleBlock = new Cube();
	singleBlock->SetTag("Item");
	singleBlock->Load();
	singleBlock->SetParent(weaponSocket);

	multiBlock = new Cube();
	multiBlock->SetTag("Item");
	multiBlock->Load();
	multiBlock->ApplyBlockUVMapping();
	multiBlock->SetParent(weaponSocket);

	item = new Quad(Vector2(1, 1));
	item->SetTag("Item");
	item->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	item->Load();
	item->SetParent(weaponSocket);

	string weapon = "Bow";
	Model* model = new Model(weapon);
	model->Load();
	model->SetParent(weaponSocket);
	equipments[weapon] = model;

	for (const string& type : types)
	{
		for (int i = 0; i < names.size(); ++i)
		{
			if ((type == "Wood" || type == "Stone") && i < 8)
				continue;
	
			string key = type + names[i];
	
			Model* model = new Model(key);
			model->Load();
	
			switch (i)
			{
			case 0:
				model->SetParent(leftBootsSocket);
				break;
			case 1:
				model->SetParent(rightBootsSocket);
				break;
			case 2:
				model->SetParent(leftLegSocket);
				break;
			case 3:
				model->SetParent(rightLegSocket);
				break;
			case 4:
				model->SetParent(chestPlateSocket);
				break;
			case 5:
				model->SetParent(leftArmSocket);
				break;
			case 6:
				model->SetParent(rightArmSocket);
				break;
			case 7:
				model->SetParent(helmetSocket);
				break;
			default:
				model->SetParent(weaponSocket);
				break;
			}
			equipments[key] = model;
		}
	}
}

EquipManager::~EquipManager()
{
	for (const pair<string, Model*>equipment : equipments)
		delete equipment.second;
	equipments.clear();
	
	delete singleBlock;
	delete multiBlock;
	delete item;

	delete modelAnimator;
	delete weaponSocket;

	delete leftBootsSocket;
	delete rightBootsSocket;

	delete leftLegSocket;
	delete rightLegSocket;

	delete chestPlateSocket;
	delete leftArmSocket;
	delete rightArmSocket;

	delete helmetSocket;
}

void EquipManager::Update()
{
	if (isEquipWeapon)
	{
		weaponSocket->SetWorld(modelAnimator->GetTransformByNode(18));
		weapon->UpdateWorld();
	}
	if (isEquipItem)
	{
		weaponSocket->SetWorld(modelAnimator->GetTransformByNode(18));
		if (singleBlock->IsActive())
			singleBlock->UpdateWorld();
		else if (multiBlock->IsActive())
			multiBlock->UpdateWorld();
		else if (item->IsActive())
			item->UpdateWorld();
	}
	if (isEquipHelmet)
	{
		helmetSocket->SetWorld(modelAnimator->GetTransformByNode(12));
		helmet->UpdateWorld();
	}
	if (isEquipChestPlate)
	{
		chestPlateSocket->SetWorld(modelAnimator->GetTransformByNode(10));
		leftArmSocket->SetWorld(modelAnimator->GetTransformByNode(13));
		rightArmSocket->SetWorld(modelAnimator->GetTransformByNode(16));

		chestPlate->UpdateWorld();
		leftArm->UpdateWorld();
		rightArm->UpdateWorld();
	}
	if (isEquipLeggings)
	{
		leftLegSocket->SetWorld(modelAnimator->GetTransformByNode(3));
		rightLegSocket->SetWorld(modelAnimator->GetTransformByNode(6));

		leftLeg->UpdateWorld();
		rightLeg->UpdateWorld();
	}
	if (isEquipBoots)
	{
		leftBootsSocket->SetWorld(modelAnimator->GetTransformByNode(5));
		rightBootsSocket->SetWorld(modelAnimator->GetTransformByNode(8));

		leftBoots->UpdateWorld();
		rightBoots->UpdateWorld();
	}
}

void EquipManager::Render()
{
	if (isEquipWeapon)
	{
		weapon->Render();
	}
	if (isEquipItem)
	{
		if (singleBlock->IsActive())
			singleBlock->Render();
		else if (multiBlock->IsActive())
			multiBlock->Render();
		else if (item->IsActive())
		{
			Environment::Get()->SetAlphaBlend(true);
			item->Render();
			Environment::Get()->SetAlphaBlend(false);
		}
	}
	if (isEquipHelmet)
	{
		helmet->Render();
	}
	if (isEquipChestPlate)
	{
		chestPlate->Render();
		leftArm->Render();
		rightArm->Render();
	}
	if (isEquipLeggings)
	{
		leftLeg->Render();
		rightLeg->Render();
	}
	if (isEquipBoots)
	{
		leftBoots->Render();
		rightBoots->Render();
	}
}

void EquipManager::EquipTotalArmor(const unordered_map<AmoType, UINT>& equips)
{
	curDefense = 0;

	for (const pair<AmoType, UINT>& equip : equips)
	{
		if (equip.second == 0)
		{
			UnequipArmor(equip.first);
			continue;
		}

		EquipmentData data = DataManager::Get()->GetEquipmentData(equip.second);

		curDefense += data.defense;

		EquipArmor(equip.first, data.equipType); 
	}

	UIManager::Get()->GetArmorBar()->UpdateArmorBar(curDefense);
}

void EquipManager::EquipArmor(AmoType type, string name)
{
	vector<string> slotNames;

	switch (type)
	{
	case AmoType::HELMET:
		slotNames = { "Helmet" };
		break;
	case AmoType::CHESTPLATE:
		slotNames = { "ChestPlate", "Chestplate_LeftArm", "Chestplate_RightArm" };
		break;
	case AmoType::LEGGINGS:
		slotNames = { "Leggings_Left", "Leggings_Right" };
		break;
	case AmoType::BOOTS:
		slotNames = { "Boots_Left", "Boots_Right" };
		break;
	}

	for (const string& partName : slotNames)
	{
		string key = name + partName;

		auto it = equipments.find(key);
		if (it != equipments.end())
		{
			it->second->SetActive(true);
			if (type == AmoType::HELMET)
			{
				isEquipHelmet = true;
				helmet = it->second;
			}
			if (type == AmoType::CHESTPLATE)
			{
				isEquipChestPlate = true;
				if (partName == "ChestPlate")
					chestPlate = it->second;
				else if (partName == "Chestplate_LeftArm")
					leftArm = it->second;
				else if (partName == "Chestplate_RightArm")
					rightArm = it->second;
			}
			if (type == AmoType::LEGGINGS)
			{
				isEquipLeggings = true;
				if (partName == "Leggings_Left")
					leftLeg = it->second;
				else if (partName == "Leggings_Right")
					rightLeg = it->second;
			}
			if (type == AmoType::BOOTS)
			{
				isEquipBoots = true;
				if (partName == "Boots_Left")
					leftBoots = it->second;
				else if (partName == "Boots_Right")
					rightBoots = it->second;
			}
		}
	}
}

void EquipManager::UnequipArmor(AmoType type)
{
	switch (type)
	{
	case AmoType::HELMET:
		isEquipHelmet = false;
		helmet = nullptr;
		break;
	case AmoType::CHESTPLATE:
		isEquipChestPlate = false;
		chestPlate = leftArm = rightArm = nullptr;
		break;
	case AmoType::LEGGINGS:
		isEquipLeggings = false;
		leftLeg = rightLeg = nullptr;
		break;
	case AmoType::BOOTS:
		isEquipBoots = false;
		leftBoots = rightBoots = nullptr;
		break;
	}
}

void EquipManager::EquipWeapon(WeaponType type, string name)
{
	isEquipItem = false;

	string category = {};

	switch (type)
	{
	case WeaponType::SWORD:
		category = "Sword";
		break;
	case WeaponType::AXE:
		category = "Axe";
		break;
	case WeaponType::PICKAXE:
		category = "PickAxe";
		break;
	case WeaponType::SHOVEL:
		category = "Shovel";
		break;
	case WeaponType::BOW:
		break;
	}
	isEquipWeapon = true;

	weapon = equipments[name + category];
}

void EquipManager::EquipItem(ItemType type, string path)
{
	isEquipWeapon = false;

	string itemPath = "Resources/Textures/Itemx2/" + path + ".png";

	singleBlock->SetActive(false);
	multiBlock->SetActive(false);
	item->SetActive(false);

	switch (type)
	{
	case ItemType::SINGLE:
		singleBlock->GetMaterial()->SetDiffuseMap(Utility::ToWString(itemPath));
		singleBlock->SetActive(true);
		break;
	case ItemType::MULTI:
		multiBlock->GetMaterial()->SetDiffuseMap(Utility::ToWString(itemPath));
		multiBlock->SetActive(true);
		break;
	case ItemType::QUAD:
		item->GetMaterial()->SetDiffuseMap(Utility::ToWString(itemPath));
		item->SetActive(true);
		break;
	case ItemType::MODEL:
		break;
	default:
		break;
	}

	isEquipItem = true;
}
