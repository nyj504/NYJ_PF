#pragma once

class ModelEquipScene : public Scene
{
public:
	ModelEquipScene();
	~ModelEquipScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	ModelAnimator* modelAnimator;
	
	Transform* leftBootsSocket;
	Transform* rightBootsSocket;

	Model* leftBoots;
	Model* rightBoots;

	Transform* leftLegSocket;
	Transform* rightLegSocket;

	Model* leftLeg;
	Model* rightLeg;

	Transform* chestPlateSocket;
	Transform* leftArmSocket;
	Transform* rightArmSocket;

	Model* chestPlate;
	Model* leftArm;
	Model* rightArm;
	
	Transform* helmetSocket;

	Model* helmet;
	
	Transform* weaponSocket;

	Model* weapon;
};