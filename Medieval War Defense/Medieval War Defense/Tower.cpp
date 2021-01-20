#include "Tower.h"

	void Tower::CreateTower(Shader& towerShader, Texture& towerTextures, Projectile projectile, Range towerRange, vector<Sound*>* towerShotSounds)
	{
		this->towerShotSounds = towerShotSounds;
		this->projectile = projectile;
		this->towerRange = towerRange;
		tower.CreateStaticObject2D(&towerShader, towerTextures);
	}

	void Tower::Draw(float viewXY[2])
	{
		this->viewXY[0] = viewXY[0];
		this->viewXY[0] = viewXY[0];
		if (tempTower) color[3] = 0.1f;
		else color[3] = 1.0f;
		tower.DrawStatic(size, position, viewXY, rotation, degrees, color);
	}

	void Tower::SetTarget(vector<Enemy*> enemies)
	{
		projectile.position[2] = -1.0f;
		if (!tempTower)
		{
			timer = timeStep.step(1);
			if (!targetSet) // se n tem alvo...
			{
				for (size_t i = 0; i < enemies.size(); i++)
				{
					//position[2] + anim_size_walk[1] / 2.0f;
					this->targetPos[0] = enemies[i]->position[0];
					this->targetPos[1] = enemies[i]->position[1] - enemies[i]->anim_size_walk[1] / 2.0f;
					this->targetPos[2] = enemies[i]->position[2];

					if (enemies[i]->alive && !enemies[i]->outward) // se o inimigo está vivo
					{
						// se está no range...
						if (mu.chkCollisionPoint_Ellipse(targetPos, towerRange.position, towerRange.size))
						{
							float newFartherEnemyDist = mu.CompDistance(targetPos, towerRange.position);
							// se ele é o mais distante...
							if (newFartherEnemyDist > fartherEnemyDist)
							{
								fartherEnemyDist = newFartherEnemyDist;
								enemyTarget = i;
							}
						}
					}
				} // até aqui tenho o vivo, dentro do range, mais distante

				// se um alvo foi encontrado, então fartherEnemyDist > 0.0
				if (fartherEnemyDist > 0.0f)
				{
					targetSet = true;
				}
			}
			else // se já tem alvo, só atualiza a posição dele...
			{

				this->targetPos[0] = enemies[enemyTarget]->position[0];
				this->targetPos[1] = enemies[enemyTarget]->position[1] - enemies[enemyTarget]->anim_size_walk[1] / 2.0f;
				this->targetPos[2] = enemies[enemyTarget]->position[2];

				glm::vec3 pInit = { towerRange.position[0], towerRange.position[1], towerRange.position[2] };
				glm::vec3 am = { 0.0, -projectileHeight, -0.2 };
				glm::vec3 target = { targetPos[0],  targetPos[1], targetPos[2] };
				glm::vec3 vm = target - am / 2.0f - pInit;

				glm::vec3 v = vm * projectileTime;
				glm::vec3 a = am * float(pow(projectileTime, 2));
				glm::vec3 newPos = { pInit + vm * projectileTime + (1.0f / 2.0f) * am * float(pow(projectileTime, 2)) };

				glm::vec3 vPlusA = v + a;
				glm::vec3 versor = glm::normalize(vPlusA);
				float arrowDegrees = atan2(versor[1], versor[0]) * (180.0f / glm::pi<float>());
				float newP[3] = { newPos[0], newPos[1], ((1.0f - newPos[1]) / 2.0f) + newPos[2] };
				projectile.SetPosition(newP);

				this->projectile.degrees = arrowDegrees;

				if (mu.chkCollisionPoint_Ellipse(targetPos, towerRange.position, towerRange.size))
				{
					isInRange = true;

				}
				else {
					isInRange = false;
				}

				// vivo no range com flecha caindo
				if (isInRange && enemies[enemyTarget]->alive && projectileTime < 1.0f)
				{
					projectileTime += timer;
				}
				// vivo no range com flecha caída
				else if (isInRange && enemies[enemyTarget]->alive)
				{
					projectileTime = 0.0f;
					enemies[enemyTarget]->decreaseLife(damage);
					if (fireDmg && (enemies[enemyTarget]->name() == "class Skeleton" || enemies[enemyTarget]->name() == "class Ogre")) enemies[enemyTarget]->isOnFire = true;
					else if (poisonDmg && enemies[enemyTarget]->name() == "class Ogre") enemies[enemyTarget]->isPoisoned = true;
					targetSet = false;
					fartherEnemyDist = 0.0f;
					shot = false;
				}
				// fora do range, vivo, com flecha caindo
				else if (enemies[enemyTarget]->alive && projectileTime < 1.0f)
				{
					projectileTime += timer;
				}
				// fora do range, vivo, com flecha caída
				else if (enemies[enemyTarget]->alive)
				{
					projectileTime = 0.0f;
					targetSet = false;
					fartherEnemyDist = 0.0f;
					enemies[enemyTarget]->decreaseLife(damage / 2.0f);
					shot = false;
				}

				// fora do range, morto, com flecha caindo
				else if (projectileTime < 1.0f)
				{
					projectileTime += timer;
				}

				// fora do range, morto, flecha caída
				else
				{
					projectileTime = 0.0f;
					targetSet = false;
					fartherEnemyDist = 0.0f;
					shot = false;
				}

				if (projectileTime > 0.0f && !shot)
				{
					shot = true;
					mu.ChooseRandom(*towerShotSounds)->PlayAudio();
				}
			}

		}
	}

	void Tower::SetRangeVisibility(bool visible)
	{
		this->towerRange.visible = visible;
	}