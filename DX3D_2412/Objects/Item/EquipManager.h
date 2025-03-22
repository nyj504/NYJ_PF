#pragma once
class EquipManager : public Singleton<EquipManager>
{
private:
	friend class Singleton;

	EquipManager();
	~EquipManager();

public:
	void Update();
	void Render();

	void EquipArmor(AmoType type, string name);

	void EquipWeapon(string name);
	void SetTarget(ModelAnimator* modelAnimator) { this->modelAnimator = modelAnimator; }

private:
	bool isEquipHelmet = false;
	bool isEquipChestPlate = false;
	bool isEquipLeggings = false;
	bool isEquipBoots = false;

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