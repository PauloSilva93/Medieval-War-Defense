#pragma once

#include "Tower.h"
#include "ProjectileEffects.h"
#include "FireEffects.h"
#include "PoisonEffects.h"

class ArcherTower : public Tower
{
public:
	ProjectileEffects projectileEffects;
	FireEffects fire;
	PoisonEffects poison;
	bool arrowOnFire = false;
	bool arrowPoisoned = false;
	int upgradeToFirePrice = 50;
	int upgradeToPoisonPrice = 50;
	int price = 100;

	void CreateArcherTower(Shader& towerShader, Texture& towerTextures, Projectile arrow, Range towerRange, vector<Sound*>* towerShotSounds, vector<GameObject*>& gameObjects);

	void SetArrowOnFire(Object2D_Animated particle);

	void SetArrowOnPoison(Object2D_Animated particle);
};
