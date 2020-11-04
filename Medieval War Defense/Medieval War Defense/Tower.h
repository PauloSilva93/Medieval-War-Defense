#pragma once

#include "GameObject.h"
#include <vector>
#include "Object2D_Static.h"
#include "Range.h"
#include "MathUtils.h"
#include "Projectile.h"
#include "Sound.h"
#include "Enemy.h"

class Tower : public GameObject
{
public:
	vector<GameObject*>* gameObjects = nullptr;
	Object2D_Static tower = {};
	Projectile projectile = {};
	Range towerRange = {};
	MathUtils mu = {};
	TimeStep timeStep = {};
	vector<Sound*>* towerShotSounds = nullptr;
	int price = 0;
	int sellValue = 0;
	int level = 1;
	float timer = 0.0f;
	float viewXY[2] = { 0.0f, 0.0f };
	float projectileTime = 0.0f;
	float targetPos[3] = {};
	bool isInRange = false;
	int enemyTarget = 0;
	float size[2] = { 0.08f, 0.12f };
	float projectileHeight = 2.0f;
	bool targetSet = false;
	float fartherEnemyDist = 0.0f;
	float damage = 1.0f;
	bool tempTower = false;
	bool shot = false;
	bool fireDmg = false;
	bool poisonDmg = false;

	void CreateTower(Shader& towerShader, Texture& towerTextures, Projectile projectile, Range towerRange, vector<Sound*>* towerShotSounds);

	void Draw(float viewXY[2]);

	void SetTarget(vector<Enemy*> enemies);

	void SetRangeVisibility(bool visible);
};