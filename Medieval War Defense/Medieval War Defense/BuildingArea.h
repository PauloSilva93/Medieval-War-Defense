#pragma once

#include "Object2D_Animated.h"
#include "Object2D_Static.h"
#include "Selection.h"
#include "Text.h"
#include "GameObject.h"
#include "ArcherTower.h"

class BuildingArea
{
public:
	// específico da BuildingArea
	Object2D_Animated towersMenu;
	Selection s;
	Shader* shader_Animation;
	Shader* shader_Static;
	Shader* shader_Animation_NL;
	Shader* shader_Static_NL;
	Text text;
	vector<GameObject*>* gameObjects;
	Texture towersMenuTex;
	float towersMenuSize[2] = { 0.15, 0.15 };
	bool towerSold;
	bool buildingIsUnderMouse;
	bool towerIsUnderMouse;
	bool towerIsShown;
	bool towerOverIsShown;
	bool buildingAreaVisible;
	bool towerPlaced;
	bool towerMenuOpened;
	bool towerMenuUpgradeOpened;
	float towersMenuPos[2];
	float towersMenuColor[4];
	float towersMenuUpgradePos[2];
	float towersMenuUpgradeSize[2] = { 0.12f, 0.2f };
	float buildingAreaPos[3];
	float buildingAreaSize[2] = { 0.05, 0.05 };
	float buildingAreaOverSize[2] = { 0.06, 0.06 };
	float buildingAreaRotation[3] = { 0.0, 0.0, 1.0 };
	float buildingAreaDegrees = 0.0f;
	float buildingAreaColor[4] = { 0.0, 0.0, 0.0, 1.0 };
	int towerLevel;
	float towerPos[3];
	float towerPosPrice[2];
	float towerUpg1PosPrice[2];
	float towerUpg2PosPrice[2];
	float towerIconSize[2] = { (towersMenuSize[0] / 10.0f) * 3.0f, (towersMenuSize[1] / 10.0f) * 3.0f };
	float towerIconPos[2];
	float towerUpg1IconPos[2];
	float towerUpg2IconPos[2];
	float towersMenuUpgradeIconPos[2];
	float towerUpg1IconSize[2] = { towersMenuUpgradeSize[0] / 3.0f, towersMenuUpgradeSize[1] / 5.0f };
	float towerUpg2IconSize[2] = { towersMenuUpgradeSize[0] / 3.0f, towersMenuUpgradeSize[1] / 5.0f };
	float sellTowerIconSize[2] = { towersMenuUpgradeSize[0] / 3.0f, towersMenuUpgradeSize[1] / 5.0f };
	float sellTowerIconPos[2];
	float towerSellValuePos[2];
	bool upg1;
	bool upg2;

	// específico da torre
	Object2D_Animated towersMenuUpgrade;
	Object2D_Animated towersMenuUpgradeD;
	Object2D_Static buildingArea;
	Object2D_Static buildingAreaOver;
	vector<ArcherTower*>* archerTowers;
	vector<Sound*>* towerShotSounds;
	Texture towerTextures;
	Projectile arrow;
	Range towerRange;
	Object2D_Static tower_archer_over;
	ArcherTower archerTowerTemp;
	ArcherTower* archerTower = nullptr;
	Object2D_Animated particle;
	int* totalArcherTowers;

	void CreateBuildingArea(Object2D_Static buildingArea, Object2D_Static buildingAreaOver, float buildingAreaPos[3], Shader* shader_Animation_NL, Shader* shader_Static_NL, Shader* shader_Animation, Shader* shader_Static, Texture& towersMenuTex, Text text, vector<GameObject*>* gameObjects);

	void LoadArcherTower(vector<ArcherTower*>* archerTowers, Texture towersMenuUpgradeTex, Texture towersMenuUpgradeDTex, Texture& towerTextures, Texture& arrowTex, Texture& towerRangeTex, vector<Sound*>* towerShotSounds, int* totalArcherTowers, Object2D_Static tower_archer_over, Object2D_Animated particle);

	void DrawBuildingArea(float mouseCoords[2], float viewXY[2], bool leftMousePressed, Player& player);
};
