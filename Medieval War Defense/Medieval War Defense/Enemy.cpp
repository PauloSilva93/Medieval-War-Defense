#include "Enemy.h"

	void Enemy::CreateEnemy(Shader& shader_animation, Texture& textures_walk, Texture& textures_die, Graph mapPath, vector<Sound*>* enemyDyingSounds, vector<GameObject*>& gameObjects, vector<Enemy*>* enemies, Player& player)
	{
		this->player = &player;
		this->enemies = enemies;
		this->enemyDyingSounds = enemyDyingSounds;
		this->gameObjects = &gameObjects;
		float totalFrames_walk = frames_x_walk * frames_y_walk - blankFrames_walk;
		anim_walk.CreateAnimatedObject2D(&shader_animation, textures_walk, frames_x_walk, frames_y_walk, blankFrames_walk, mu.GenRandFloat(0.0f, 1.0f) * (totalFrames_walk - 1.0f));

		int totalFrames_die = frames_x_die * frames_y_die - blankFrames_die;
		anim_die.CreateAnimatedObject2D(&shader_animation, textures_die, frames_x_die, frames_y_die, blankFrames_die, 0);
		mapPath.loadMap1();
		target = mapPath.genRandInit();
		initPos[0] = { target->position[0] };
		initPos[1] = { target->position[1] };
		initPos[2] = { target->position[2] };
		position[0] = initPos[0];
		position[1] = initPos[1];
		position[2] = initPos[2];
		isOnFire = false;
		isPoisoned = false;

		gameObjects.push_back(this);
		this->enemies->push_back(this);
	}

	void Enemy::Draw(float viewXY[2])
	{
		if (speed >= 0.0f)
		{
			renderTime += speed * timeStep.step(1);
			float newPos[3];
			if (mu.CompDistance(initPos, target->position) != 0)
			{
				newPos[0] = initPos[0] + (target->position[0] - initPos[0]) * renderTime / mu.CompDistance(initPos, target->position);
				newPos[1] = initPos[1] + (target->position[1] - initPos[1]) * renderTime / mu.CompDistance(initPos, target->position);
				newPos[2] = initPos[2] + (target->position[2] - initPos[2]) * renderTime / mu.CompDistance(initPos, target->position);
			}
			else
			{
				newPos[0] = initPos[0];
				newPos[1] = initPos[1];
				newPos[2] = initPos[2];
			}

			position[0] = newPos[0];
			position[1] = newPos[1];
			position[2] = newPos[2];
		}

		if (position[0] <= (-1.0f - anim_size_walk[0] / 2.0f) || position[0] >= (1.0f + anim_size_walk[0] / 2.0f)) this->outward = true;
		if (position[1] <= (-1.0f - anim_size_walk[1] / 2.0f) || position[1] >= (1.0f + anim_size_walk[1] / 2.0f)) this->outward = true;

		if (renderTime / mu.CompDistance(initPos, target->position) >= 1.0f && !outward)
		{
			renderTime = 0.0f;
			initPos[0] = target->position[0];
			initPos[1] = target->position[1];
			initPos[2] = target->position[2];
			target = target->NextTarget();
		}

		position[2] += anim_size_walk[1] / 2.0f;


		for (int i = 0; i < gameObjects->size(); i++)
		{
			if (gameObjects->at(i)->outward)
			{
				gameObjects->at(i)->exists = false;
			}
		}

		if (life > 0.0f)
		{
			anim_walk.DrawAnimated(anim_size_walk, position, viewXY, rotation, degrees, color, invertedXTexture, fps_walk, true);
			if (isOnFire)
			{
				onFireCounter += onFireTime.step(1);
				isOnFire = false;
			}
			else if (onFireCounter <= 5.0f && onFireCounter != 0.0f)
			{
				onFireCounter += onFireTime.step(1);
				if (onFireCounter >= (onFireSeconds + 1))
				{
					onFireSeconds++;
					this->life--;
				}
			}
			else
			{
				onFireCounter = 0.0f;
				onFireSeconds = 0.0f;
			}
		}
		else if (!anim_die.animationFinished)
		{
			if (alive) {
				mu.ChooseRandom(*enemyDyingSounds)->PlayAudio();
				player->SetGold(player->GetGold() + 10.0f);
			}
			anim_die.DrawAnimated(anim_size_die, position, viewXY, rotation, degrees, color, invertedXTexture, fps_die, false);
			alive = false;
			speed = 0.0f;

		}
		else if (color[3] >= 0.0)
		{
			fps_die = 0;
			color[3] -= fadeTime.step(1) / secondsToFade;
			anim_die.DrawAnimated(anim_size_die, position, viewXY, rotation, degrees, color, invertedXTexture, fps_die, false);
		}
		else
		{
			for (int i = 0; i < gameObjects->size(); i++)
			{
				if (gameObjects->at(i)->internalObjID == this->internalObjID)
				{
					gameObjects->at(i)->exists = false;
					i = gameObjects->size();
				}
			}

		}

	}

	void Enemy::decreaseLife(float value)
	{
		this->life -= value;
	}

	void Enemy::decreaseSpeed(float value)
	{
		this->speed -= value;
	}

	void Enemy::increaseSpeed(float value)
	{
		this->speed += value;
	}
