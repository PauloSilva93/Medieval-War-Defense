#include "ArcherTower.h"

	void ArcherTower::CreateArcherTower(Shader& towerShader, Texture& towerTextures, Projectile arrow, Range towerRange, vector<Sound*>* towerShotSounds, vector<GameObject*>& gameObjects)
	{
		sellValue = price / (10.0f / 6.0f);
		this->gameObjects = &gameObjects;
		towerRange.color[2] = 1.0f;
		towerRange.color[3] = 0.5f;
		towerRange.size[0] = 0.28;
		towerRange.size[1] = 0.2f;
		towerRange.position[0] = this->position[0];
		towerRange.position[1] = this->position[1] - this->size[1] / 2.0f;
		towerRange.position[2] = 0.0f;

		CreateTower(towerShader, towerTextures, arrow, towerRange, towerShotSounds);
		gameObjects.push_back(&this->towerRange);
		gameObjects.push_back(&this->projectile);
		gameObjects.push_back(this);
	}

	void ArcherTower::SetArrowOnFire(Object2D_Animated particle)
	{
		if (!arrowOnFire)
		{
			fireDmg = true;
			fire.CreateParticleEffects(particle);
			for (size_t i = 0; i < gameObjects->size(); i++)
			{
				if (gameObjects->at(i)->internalObjID == this->poison.internalObjID)
				{
					gameObjects->at(i)->exists = false;
					i = gameObjects->size();
				}
			}
			this->gameObjects->push_back(&fire);
			this->arrowOnFire = true;
			this->arrowPoisoned = false;
			damage++;
		}

		this->fire.UpdateParticleEffects();
		this->fire.position[0] = this->projectile.position[0];
		this->fire.position[1] = this->projectile.position[1];
		this->fire.position[2] = this->projectile.position[2] - 0.000001f;
	}

	void ArcherTower::SetArrowOnPoison(Object2D_Animated particle)
	{
		if (!arrowPoisoned)
		{
			poison.CreateParticleEffects(particle);
			for (size_t i = 0; i < gameObjects->size(); i++)
			{
				if (gameObjects->at(i)->internalObjID == this->fire.internalObjID)
				{
					gameObjects->at(i)->exists = false;
					i = gameObjects->size();
				}
			}
			this->gameObjects->push_back(&poison);
			this->arrowPoisoned = true;
			this->arrowOnFire = false;
			damage++;
		}

		this->poison.UpdateParticleEffects();
		this->poison.position[0] = this->projectile.position[0];
		this->poison.position[1] = this->projectile.position[1];
		this->poison.position[2] = this->projectile.position[2] - 0.000001f;
	}