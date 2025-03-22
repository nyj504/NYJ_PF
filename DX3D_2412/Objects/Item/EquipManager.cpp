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
	case AmoType::WEAPON:
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

void EquipManager::EquipWeapon(string name)
{
}
