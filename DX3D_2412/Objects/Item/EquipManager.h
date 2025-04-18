#pragma once
class EquipManager : public Singleton<EquipManager>
{
private:
	const int HAND_INDEX = 18;
	const int HEAD_INDEX = 12;
	const int CHEST_INDEX = 10;
private:
	friend class Singleton;

	EquipManager();
	~EquipManager();

public:
	void Update();
	void Render();

	void EquipTotalArmor(const unordered_map<AmoType, UINT>& equips);

	void EquipWeapon(WeaponType type, string name);
	void UnEquipWeapon();
	void EquipItem(ItemType type, string path);

	void SetTarget(ModelAnimator* modelAnimator) { this->modelAnimator = modelAnimator; }

private:
	void EquipArmor(AmoType type, string name);
	void UnequipArmor(AmoType type);

private:
	UINT curDefense = 0;
	bool isEquipHelmet = false;
	bool isEquipChestPlate = false;
	bool isEquipLeggings = false;
	bool isEquipBoots = false;
	bool isEquipWeapon = false;
	bool isEquipItem = false;

	Cube* singleBlock = nullptr;
	Cube* multiBlock = nullptr;
	Quad* item = nullptr;

	unordered_map<string, Model*>equipments;
	ModelAnimator* modelAnimator = nullptr;

	Transform* leftBootsSocket;
	Transform* rightBootsSocket;
	Model* leftBoots = nullptr;
	Model* rightBoots = nullptr;

	Transform* leftLegSocket;
	Transform* rightLegSocket;
	Model* leftLeg = nullptr;
	Model* rightLeg = nullptr;

	Transform* chestPlateSocket;
	Transform* leftArmSocket;
	Transform* rightArmSocket;
	Model* chestPlate = nullptr;
	Model* leftArm = nullptr;
	Model* rightArm = nullptr;

	Transform* helmetSocket;
	Model* helmet = nullptr;

	Transform* weaponSocket;
	Model* weapon = nullptr;
};