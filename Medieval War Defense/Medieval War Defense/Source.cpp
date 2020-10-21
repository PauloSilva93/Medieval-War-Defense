#include <iostream>
#include <SDL/SDL.h>
#include <glad/glad.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TimeStep.h"
#include "SDLWindow.h"
#include "Vertices.h"
#include "Texture.h"
#include "Transform.h"
#include "Object2D_Animated.h"
#include "Object2D_Static.h"
#include "Object2D_Interface.h"
#include <vector>
#include <ctime>
#include "Graph.h"
#include "MathUtils.h"
#include "Light.h"
#include "LightSystem.h"
#include "ParticleEmitter.h"
#include "Selection.h"
#include "Music.h"
#include "Sound.h"
#include <array>

//#include "GameObject.h"
#include "Text.h"
#include "Sorter.h"
#include <type_traits>
#include "ProjectileEffects.h"
#include "FireEffects.h"
#include "PoisonEffects.h"

#define PI 3.1415f

class Player
{
private:
	int initialLife;
	int initialGold;
	int currentGold;
	int currentLife;
	bool win;
public:
	void CreatePlayer()
	{
		initialLife = 20;
		initialGold = 300;
		currentLife = initialLife;
		currentGold = initialGold;
	}
	int GetInitialLife() { return initialLife; }
	void SetGold(int gold) { this->currentGold = gold; }
	int GetGold() { return currentGold; }
	void SetLife(int life) { this->currentLife = life; }
	int GetLife() { return currentLife; }
	void Lost() { win = false; }
	void Won() { win = true; };
};

class Enemy : public GameObject
{
public:
	MathUtils mu;
	shared_ptr<Node> target;
	TimeStep timeStep;
	TimeStep fadeTime;
	TimeStep onFireTime;
	TimeStep onPoisonTime;
	Object2D_Animated anim_walk;
	Object2D_Animated anim_die;
	vector<GameObject*> *gameObjects;
	vector<Sound*> *enemyDyingSounds;
	vector<Enemy*> *enemies;
	Player *player = nullptr;
	float onFireCounter = 0.0f;
	float onPoisonCounter = 0.0f;
	float onFireSeconds = 0.0f;
	float onPoisonSeconds = 0.0f;
	float initPos[3];
	float renderTime = 0.0f;
	float secondsToFade = 3.0f;
	float anim_size_walk[2];
	float anim_size_die[2];
	float viewXY[2];
	bool invertedXTexture = false;
	int fps_walk;
	int fps_die;
	bool alive = true;
	float speed;
	float life;
	float frames_x_walk;
	float frames_y_walk;
	float frames_x_die;
	float frames_y_die;
	float blankFrames_walk;
	float blankFrames_die;
	bool isOnFire;
	bool isPoisoned;

public:

	virtual string name() const = 0;

	void CreateEnemy(Shader &shader_animation, Texture &textures_walk, Texture &textures_die, Graph mapPath, vector<Sound*> *enemyDyingSounds, vector<GameObject*> &gameObjects, vector<Enemy*> *enemies, Player &player)
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

	void Draw(float viewXY[2])
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

		if (position[0] <= (-1.0f - anim_size_walk[0]) || position[0] >= (1.0f + anim_size_walk[0])) this->outward = true;
		if (position[1] <= (-1.0f - anim_size_walk[1]) || position[1] >= (1.0f + anim_size_walk[1])) this->outward = true;

		if (renderTime / mu.CompDistance(initPos, target->position) >= 1.0 && !outward)
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
			cout << gameObjects->size() << endl;
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

	void decreaseLife(float value)
	{
		this->life -= value;
	}

	void decreaseSpeed(float value)
	{
		this->speed -= value;
	}

	void increaseSpeed(float value)
	{
		this->speed += value;
	}

};

class Skeleton : public Enemy
{
public:

	string name() const
	{
		return typeid(Skeleton).name();
	}

	void CreateSkeleton(Shader &shader_animation, Texture &textures_walk, Texture &textures_die, Graph mapPath, vector<Sound*> *enemyDyingSounds, vector<GameObject*> &gameObjects, vector<Enemy*> &enemies, Player &player)
	{
		anim_size_walk[0] = { 0.05f };
		anim_size_walk[1] = { 0.05f };
		anim_size_die[0] = { 0.06f };
		anim_size_die[1] = { 0.06f };
		fps_walk = 60;
		fps_die = 90;
		speed = 0.05f;
		life = 3.0f;
		frames_x_walk = 8;
		frames_y_walk = 13;
		blankFrames_walk = 4;
		frames_x_die = 8;
		frames_y_die = 13;
		blankFrames_die = 4;

		CreateEnemy(shader_animation, textures_walk, textures_die, mapPath, enemyDyingSounds, gameObjects, &enemies, player);
	}
};

class Ogre : public Enemy
{
public:

	string name() const
	{
		return typeid(Ogre).name();
	}

	void CreateOgre(Shader &shader_animation, Texture &textures_walk, Texture &textures_die, Graph mapPath, vector<Sound*> *enemyDyingSounds, vector<GameObject*> &gameObjects, vector<Enemy*> &enemies, Player &player)
	{
		anim_size_walk[0] = { 0.12f };
		anim_size_walk[1] = { 0.12f };
		anim_size_die[0] = { 0.12f };
		anim_size_die[1] = { 0.12f };
		fps_walk = 15;
		fps_die = 15;
		speed = 0.04f;
		life = 20.0f;
		frames_x_walk = 4;
		frames_y_walk = 5;
		blankFrames_walk = 2;
		frames_x_die = 4;
		frames_y_die = 4;
		blankFrames_die = 1;

		CreateEnemy(shader_animation, textures_walk, textures_die, mapPath, enemyDyingSounds, gameObjects, &enemies, player);
	}
};

class Projectile : public GameObject
{
public:
	Object2D_Animated projectile;
	int frames_X;
	int frames_Y;
	int blankFrames;
	float initialFrame;
	float size[2];
	bool isInvertedXTexture;
	int fps;
	bool loop;
	
	void CreateProjectile(Shader &shader_animation, Texture &projectileTex)
	{	
		frames_X = 1;
		frames_Y = 1;
		blankFrames = 0;
		initialFrame = 0.0f;
		size[0] = 0.02f;
		size[1] = 0.02f;
		isInvertedXTexture = false;
		fps = 1;
		loop = false;
		projectile.CreateAnimatedObject2D(&shader_animation, projectileTex, frames_X, frames_Y, blankFrames, initialFrame);
	}

	void Draw(float viewXY[2])
	{
		projectile.DrawAnimated(size, position, viewXY, rotation, degrees, color, isInvertedXTexture, fps, loop);
	}

};

class Range : public GameObject
{
public:
	Object2D_Static rangeObject;
	float size[2] = { 0.1f, 0.1f };
	bool visible = true;

	void CreateRangeObject(Shader &shader, Texture &textures)
	{
		rangeObject.CreateStaticObject2D(&shader, textures);
	}

	void Draw(float view[2])
	{
		if (visible) rangeObject.DrawStatic(size, position, view, rotation, degrees, color);
	}
};

class Tower : public GameObject
{
public:
	vector<GameObject*> *gameObjects;
	Object2D_Static tower;
	Projectile projectile;
	Range towerRange;
	MathUtils mu;
	TimeStep timeStep;
	vector<Sound*> *towerShotSounds;
	int price = 0;
	int sellValue = 0;
	int level = 1;
	float timer = 0.0f;
	float viewXY[2];
	float projectileTime = 0.0f;
	float targetPos[3];
	bool isInRange = isInRange;
	int enemyTarget;
	float size[2] = { 0.08f, 0.12f };
	float projectileHeight = 2.0f;
	bool targetSet = false;
	float fartherEnemyDist = 0.0f;
	float damage = 1.0f;
	bool tempTower = false;
	bool shot = false;
	bool fireDmg = false;
	bool poisonDmg = false;

	void CreateTower(Shader &towerShader, Texture &towerTextures, Projectile projectile, Range towerRange, vector<Sound*> *towerShotSounds)
	{
		this->towerShotSounds = towerShotSounds;
		this->projectile = projectile;
		this->towerRange = towerRange;
		tower.CreateStaticObject2D(&towerShader, towerTextures);
	}

	void Draw(float viewXY[2])
	{
		this->viewXY[0] = viewXY[0];
		this->viewXY[0] = viewXY[0];
		if (tempTower) color[3] = 0.1f;
		else color[3] = 1.0f;
		tower.DrawStatic(size, position, viewXY, rotation, degrees, color);
	}

	void SetTarget(vector<Enemy*> enemies)
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
				glm::vec3 a = am * pow(projectileTime, 2);
				glm::vec3 newPos = { pInit + vm * projectileTime + (1.0f / 2.0f) * am * pow(projectileTime, 2) };

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

	void SetRangeVisibility(bool visible)
	{
		this->towerRange.visible = visible;
	}
};


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

	void CreateArcherTower(Shader &towerShader, Texture &towerTextures, Projectile arrow, Range towerRange, vector<Sound*> *towerShotSounds, vector<GameObject*> &gameObjects)
	{
		sellValue = price / (10.0f/6.0f);
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

	void SetArrowOnFire(Object2D_Animated particle)
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
		this->fire.position[2] = this->projectile.position[2] -0.000001f;
	}

	void SetArrowOnPoison(Object2D_Animated particle)
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

};

class MapObject : public GameObject
{
public:
	Object2D_Static mapObject;
	float size[2] = { 0.1f, 0.1f };

	void CreateMapObject(Shader &shader, Texture &textures, vector<GameObject*> &gameObjects)
	{
		mapObject.CreateStaticObject2D(&shader, textures);
		gameObjects.push_back(this);
	}

	void Draw(float view[2])
	{
		mapObject.DrawStatic(size, position, view, rotation, degrees, color);
	}
};

class Map : public GameObject
{
public:
	Object2D_Static map;
	float size[2] = { 1.0f, 1.0f };

	void CreateMap(Shader &shader, Texture &textures)
	{
		map.CreateStaticObject2D(&shader, textures);;
	}
	void DrawMapObject(float view[2])
	{
		map.DrawStatic(size, position, view, rotation, degrees, color);
	}
};

class BuildingArea
{
public:
	// específico da BuildingArea
	Object2D_Animated towersMenu;
	Selection s;
	Shader *shader_Animation;
	Shader *shader_Static;
	Shader *shader_Animation_NL;
	Shader *shader_Static_NL;
	Text text;
	vector<GameObject*> *gameObjects;
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
	vector<ArcherTower*> *archerTowers;
	vector<Sound*> *towerShotSounds;
	Texture towerTextures;
	Projectile arrow;
	Range towerRange;
	Object2D_Static tower_archer_over;
	ArcherTower archerTowerTemp;
	ArcherTower *archerTower = nullptr;
	Object2D_Animated particle;
	int *totalArcherTowers;

	void CreateBuildingArea(Object2D_Static buildingArea, Object2D_Static buildingAreaOver, float buildingAreaPos[3], Shader *shader_Animation_NL, Shader *shader_Static_NL, Shader *shader_Animation, Shader *shader_Static, Texture &towersMenuTex, Text text, vector<GameObject*> *gameObjects)
	{
		towerLevel = 1;
		towerSold = false;
		buildingIsUnderMouse = false;
		towerIsUnderMouse = false;
		towerIsShown = false;
		towerOverIsShown = false;
		buildingAreaVisible = true;
		towerPlaced = false;
		towerMenuOpened = false;
		towerMenuUpgradeOpened = false;
		upg1 = false;
		upg2 = false;
		s.isOver = false;
		s.selected = false;
		this->buildingArea = buildingArea;
		this->buildingAreaOver = buildingAreaOver;
		this->towersMenuTex = towersMenuTex;
		this->shader_Animation_NL = shader_Animation_NL;
		this->shader_Animation = shader_Animation;
		this->shader_Static = shader_Static;
		this->shader_Static_NL = shader_Static_NL;
		this->buildingAreaPos[0] = buildingAreaPos[0];
		this->buildingAreaPos[1] = buildingAreaPos[1];
		this->buildingAreaPos[2] = buildingAreaPos[2];
		this->text = text;
		this->gameObjects = gameObjects;
		towersMenuPos[0] = buildingAreaPos[0];
		towersMenuPos[1] = buildingAreaPos[1];
		towersMenuColor[0] = 0.0f;
		towersMenuColor[1] = 0.0f;
		towersMenuColor[2] = 0.0f;
		towersMenuColor[3] = 1.0f;
		towersMenuUpgradePos[0] = buildingAreaPos[0];
		towersMenuUpgradePos[1] = buildingAreaPos[1] + 0.05f;
		towerIconPos[0] = towersMenuPos[0] - ((towersMenuSize[0] / 5.0f) * 3.5f);
		towerIconPos[1] = towersMenuPos[1] - ((towersMenuSize[1] / 5.0f) * 3.5f);
		towerPosPrice[0] = towerIconPos[0] - 0.02f;
		towerPosPrice[1] = towerIconPos[1] - 0.08f;
		towerUpg1IconPos[0] = { towersMenuUpgradePos[0] - ((towersMenuUpgradeSize[0] / 3.0f) * 2.0f) };
		towerUpg1IconPos[1] = { towersMenuUpgradePos[1] + ((towersMenuUpgradeSize[1] / 5.0f) * 4.0f) };
		towerUpg2IconPos[0] = { towersMenuUpgradePos[0] + ((towersMenuUpgradeSize[0] / 3.0f) * 2.0f) };
		towerUpg2IconPos[1] = { towersMenuUpgradePos[1] + ((towersMenuUpgradeSize[1] / 5.0f) * 4.0f) };
		towerUpg1PosPrice[0] = { towerUpg1IconPos[0] - 0.02f };
		towerUpg1PosPrice[1] = { towerUpg1IconPos[1] - 0.08f };
		towerUpg2PosPrice[0] = { towerUpg2IconPos[0] - 0.02f };
		towerUpg2PosPrice[1] = { towerUpg2IconPos[1] - 0.08f };
		sellTowerIconPos[0] = towersMenuUpgradePos[0];
		sellTowerIconPos[1] = { towersMenuUpgradePos[1] - ((towersMenuUpgradeSize[1] / 5.0f) * 4.0f) };
		towerSellValuePos[0] = sellTowerIconPos[0] - 0.02f;
		towerSellValuePos[1] = sellTowerIconPos[1] - 0.08f;
	};

	void LoadArcherTower(vector<ArcherTower*> *archerTowers, Texture towersMenuUpgradeTex, Texture towersMenuUpgradeDTex, Texture &towerTextures, Texture &arrowTex, Texture &towerRangeTex, vector<Sound*> *towerShotSounds, int *totalArcherTowers, Object2D_Static tower_archer_over, Object2D_Animated particle)
	{
		this->towerShotSounds = towerShotSounds;
		towersMenu.CreateAnimatedObject2D(shader_Animation_NL, towersMenuTex, 5, 5, 0, 0);
		towersMenuUpgrade.CreateAnimatedObject2D(shader_Animation_NL, towersMenuUpgradeTex, 5, 5, 0, 0);
		towersMenuUpgradeD.CreateAnimatedObject2D(shader_Animation_NL, towersMenuUpgradeDTex, 5, 5, 0, 25);
		this->tower_archer_over = tower_archer_over;
		this->archerTowers = archerTowers;
		this->towerTextures = towerTextures;
		this->arrow = arrow;
		arrow.CreateProjectile(*shader_Animation, arrowTex);
		arrow.size[0] = 0.03f;
		arrow.size[1] = 0.008f;
		this->towerRange = towerRange;
		towerRange.CreateRangeObject(*shader_Static_NL, towerRangeTex);
		this->totalArcherTowers = totalArcherTowers;
		this->particle = particle;
	}

	void DrawBuildingArea(float mouseCoords[2], float viewXY[2], bool leftMousePressed, Player &player)
	{
		if (!towerPlaced) // Se não há torre colocada...
		{
			if (s.isUnderMouse(mouseCoords, buildingAreaPos, buildingAreaSize)) // Se o mouse está sobre a área de construção
			{
				// glow sobre a área
				buildingAreaOver.DrawStatic(buildingAreaOverSize, buildingAreaPos, viewXY, buildingAreaRotation, buildingAreaDegrees, buildingAreaColor);
			}
			if (buildingAreaVisible) // Se a área de construção está visível
			{
				// desenha a área
				buildingArea.DrawStatic(buildingAreaSize, buildingAreaPos, viewXY, buildingAreaRotation, buildingAreaDegrees, buildingAreaColor);
			}
			// se clicou na área de construção, menu se abre
			if (s.isSelected(mouseCoords, buildingAreaPos, buildingAreaSize, leftMousePressed))
			{
				towerMenuOpened = true;
			}
			else if (!s.isUnderMouse(mouseCoords, towerIconPos, towerIconSize)) towerMenuOpened = false;

			if (towerMenuOpened || towerSold) // e se o menu está aberto
			{
	
				towersMenu.DrawAnimated(towersMenuSize, towersMenuPos, viewXY, buildingAreaRotation, 0, towersMenuColor, false, 40, false);

				if (towersMenu.animationFinished)
					text.RenderText(to_string(archerTowerTemp.price), towerPosPrice[0], towerPosPrice[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
				// se o mouse estiver sobre o ícone da torre arqueira...
				if (s.isUnderMouse(mouseCoords, towerIconPos, towerIconSize) && player.GetGold() >= archerTowerTemp.price)
				{
					// e se o esquerdo for pressionado...coloco a torre e área da construção n é visível
					if (leftMousePressed)
					{
						towerPlaced = true;
						towerIsShown = true;
					}
					// se ainda não registrou que o mouse está em cima e a torre está sendo mostrada...registre e desenhe a torre.
					if (!buildingIsUnderMouse && !towerIsShown)
					{
						archerTower = new ArcherTower;
						archerTowers->push_back(archerTower);
						towerPos[0] = buildingAreaPos[0];
						towerPos[1] = buildingAreaPos[1] + (archerTowers->at(*totalArcherTowers)->size[1] / 2.0f);
						towerPos[2] = ((1.0f - towerPos[1]) / 2.0f) + (archerTowers->at(*totalArcherTowers)->size[1] / 2.0f);
						archerTowers->at(*totalArcherTowers)->SetPosition(towerPos);

						archerTowers->at(*totalArcherTowers)->tempTower = true;
						archerTowers->at(*totalArcherTowers)->CreateArcherTower(*shader_Static, towerTextures, arrow, towerRange, towerShotSounds, *gameObjects);
						(*totalArcherTowers)++;
						buildingIsUnderMouse = true;
						towerIsShown = true;

						archerTowerTemp = *archerTower;
						archerTowerTemp.size[0] = archerTower->size[0] + 0.02;
						archerTowerTemp.size[1] = archerTower->size[1] + 0.02;
					}
					// se a torre for colocada, a torre deixa de ser temporária...
					if (towerPlaced) {
						archerTowers->at(*totalArcherTowers - 1)->tempTower = false;
						player.SetGold(player.GetGold() -archerTowerTemp.price);
					}
				}
				// mas se o mouse não está mais sobre o ícone da torre arqueira, mas a torre ainda está sendo mostrada...delete-a 
				else if (towerIsShown)
				{
					for (int i = 0; i < gameObjects->size(); i++)
					{
						for (int j = 0; j < archerTowers->size(); j++)
						{
							if (archerTowers->at(j)->internalObjID == gameObjects->at(i)->internalObjID)
							{
								if (archerTowers->at(j)->internalObjID == archerTower->internalObjID)
								{
									for (int k = 0; k < gameObjects->size(); k++)
									{
										if (archerTowers->at(j)->internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->projectile.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->fire.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->poison.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->towerRange.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
									}

									delete archerTowers->at(j);

									archerTowers->erase(archerTowers->begin() + j);
									(*totalArcherTowers)--;
								}
							}
						}
					}
					// e registre que não está mais sob o mouse e não está sendo mostrada
					buildingIsUnderMouse = false;
					towerIsShown = false;
				}
				else // se não estiver mais com o mouse em cima e a torre já n é mostrada, mostre a área de construção visível...
				{
					buildingAreaVisible = true;
				}
			}
			if (towerSold) {
				buildingAreaVisible = true;
				towerSold = false;
				upg1 = false;
				upg2 = false;
			}
		}
		else if (towerPlaced)
		{
			if (s.isUnderMouse(mouseCoords, archerTowerTemp.position, archerTowerTemp.size)) // Se o mouse está sobre a área da torre
			{
				archerTower->SetRangeVisibility(true);
				// glow sobre a torre
				tower_archer_over.DrawStatic(archerTowerTemp.size, archerTowerTemp.position, viewXY, buildingAreaRotation, buildingAreaDegrees, buildingAreaColor);
			}
			else archerTower->SetRangeVisibility(false);
			if (s.isSelected(mouseCoords, archerTowerTemp.position, archerTowerTemp.size, leftMousePressed))
			{
				archerTower->SetRangeVisibility(true);
				towerMenuUpgradeOpened = true;
			}
			else if
				(!s.isUnderMouse(mouseCoords, towerUpg1IconPos, towerUpg1IconSize)
					&& !s.isUnderMouse(mouseCoords, towerUpg2IconPos, towerUpg2IconSize)
					&& !s.isUnderMouse(mouseCoords, sellTowerIconPos, sellTowerIconSize)
					) towerMenuUpgradeOpened = false;

			if (towerMenuUpgradeOpened)
			{
				if (towerLevel == 1 && player.GetGold() >= archerTowerTemp.upgradeToFirePrice)
				{
					towersMenuUpgrade.DrawAnimated(towersMenuUpgradeSize, towersMenuUpgradePos, viewXY, buildingAreaRotation, 0, buildingAreaColor, false, 40, false);
				}
				else {
					towersMenuUpgradeD.DrawAnimated(towersMenuUpgradeSize, towersMenuUpgradePos, viewXY, buildingAreaRotation, 0, buildingAreaColor, false, 0, false);
				}
				if (towersMenuUpgrade.animationFinished || towersMenuUpgradeD.animationFinished)
				{
					text.RenderText(to_string(archerTowerTemp.upgradeToFirePrice), towerUpg1PosPrice[0], towerUpg1PosPrice[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
					text.RenderText(to_string(archerTowerTemp.upgradeToPoisonPrice), towerUpg2PosPrice[0], towerUpg2PosPrice[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
					text.RenderText(to_string(archerTowerTemp.sellValue), towerSellValuePos[0], towerSellValuePos[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
				}

				if (s.isUnderMouse(mouseCoords, towerUpg1IconPos, towerUpg1IconSize) && player.GetGold() >= archerTowerTemp.upgradeToFirePrice)
				{
					archerTower->towerRange.size[0] = 0.28f * 1.33f;
					archerTower->towerRange.size[1] = 0.2f * 1.33f;
					if (leftMousePressed)
					{
						if (!upg1 && !upg2)
						{
							archerTower->towerRange.size[0] = 0.28f * 1.33f;
							archerTower->towerRange.size[1] = 0.2f * 1.33f;
							towerLevel = 2;
							player.SetGold(player.GetGold() - archerTowerTemp.upgradeToFirePrice);
							upg1 = true;
						}
					}
				}

				else if (s.isUnderMouse(mouseCoords, towerUpg2IconPos, towerUpg2IconSize) && player.GetGold() >= archerTowerTemp.upgradeToFirePrice)
				{
					archerTower->towerRange.size[0] = 0.28f * 1.33f;
					archerTower->towerRange.size[1] = 0.2f * 1.33f;
					if (leftMousePressed)
					{
						if (!upg1 && !upg2)
						{
							archerTower->towerRange.size[0] = 0.28f * 1.33f;
							archerTower->towerRange.size[1] = 0.2f * 1.33f;
							towerLevel = 2;
							player.SetGold(player.GetGold() - archerTowerTemp.upgradeToPoisonPrice);
							upg2 = true;
						}
					}
				}
				else if (s.isUnderMouse(mouseCoords, sellTowerIconPos, sellTowerIconSize))
				{
					if (leftMousePressed)
					{
						towerSold = true;
						towerPlaced = false;
						player.SetGold(player.GetGold() + archerTowerTemp.sellValue);
						towerLevel = 1;
					}
				}
				else
				{
					if (towerLevel == 1)
					{
						archerTower->towerRange.size[0] = 0.28f;
						archerTower->towerRange.size[1] = 0.2f;
					}
				}
			}
			if (upg1) archerTower->SetArrowOnFire(particle);
			else if (upg2) archerTower->SetArrowOnPoison(particle);
		}
	}
};

int GameObject::objID = 0;

using namespace std;

int main(int argc, char** argv) {

	srand((unsigned int)time(NULL));

	MathUtils mu;	
	Selection selector;
	Sorter sorter;
	int mouseX, mouseY;
	float viewX = 0.0f, viewY = 0.0f;
	float viewXY[2] = { viewX , viewY };

	// Cria janela
	int windowWidth = GetSystemMetrics(SM_CXSCREEN), windowHeight = GetSystemMetrics(SM_CYSCREEN);
	windowHeight -= 60;
	windowWidth = windowHeight;
	SDLWindow window(windowWidth, windowHeight, "Medieval War Defense");

	// Carrega arquivos de shader
	Shader shader_Interface("shaders/shader_Interface.vert", "shaders/shader_Interface.frag");
	Shader shader_Static("shaders/shader_Static.vert", "shaders/shader_Static.frag");
	Shader shader_Static_NL("shaders/shader_Static_NL.vert", "shaders/shader_Static_NL.frag");
	Shader shader_Animation("shaders/shader_Animation.vert", "shaders/shader_Animation.frag");
	Shader shader_Animation_NL("shaders/shader_Animation_NL.vert", "shaders/shader_Animation_NL.frag");
	Shader shader_Text("shaders/shader_Text.vert", "shaders/shader_Text.frag");
	Shader lighShaderList[] = { shader_Static , shader_Animation };

	Text text;
	text.CreateText(&shader_Text, &window, "fonts/RINGM___.TTF");

	// Carrega vertices
	Vertices vertices;
	vertices.LoadVertices();

	const char* leafTexPaths[1] = { "imgs/leaf.png" };
	const char* earthTexPaths[1] = { "imgs/rock.png" };
	const char* waterTexPaths[1] = { "imgs/particle.png" };
	const char* fireTexPaths[1] = { "imgs/particle.png" };
	const char* cloudsTexPaths[1] = { "imgs/clouds.png" };
	const char* you_lostTexPaths[1] = { "imgs/you_Lost.png" };
	const char* playerHeartTexPaths[1] = { "imgs/playerHeart.png" };
	const char* playerGoldTexPaths[1] = { "imgs/playerGold.png" };
	const char* menu_backGroundTexPaths[5] = { "imgs/menu_background.png", "imgs/menu_background.png", "imgs/menu_background.png", "imgs/menu_background.png", "imgs/noEmit.png" };
	const char* play_glow_TexPaths[5] = { "imgs/play_glow.png", "imgs/play_glow.png", "imgs/play_glow.png", "imgs/play_glow.png", "imgs/noEmit.png" };
	const char* exit_glow_TexPaths[5] = { "imgs/exit_glow.png", "imgs/exit_glow.png", "imgs/exit_glow.png", "imgs/exit_glow.png", "imgs/noEmit.png" };
	const char* credits_glow_TexPaths[5] = { "imgs/credits_glow.png", "imgs/credits_glow.png", "imgs/credits_glow.png", "imgs/credits_glow.png", "imgs/noEmit.png" };
	const char* loading_TexPaths[5] = { "imgs/king_anim.png", "imgs/king_anim.png" , "imgs/king_anim.png" , "imgs/king_anim.png" , "imgs/noEmit.png" };
	const char* skeleton_w_TexPaths[5] = { "imgs/walk_skeleton.png", "imgs/walk_skeleton_n.png", "imgs/walk_skeleton.png" , "imgs/walk_skeleton.png" , "imgs/walk_skeleton_emit.png" };
	const char* skeleton_d_TexPaths[5] = { "imgs/dying_skeleton.png", "imgs/dying_skeleton_n.png", "imgs/dying_skeleton.png" , "imgs/dying_skeleton.png" , "imgs/noEmit.png" };
	const char* ogre_w_TexPaths[5] = { "imgs/walk_ogre.png", "imgs/walk_ogre_n.png", "imgs/walk_ogre.png", "imgs/walk_ogre.png", "imgs/noEmit.png" };
	const char* ogre_d_TexPaths[5] = { "imgs/dying_ogre.png", "imgs/dying_ogre.png", "imgs/dying_ogre.png", "imgs/dying_ogre.png", "imgs/noEmit.png" };
	const char* particleTexPaths[1] = { "imgs/particle.png" };
	const char* arrowTexPaths[5] = { "imgs/Arrow.png", "imgs/Arrow_n.png", "imgs/Arrow.png", "imgs/Arrow.png", "imgs/noEmit.png" };
	const char* towerRangeTexPaths[1] = { "imgs/selection_image.png" };
	const char* towerArcherTexPaths[5] = { "imgs/tower_archer.png", "imgs/tower_archer_n.png", "imgs/tower_archer.png", "imgs/tower_archer.png", "imgs/noEmit.png" };
	const char* tree_1TexPaths[5] = { "imgs/tree_1.png", "imgs/tree_1_n.png", "imgs/tree_1.png", "imgs/tree_1.png", "imgs/noEmit.png" };
	const char* forestTexPaths[5] = { "imgs/map_forest.png", "imgs/map_forest_n.png", "imgs/map_forest.png", "imgs/map_forest.png", "imgs/noEmit.png" };
	const char* stone_1TexPaths[5] = { "imgs/stone_1.png", "imgs/stone_1_n.png", "imgs/stone_1.png", "imgs/stone_1.png", "imgs/noEmit.png" };
	const char* stone_2TexPaths[5] = { "imgs/stone_2.png", "imgs/stone_2_n.png", "imgs/stone_2.png", "imgs/stone_2.png", "imgs/noEmit.png" };
	const char* buildingAreaTexPaths[5] = { "imgs/building_area.png", "imgs/building_area_n.png", "imgs/building_area.png", "imgs/building_area.png", "imgs/noEmit.png" };
	const char* buildingAreaOverTexPaths[5] = { "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/noEmit.png" };
	const char* towersMenuTexPaths[5] = { "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/noEmit.png" };
	const char* towersUpgradeTexPaths[5] = { "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/noEmit.png" };
	const char* tower_archer_overTexPaths[5] = { "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/noEmit.png" };
	const char* towerUpgradeDTexPaths[5] = { "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/noEmit.png" };
	const char* bush_1TexPaths[5] = { "imgs/bush_object_1.png", "imgs/bush_object_1_n.png", "imgs/bush_object_1.png" , "imgs/bush_object_1.png" , "imgs/noEmit.png" };
	const char* stump_1TexPaths[5] = { "imgs/stump_1.png", "imgs/stump_1_n.png", "imgs/stump_1.png" , "imgs/stump_1.png" , "imgs/noEmit.png" };
	const char* tree_2TexPaths[5] = { "imgs/tree_2.png", "imgs/tree_2_n.png", "imgs/tree_2.png" , "imgs/tree_2.png" , "imgs/noEmit.png" };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Texture loadingTex;
	loadingTex.LoadTextures(loading_TexPaths);
	Object2D_Animated loading;
	loading.CreateAnimatedObject2D(&shader_Animation_NL, loadingTex, 3, 4, 0, 0);
	float loadingPos[3] = { -0.4, 0, 0.1 };
	float loadingSize[2] = { 0.2, 0.2 };
	float loadingRotation[3] = { 0, 0, 1 };
	float loadingColor[4] = {0, 0, 0, 1 };
	while (!loading.animationFinished)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		loading.DrawAnimated(loadingSize, loadingPos, viewXY, loadingRotation, 0, loadingColor, false, 20, false);
		if (loading.animationFinished) text.RenderText("Loading...", -0.2, 0, 1, glm::vec3(1.0, 0.8f, 0.4f));
		window.SDL_SwapWindow();
	}

	Texture leafTex;
	Texture earthTex;
	Texture waterTex;
	Texture fireTex;
	Texture cloudsTex;
	Texture you_lostTex;
	Texture playerHeartTex;
	Texture playerGoldTex;
	Texture menu_backGroundTex;
	Texture play_glow_Tex;
	Texture exit_glow_Tex;
	Texture credits_glow_Tex;
	Texture skeleton_w_Tex;
	Texture skeleton_d_Tex;
	Texture ogre_w_Tex;
	Texture ogre_d_Tex;
	Texture backGroundTex;
	Texture particleTex;
	Texture arrowTex;
	Texture rangeTex;
	Texture towerArcherTex;
	Texture tree_1Tex;
	Texture forestTex;
	Texture stone_1Tex;
	Texture stone_2Tex;
	Texture buildingAreaTex;
	Texture buildingAreaOverTex;
	Texture towersMenuTex;
	Texture towersUpgradeTex;
	Texture tower_archer_overTex;
	Texture towersUpgradeDTex;
	Texture bush_1Tex;
	Texture stump_1Tex;
	Texture tree_2Tex;

	leafTex.LoadTextures(leafTexPaths);
	earthTex.LoadTextures(earthTexPaths);
	waterTex.LoadTextures(waterTexPaths);
	fireTex.LoadTextures(fireTexPaths);
	cloudsTex.LoadTextures(cloudsTexPaths);
	you_lostTex.LoadTextures(you_lostTexPaths);
	playerHeartTex.LoadTextures(playerHeartTexPaths);
	playerGoldTex.LoadTextures(playerGoldTexPaths);
	menu_backGroundTex.LoadTextures(menu_backGroundTexPaths);
	play_glow_Tex.LoadTextures(play_glow_TexPaths);
	exit_glow_Tex.LoadTextures(exit_glow_TexPaths);
	credits_glow_Tex.LoadTextures(credits_glow_TexPaths);
	skeleton_w_Tex.LoadTextures(skeleton_w_TexPaths);
	skeleton_d_Tex.LoadTextures(skeleton_d_TexPaths);
	ogre_w_Tex.LoadTextures(ogre_w_TexPaths);
	ogre_d_Tex.LoadTextures(ogre_d_TexPaths);
	particleTex.LoadTextures(particleTexPaths);
	arrowTex.LoadTextures(arrowTexPaths);
	rangeTex.LoadTextures(towerRangeTexPaths);
	towerArcherTex.LoadTextures(towerArcherTexPaths);
	tree_1Tex.LoadTextures(tree_1TexPaths);
	forestTex.LoadTextures(forestTexPaths);
	stone_1Tex.LoadTextures(stone_1TexPaths);
	stone_2Tex.LoadTextures(stone_2TexPaths);
	buildingAreaTex.LoadTextures(buildingAreaTexPaths);
	buildingAreaOverTex.LoadTextures(buildingAreaOverTexPaths);
	towersMenuTex.LoadTextures(towersMenuTexPaths);
	towersUpgradeTex.LoadTextures(towersUpgradeTexPaths);
	tower_archer_overTex.LoadTextures(tower_archer_overTexPaths);
	towersUpgradeDTex.LoadTextures(towerUpgradeDTexPaths);
	bush_1Tex.LoadTextures(bush_1TexPaths);
	stump_1Tex.LoadTextures(stump_1TexPaths);
	tree_2Tex.LoadTextures(tree_2TexPaths);

	// CARREGA AUDIO
	Music mainTheme;
	Music battleMusic;
	mainTheme.LoadAudio("audio/main_theme.mp3");
	mainTheme.SetVolume(25);
	battleMusic.LoadAudio("audio/battleMusic.mp3");
	bool firstBattleMusicPlay;

	Sound skeletonVoice;
	skeletonVoice.LoadAudio("audio/zombie_voice_brains_09.wav");
	Sound skeletonDying_1;
	skeletonDying_1.LoadAudio("audio/skeletonDying_1.wav");
	Sound skeletonDying_2;
	skeletonDying_2.LoadAudio("audio/skeletonDying_2.wav");
	Sound skeletonDying_3;
	skeletonDying_3.LoadAudio("audio/skeletonDying_3.wav");
	Sound skeletonDying_4;
	skeletonDying_4.LoadAudio("audio/skeletonDying_4.wav");
	Sound skeletonDying_5;
	skeletonDying_5.LoadAudio("audio/skeletonDying_5.wav");
	vector<Sound*> skeletonDying_Sounds = { &skeletonDying_1 , &skeletonDying_2 , &skeletonDying_3, &skeletonDying_4, &skeletonDying_5 };

	Sound ogreDying_1;
	ogreDying_1.LoadAudio("audio/ogreDying_1.wav");
	Sound ogreDying_2;
	ogreDying_2.LoadAudio("audio/ogreDying_2.wav");
	Sound ogreDying_3;
	ogreDying_3.LoadAudio("audio/ogreDying_3.wav");
	Sound ogreDying_4;
	ogreDying_4.LoadAudio("audio/ogreDying_4.wav");
	Sound ogreDying_5;
	ogreDying_5.LoadAudio("audio/ogreDying_5.wav");
	vector<Sound*> ogreDying_Sounds = { &ogreDying_1 , &ogreDying_2 , &ogreDying_3, &ogreDying_4, &ogreDying_5 };

	Sound arrowShot_1;
	Sound arrowShot_2;
	Sound arrowShot_3;
	Sound arrowShot_4;
	Sound arrowShot_5;
	arrowShot_1.LoadAudio("audio/arrowShot_1.wav");
	arrowShot_2.LoadAudio("audio/arrowShot_2.wav");
	arrowShot_3.LoadAudio("audio/arrowShot_3.wav");
	arrowShot_4.LoadAudio("audio/arrowShot_4.wav");
	arrowShot_5.LoadAudio("audio/arrowShot_5.wav");
	vector<Sound*> arrowShot_Sounds = { &arrowShot_1 , &arrowShot_2, &arrowShot_3, &arrowShot_4, &arrowShot_5 };

	Object2D_Static buildingArea;
	Object2D_Static buildingAreaOver;
	Object2D_Static tower_archer_over;
	Object2D_Animated particle;
	Object2D_Animated arrow;

	Object2D_Animated clouds;
	vector<Particle> cloudParticles;
	ParticleEmitter cloudsEmitter;
	clouds.CreateAnimatedObject2D(&shader_Animation_NL, cloudsTex, 1, 1, 0, 0);
	float cloudsSize[2] = { 1.0, 0.1375 };
	float cloudsPos[3] = { -0.8, 0.7, 1 };
	float cloudsRot[3] = { 0, 0, 1 };
	float cloudsColor[4] = { 0, 0, 0, 0 };
	float cloudsPntList[2][2] = { {0.0, 0.6},{0.0, -0.9} };
	cloudsEmitter.SetPosition(cloudsPos);
	cloudsEmitter.SetEmissionTime(-1);
	cloudsEmitter.SetEmissionDelay(1);
	cloudsEmitter.RandPosBtw2Pts(cloudsPntList);
	cloudsEmitter.SetParticleList(cloudParticles);
	cloudsEmitter.SetParticleProperties(clouds, cloudsSize, 0, 200, 10, cloudsColor, 0);
	float cloudsChgColor[4] = { 1, 1, 1, 1.0 };
	float cloudsChgColorVel[4] = { 0.5, 0.5, 0.5, .05 };

	// LEAF EMITTER
	Object2D_Animated leaf;
	vector<Particle> leafParticles;
	ParticleEmitter leafEmitter;
	leaf.CreateAnimatedObject2D(&shader_Animation_NL, leafTex, 1, 1, 0, 0);
	float leafSize[2] = { 0.08, 0.08 };
	float leafPos[3] = { 0, 0, 1 };
	float leafRot[3] = { 0, 0, 1 };
	float leafColor[4] = { 0, 0, 0, 1 };
	leafEmitter.SetEmissionTime(-1);
	float leafPntList[2][2] = { {-1.0, 1.0},{-1.0, 0.5} };
	leafEmitter.RandPosBtw2Pts(leafPntList);
	leafEmitter.SetEmissionDelay(0.5);
	float leafChgColor[4] = { 0, 0, 0, 1 };
	float leafChgColorVel[4] = { 0, 0, 0, 0 };

	leafEmitter.SetParticleProperties(leaf, leafSize, 0, 60, 5000, leafColor, 0);

	// EARTH EMITTER
	Object2D_Animated earth;
	vector<Particle> earthParticles;
	ParticleEmitter earthEmitter;
	earth.CreateAnimatedObject2D(&shader_Animation_NL, earthTex, 1, 1, 0, 0);
	float earthSize[2] = { 0.0144, 0.0072 };
	float earthPos[3] = { 0, 0, 1 };
	float earthRot[3] = { 0, 0, 1 };
	float earthColor[4] = { 0, 0, 0, 1 };
	float earthPntList[2][2] = { {-1.0, 1.0},{-1.0, 0.5} };
	earthEmitter.SetPosition(earthPos);
	earthEmitter.SetEmissionTime(-1);
	earthEmitter.SetEmissionDelay(0);
	earthEmitter.RandPosBtw2Pts(earthPntList);
	earthEmitter.SetParticleList(earthParticles);
	float earthEmitterNewSize[2] = { 0.0, 0.0 };
	earthEmitter.SetParticleProperties(earth, earthSize, 0, 10, 5000, earthColor, 0);
	vector<array<float, 2>> earthEmitterPntList = { {-0.52721, 0.38033}, {-0.53985, 0.37401}, {-0.55144, 0.36452}, {-0.56566, 0.35399}, {-0.57356, 0.33976}, {-0.57988, 0.32659}, {-0.5841, 0.31237}, {-0.58463, 0.29499}, {-0.57778, 0.28024}, {-0.55987, 0.28129}, {-0.5409, 0.28498}, {-0.52562, 0.28919}, {-0.50982, 0.29393}, {-0.5, 0.3}, {-0.47771, 0.33739}, {-0.46568, 0.34452}, {-0.44875, 0.34674}, {-0.43806, 0.33516}, {-0.43494, 0.32091}, {-0.43316, 0.31111}, {-0.43271, 0.29952}, {-0.43271, 0.28393}, {-0.43494, 0.27368}, {-0.43984, 0.28616}, {-0.44964, 0.27948}, {-0.46701, 0.27324}, {-0.48, 0.28}, {-0.47414, 0.29507}, {-0.46211, 0.30799}, {-0.4483, 0.31779}, {-0.38854, 0.27032}, {-0.39196, 0.28501}, {-0.3949, 0.30068}, {-0.39637, 0.3139}, {-0.3998, 0.32663}, {-0.4, 0.34}, {-0.38658, 0.33006}, {-0.37577, 0.33936}, {-0.36432, 0.34294}, {-0.36, 0.34}, {-0.35752, 0.33042}, {-0.35609, 0.32433}, {-0.33049, 0.39317}, {-0.33009, 0.37728}, {-0.32968, 0.36628}, {-0.32764, 0.35284}, {-0.32561, 0.34144}, {-0.32479, 0.32718}, {-0.32357, 0.31415}, {-0.32, 0.3}, {-0.3195, 0.28808}, {-0.31868, 0.27342}, {-0.26093, 0.34762}, {-0.27276, 0.33858}, {-0.27902, 0.32605}, {-0.28389, 0.31214}, {-0.28389, 0.29544}, {-0.27902, 0.28292}, {-0.26928, 0.27457}, {-0.25701, 0.27584}, {-0.24511, 0.28576}, {-0.24, 0.3}, {-0.23866, 0.31454}, {-0.24114, 0.32793}, {-0.24908, 0.33934}, {-0.17566, 0.3443}, {-0.18359, 0.34728}, {-0.196, 0.34033}, {-0.20046, 0.33289}, {-0.20641, 0.32446}, {-0.20691, 0.31454}, {-0.19649, 0.31354}, {-0.18558, 0.31354}, {-0.17169, 0.31106}, {-0.16177, 0.3061}, {-0.1583, 0.2937}, {-0.16127, 0.28527}, {-0.17219, 0.27485}, {-0.18459, 0.27336}, {-0.1955, 0.27683}, {-0.05385, 0.26315}, {-0.05728, 0.28112}, {-0.0607, 0.29995}, {-0.06327, 0.31878}, {-0.06498, 0.33846}, {-0.06755, 0.35557}, {-0.06755, 0.37611}, {-0.05211, 0.37862}, {-0.03419, 0.37967}, {-0.01679, 0.37967}, {-0.0052, 0.37545}, {-0.00151, 0.36386}, {-0.00941, 0.35173}, {-0.01837, 0.34172}, {-0.02944, 0.33276}, {-0.03788, 0.32485}, {-0.04631, 0.31852}, {-0.0363, 0.31167}, {-0.02417, 0.30798}, {-0.00678, 0.30271}, {0.01114, 0.29955}, {0.02854, 0.29638}, {0.04593, 0.32274}, {0.05595, 0.34014}, {0.06966, 0.34488}, {0.08178, 0.34014}, {0.08969, 0.32485}, {0.09179, 0.30956}, {0.09127, 0.29638}, {0.0802, 0.28215}, {0.06597, 0.27425}, {0.04752, 0.28373}, {0.04373, 0.30358}, {0.17231, 0.39002}, {0.17362, 0.37612}, {0.17362, 0.35788}, {0.17579, 0.34094}, {0.17796, 0.3253}, {0.18144, 0.30836}, {0.1823, 0.29576}, {0.18535, 0.27752}, {0.18578, 0.26839}, {0.17188, 0.28924}, {0.16, 0.28}, {0.14277, 0.28186}, {0.12757, 0.29359}, {0.12627, 0.31009}, {0.13409, 0.32486}, {0.14625, 0.33486}, {0.16059, 0.34267}, {0.25908, 0.32892}, {0.26, 0.34}, {0.24995, 0.34424}, {0.24, 0.34}, {0.22989, 0.33147}, {0.22223, 0.32746}, {0.21675, 0.3406}, {0.21894, 0.31615}, {0.2215, 0.30448}, {0.22442, 0.2939}, {0.22733, 0.27602}, {0.22806, 0.26909}, {0.22563, 0.28485}, {0.21679, 0.33209}, {0.29322, 0.27141}, {0.29245, 0.28677}, {0.29092, 0.30175}, {0.2913, 0.31404}, {0.29169, 0.32709}, {0.29169, 0.33746}, {0.29169, 0.36127}, {0.29937, 0.37126}, {0.29284, 0.37356}, {0.28362, 0.36972}, {0.28593, 0.36358}, {0.36811, 0.34514}, {0.35736, 0.34668}, {0.34776, 0.34246}, {0.34085, 0.33516}, {0.33355, 0.32364}, {0.3324, 0.30866}, {0.33201, 0.29368}, {0.33393, 0.28139}, {0.3443, 0.2764}, {0.35506, 0.27486}, {0.36619, 0.28293}, {0.37387, 0.29406}, {0.37656, 0.31058}, {0.37925, 0.32748}, {0.38232, 0.29214}, {0.38271, 0.27678}, {0.38381, 0.25896}, {0.38318, 0.24219}, {0.37654, 0.22828}, {0.36832, 0.2172}, {0.34681, 0.21626}, {0.33763, 0.22764}, {0.29832, 0.36443}, {0.4168, 0.34158}, {0.4168, 0.32752}, {0.41738, 0.3146}, {0.42, 0.3}, {0.42111, 0.28849}, {0.42369, 0.28246}, {0.42484, 0.27586}, {0.4346, 0.27414}, {0.44722, 0.28189}, {0.45823, 0.29062}, {0.46341, 0.30185}, {0.466, 0.31584}, {0.466, 0.33104}, {0.46514, 0.34486}, {0.46721, 0.28889}, {0.47025, 0.27779}, {0.49495, 0.30491}, {0.50661, 0.30793}, {0.52173, 0.31441}, {0.53382, 0.32261}, {0.54073, 0.33514}, {0.53209, 0.34507}, {0.52216, 0.34723}, {0.51093, 0.34118}, {0.50877, 0.32736}, {0.50661, 0.31829}, {0.50661, 0.29281}, {0.51179, 0.27899}, {0.5187, 0.27338}, {0.53252, 0.28547}, {0.52649, 0.27954}, {0.54336, 0.2934}, {0.5524, 0.30183}, {0.56927, 0.3413}, {0.58132, 0.34341}, {0.59428, 0.34431}, {0.60482, 0.34672}, {0.61718, 0.34883}, {0.60874, 0.33618}, {0.60392, 0.32473}, {0.59789, 0.31268}, {0.59187, 0.30394}, {0.58494, 0.29189}, {0.58, 0.28}, {0.5765, 0.272}, {0.59368, 0.27502}, {0.61025, 0.27863}, {0.6232, 0.28014}, {0.63676, 0.28406}, {0.58614, 0.27472}, {0.35779, 0.214} };
	earthEmitter.RandBtwList(earthEmitterPntList);
	float earthChgColor[4] = { 0, 0, 0, 0 };
	float earthChgColorVel[4] = { 0, 0, 0, 0.08 };

	// WATER BASIS EMITTER
	Object2D_Animated waterBasis;
	vector<Particle> waterBasisParticles;
	ParticleEmitter waterBasisEmitter;
	waterBasis.CreateAnimatedObject2D(&shader_Animation_NL, waterTex, 1, 1, 0, 0);
	float waterBasisSize[2] = { 0.015, 0.015 };
	float waterBasisPos[3] = { 0, 0, 1 };
	float waterBasisRot[3] = { 0, 0, 1 };
	float waterBasisColor[4] = { 0, 0.5, 1, 1 };
	float waterBasisPntList[2][2] = { {-1.0, 1.0},{-1.0, 0.5} };
	waterBasisEmitter.SetPosition(waterBasisPos);
	waterBasisEmitter.SetEmissionTime(-1);
	waterBasisEmitter.SetEmissionDelay(0);
	waterBasisEmitter.RandPosBtw2Pts(waterBasisPntList);
	waterBasisEmitter.SetParticleList(waterBasisParticles);
	float waterBasisEmitterNewSize[2] = { 0.0, 0.0 };
	waterBasisEmitter.SetParticleProperties(waterBasis, waterBasisSize, 0, 15, 10000, waterBasisColor, 0);
	vector<array<float, 2>> waterBasisEmitterPntList = { {-0.5771, -0.02334}, {-0.57621, -0.03361}, {-0.57576, -0.04209}, {-0.57472, -0.05148}, {-0.57324, -0.06184}, {-0.57265, -0.07485}, {-0.57088, -0.08964}, {-0.5691, -0.10059}, {-0.5691, -0.11124}, {-0.56762, -0.11774}, {-0.56526, -0.12751}, {-0.58241, -0.07515}, {-0.56171, -0.07249}, {-0.55047, -0.06923}, {-0.53952, -0.06539}, {-0.52828, -0.06391}, {-0.51674, -0.06154}, {-0.50402, -0.05829}, {-0.51615, -0.04853}, {-0.51763, -0.03728}, {-0.51852, -0.02338}, {-0.52059, -0.01332}, {-0.51379, -0.07396}, {-0.5126, -0.08905}, {-0.51053, -0.0997}, {-0.50994, -0.11005}, {-0.50757, -0.12307}, {-0.50669, -0.13431}, {-0.41499, -0.09911}, {-0.42179, -0.10828}, {-0.43066, -0.11567}, {-0.44069, -0.12262}, {-0.45008, -0.12818}, {-0.45793, -0.1322}, {-0.46387, -0.1232}, {-0.4654, -0.11439}, {-0.46636, -0.1027}, {-0.46693, -0.09581}, {-0.48, -0.1}, {-0.45659, -0.09159}, {-0.44567, -0.08642}, {-0.43552, -0.08106}, {-0.42958, -0.07129}, {-0.4359, -0.06171}, {-0.44414, -0.05712}, {-0.45487, -0.05463}, {-0.46, -0.06}, {-0.46521, -0.06784}, {-0.46674, -0.07704}, {-0.46732, -0.08585}, {-0.38591, -0.1356}, {-0.38713, -0.12485}, {-0.38713, -0.11508}, {-0.38713, -0.10482}, {-0.38689, -0.09481}, {-0.38811, -0.08504}, {-0.38811, -0.07331}, {-0.38811, -0.06378}, {-0.38787, -0.03838}, {-0.38225, -0.03154}, {-0.38933, -0.02885}, {-0.39373, -0.03593}, {-0.34316, -0.02641}, {-0.34211, -0.03772}, {-0.34092, -0.04794}, {-0.34041, -0.06156}, {-0.33922, -0.07467}, {-0.33786, -0.09084}, {-0.33786, -0.1031}, {-0.33786, -0.11399}, {-0.33786, -0.1254}, {-0.33718, -0.13749}, {-0.35863, -0.06445}, {-0.35029, -0.06377}, {-0.33, -0.06}, {-0.32152, -0.05918}, {-0.27073, -0.05614}, {-0.28041, -0.06232}, {-0.28875, -0.07147}, {-0.29332, -0.08223}, {-0.2948, -0.09043}, {-0.29507, -0.0977}, {-0.29494, -0.10805}, {-0.29332, -0.11773}, {-0.28767, -0.12755}, {-0.27503, -0.13145}, {-0.26118, -0.12782}, {-0.25244, -0.11962}, {-0.24665, -0.1094}, {-0.24383, -0.09608}, {-0.24383, -0.08505}, {-0.24706, -0.07389}, {-0.2515, -0.06582}, {-0.26172, -0.0595}, {-0.16286, -0.07581}, {-0.16364, -0.06661}, {-0.16873, -0.05858}, {-0.18145, -0.06113}, {-0.18791, -0.06602}, {-0.19594, -0.07307}, {-0.20435, -0.07307}, {-0.20631, -0.06387}, {-0.20768, -0.05623}, {-0.20357, -0.08501}, {-0.20298, -0.10027}, {-0.20103, -0.11437}, {-0.19946, -0.12631}, {-0.19789, -0.13766}, {-0.00827, -0.01976}, {-0.01984, -0.02565}, {-0.03294, -0.03373}, {-0.04494, -0.0442}, {-0.05651, -0.05577}, {-0.06546, -0.07323}, {-0.06982, -0.09135}, {-0.07266, -0.10772}, {-0.07113, -0.11994}, {-0.05804, -0.12671}, {-0.03228, -0.12431}, {-0.02, -0.12}, {-0.0002, -0.11405}, {0.01595, -0.10684}, {0.03953, -0.06603}, {0.05175, -0.05795}, {0.06768, -0.05555}, {0.07925, -0.06297}, {0.08449, -0.0752}, {0.08776, -0.08982}, {0.08776, -0.10575}, {0.08776, -0.11732}, {0.08514, -0.13129}, {0.07685, -0.12103}, {0.0679, -0.12583}, {0.05546, -0.12933}, {0.04389, -0.1302}, {0.04, -0.12}, {0.04695, -0.10444}, {0.05808, -0.09506}, {0.06855, -0.08655}, {0.08, -0.08}, {0.16841, -0.0726}, {0.16569, -0.06078}, {0.16, -0.06}, {0.14992, -0.0623}, {0.14083, -0.06775}, {0.13385, -0.07351}, {0.12446, -0.07776}, {0.12264, -0.06806}, {0.12294, -0.05957}, {0.12537, -0.08564}, {0.12779, -0.09595}, {0.12779, -0.10534}, {0.12931, -0.11383}, {0.13113, -0.12262}, {0.13294, -0.13414}, {0.19317, -0.05857}, {0.19577, -0.06985}, {0.2, -0.08}, {0.20272, -0.08721}, {0.20648, -0.09646}, {0.20966, -0.10717}, {0.21313, -0.11643}, {0.21776, -0.12771}, {0.22065, -0.13465}, {0.22702, -0.12077}, {0.23078, -0.10804}, {0.23599, -0.09502}, {0.24004, -0.08402}, {0.24496, -0.07158}, {0.24814, -0.06001}, {0.27562, -0.06638}, {0.28488, -0.05914}, {0.29645, -0.05625}, {0.30831, -0.05654}, {0.31757, -0.07043}, {0.32075, -0.08605}, {0.32191, -0.09646}, {0.32191, -0.10688}, {0.32, -0.12}, {0.31931, -0.13263}, {0.31063, -0.12077}, {0.29761, -0.128}, {0.2843, -0.13031}, {0.27331, -0.12915}, {0.27331, -0.11787}, {0.28083, -0.10688}, {0.28835, -0.09791}, {0.29761, -0.09126}, {0.30687, -0.08489}, {0.35258, -0.01025}, {0.35359, -0.02582}, {0.35512, -0.04449}, {0.35639, -0.05613}, {0.35788, -0.06836}, {0.36, -0.08}, {0.36125, -0.09184}, {0.36328, -0.10485}, {0.36531, -0.11729}, {0.36646, -0.13176}, {0.35412, -0.03375}, {0.35287, -0.0172}, {0.39944, -0.00765}, {0.4, -0.02}, {0.39973, -0.03571}, {0.40118, -0.05452}, {0.40262, -0.06696}, {0.40291, -0.07882}, {0.40465, -0.08894}, {0.40668, -0.10254}, {0.40841, -0.11527}, {0.40928, -0.12655}, {0.40986, -0.13407}, {0.41449, -0.07506}, {0.4223, -0.06638}, {0.43127, -0.05741}, {0.44371, -0.05596}, {0.45181, -0.0603}, {0.45528, -0.06811}, {0.45528, -0.08055}, {0.45557, -0.09444}, {0.45615, -0.10341}, {0.45528, -0.1144}, {0.45412, -0.12655}, {0.45412, -0.13205}, {0.52, -0.06}, {0.50583, -0.0658}, {0.5, -0.08}, {0.49734, -0.09504}, {0.49671, -0.10511}, {0.5, -0.12}, {0.51149, -0.13121}, {0.52344, -0.13058}, {0.5335, -0.12397}, {0.53979, -0.11422}, {0.54388, -0.09913}, {0.5442, -0.08467}, {0.54136, -0.07177}, {0.53508, -0.06297}, {-0.04605, -0.12699} };
	waterBasisEmitter.RandBtwList(waterBasisEmitterPntList);
	float waterBasisChgColor[4] = { 1, 1, 1, 0 };
	float waterBasisChgColorVel[4] = { 0.05, 0.05, 0.05, 0.05 };

	// WATER EMITTER
	vector<Particle> waterParticles;
	ParticleEmitter waterEmitter;
	waterEmitter.SetPosition(waterBasisPos);
	waterEmitter.SetEmissionTime(-1);
	waterEmitter.SetEmissionDelay(0);
	waterEmitter.SetParticleList(waterParticles);
	waterEmitter.SetParticleProperties(waterBasis, waterBasisSize, 0, 15, 10000, waterBasisColor, 0);
	waterEmitter.RandBtwList(waterBasisEmitterPntList);
	float waterEmitterNewSize[2] = { 0.0, 0.0 };
	float waterChgColor[4] = { 1, 1, 1, 1 };
	float waterChgColorVel[4] = { 0.6, 0.6, 0.6, 0 };

	// FIRE BASIS EMITTER
	Object2D_Animated fireBasis;
	vector<Particle> fireBasisParticles;
	ParticleEmitter fireBasisEmitter;
	fireBasis.CreateAnimatedObject2D(&shader_Animation_NL, fireTex, 1, 1, 0, 0);
	float fireBasisSize[2] = { 0.015, 0.015 };
	float fireBasisPos[3] = { 0, 0, 1 };
	float fireBasisRot[3] = { 0, 0, 1 };
	float fireBasisColor[4] = { 1, 0.8, 0, 1 };
	fireBasisEmitter.SetPosition(fireBasisPos);
	fireBasisEmitter.SetEmissionTime(-1);
	fireBasisEmitter.SetEmissionDelay(0);
	fireBasisEmitter.SetParticleList(fireBasisParticles);
	fireBasisEmitter.SetParticleProperties(fireBasis, fireBasisSize, 0, 15, 10000, fireBasisColor, 0);
	vector<array<float, 2>> fireBasisEmitterPntList = { {-0.54345, -0.58012}, {-0.54597, -0.5685}, {-0.54773, -0.554}, {-0.54904, -0.5417}, {-0.55036, -0.52763}, {-0.55256, -0.51093}, {-0.55476, -0.49467}, {-0.55695, -0.48061}, {-0.55827, -0.47006}, {-0.5697, -0.47358}, {-0.58, -0.48}, {-0.56, -0.54}, {-0.53235, -0.53115}, {-0.51696, -0.51972}, {-0.50422, -0.50918}, {-0.49411, -0.49731}, {-0.48664, -0.48105}, {-0.49323, -0.46743}, {-0.50861, -0.46084}, {-0.52487, -0.45864}, {-0.54, -0.46}, {-0.45368, -0.5061}, {-0.44345, -0.50024}, {-0.42996, -0.4967}, {-0.42, -0.5}, {-0.41294, -0.50923}, {-0.40844, -0.51726}, {-0.40587, -0.52754}, {-0.40555, -0.54231}, {-0.40426, -0.55612}, {-0.40491, -0.56961}, {-0.40587, -0.57861}, {-0.41583, -0.56319}, {-0.42386, -0.56897}, {-0.43349, -0.57443}, {-0.44505, -0.57732}, {-0.45501, -0.57636}, {-0.46336, -0.56672}, {-0.45533, -0.55484}, {-0.44698, -0.54424}, {-0.43831, -0.53717}, {-0.42932, -0.53043}, {-0.41904, -0.52497}, {-0.36446, -0.49791}, {-0.36446, -0.50706}, {-0.36446, -0.51808}, {-0.36446, -0.5326}, {-0.36339, -0.54443}, {-0.36124, -0.55464}, {-0.35935, -0.56674}, {-0.35693, -0.57696}, {-0.34618, -0.57884}, {-0.33489, -0.574}, {-0.32575, -0.56594}, {-0.31768, -0.55733}, {-0.3115, -0.5412}, {-0.30961, -0.52641}, {-0.30935, -0.51566}, {-0.31042, -0.50007}, {-0.30693, -0.55034}, {-0.30666, -0.56163}, {-0.30585, -0.57185}, {-0.30558, -0.57857}, {-0.25692, -0.58072}, {-0.2588, -0.56674}, {-0.26256, -0.54954}, {-0.26606, -0.53098}, {-0.26714, -0.517}, {-0.26902, -0.50275}, {-0.27009, -0.48904}, {-0.27009, -0.47936}, {-0.2709, -0.46592}, {-0.2709, -0.45409}, {-0.2709, -0.44092}, {-0.19434, -0.49532}, {-0.20366, -0.50092}, {-0.21005, -0.50944}, {-0.21591, -0.51903}, {-0.22044, -0.52942}, {-0.2231, -0.54034}, {-0.22044, -0.55392}, {-0.21565, -0.56511}, {-0.20872, -0.5723}, {-0.201, -0.57656}, {-0.18954, -0.57443}, {-0.17756, -0.56484}, {-0.17037, -0.55232}, {-0.16531, -0.53607}, {-0.1677, -0.52169}, {-0.17276, -0.50971}, {-0.18, -0.5}, {-0.18715, -0.49719}, {-0.00683, -0.45857}, {-0.01189, -0.45031}, {-0.02361, -0.44605}, {-0.03586, -0.45031}, {-0.04785, -0.4567}, {-0.05717, -0.46496}, {-0.06329, -0.47375}, {-0.06862, -0.48653}, {-0.06409, -0.49479}, {-0.04385, -0.49985}, {-0.05371, -0.49772}, {-0.0284, -0.50251}, {-0.01562, -0.50624}, {-0.0031, -0.50997}, {0.00809, -0.5161}, {0.01741, -0.52622}, {0.02, -0.54}, {0.01741, -0.55179}, {0.01235, -0.56271}, {0.00542, -0.5723}, {-0.00496, -0.57922}, {-0.02, -0.58}, {-0.03133, -0.57816}, {-0.03986, -0.56963}, {-0.04, -0.56}, {-0.03906, -0.54673}, {0.05902, -0.58316}, {0.05845, -0.56993}, {0.05864, -0.56035}, {0.05845, -0.5498}, {0.05845, -0.53811}, {0.05768, -0.52622}, {0.05787, -0.5153}, {0.05825, -0.497}, {0.05825, -0.47675}, {0.06343, -0.4691}, {0.05688, -0.46456}, {0.05048, -0.46789}, {0.05048, -0.47557}, {0.10959, -0.57997}, {0.10729, -0.56206}, {0.10601, -0.54978}, {0.10499, -0.53826}, {0.1032, -0.52572}, {0.10089, -0.51421}, {0.1, -0.5}, {0.09885, -0.48836}, {0.09833, -0.47327}, {0.09731, -0.46047}, {0.09833, -0.4464}, {0.14, -0.5}, {0.14311, -0.51037}, {0.14695, -0.52035}, {0.15182, -0.53417}, {0.15719, -0.54594}, {0.16, -0.56}, {0.16487, -0.56897}, {0.16947, -0.57895}, {0.17638, -0.56923}, {0.18, -0.56}, {0.18303, -0.54466}, {0.18764, -0.53212}, {0.1925, -0.52086}, {0.19762, -0.50935}, {0.2, -0.5}, {0.22884, -0.50525}, {0.24, -0.5}, {0.2511, -0.49527}, {0.26262, -0.49604}, {0.26901, -0.50397}, {0.27337, -0.51446}, {0.27592, -0.52675}, {0.28, -0.54}, {0.27976, -0.55618}, {0.27951, -0.56846}, {0.28, -0.58}, {0.27004, -0.56411}, {0.26211, -0.56769}, {0.25213, -0.57434}, {0.24163, -0.57741}, {0.23345, -0.57921}, {0.22526, -0.57409}, {0.22602, -0.56053}, {0.23345, -0.54824}, {0.24, -0.54}, {0.24931, -0.53391}, {0.25673, -0.52905}, {0.26518, -0.52444}, {0.28027, -0.54747}, {0.10857, -0.57051}, {0.05787, -0.50552} };
	fireBasisEmitter.RandBtwList(fireBasisEmitterPntList);
	float fireBasisChgColor[4] = { 1, 0, 0, 0 };
	float fireBasisChgColorVel[4] = { 0.05, 0.05, 0.05, 0.05 };

	// FIRE EMITTER 
	vector<Particle> fireParticles;
	ParticleEmitter fireEmitter;
	fireEmitter.SetPosition(fireBasisPos);
	fireEmitter.SetEmissionTime(-1);
	fireEmitter.SetEmissionDelay(0);
	fireEmitter.SetParticleList(fireParticles);
	fireEmitter.SetParticleProperties(fireBasis, fireBasisSize, 0, 15, 10000, fireBasisColor, 0);
	fireEmitter.RandBtwList(fireBasisEmitterPntList);
	float fireEmitterNewSize[2] = { 0.0, 0.0 };
	float fireChgColor[4] = { 1, 0, 0, 0 };
	float fireChgColorVel[4] = { 0.2, 0.2, 0.2, 0.2 };

	// --- especifica a unidade de textura que cada sampler pertence (só precisa ser feito uma vez)
	Texture texture;
	texture.AssignTextureUnits(shader_Interface, "texture0", 0);
	texture.AssignTextureUnits(shader_Animation_NL, "texture0", 0);
	texture.AssignTextureUnits(shader_Static_NL, "texture0", 0);
	texture.AssignTextureUnits(shader_Static, "dif", 0);
	texture.AssignTextureUnits(shader_Static, "norm", 1);
	texture.AssignTextureUnits(shader_Static, "spec", 2);
	texture.AssignTextureUnits(shader_Static, "amb", 3);
	texture.AssignTextureUnits(shader_Static, "emit", 4);
	texture.AssignTextureUnits(shader_Animation, "dif", 0);
	texture.AssignTextureUnits(shader_Animation, "norm", 1);
	texture.AssignTextureUnits(shader_Animation, "spec", 2);
	texture.AssignTextureUnits(shader_Animation, "amb", 3);
	texture.AssignTextureUnits(shader_Animation, "emit", 4);

	Map forest;

	Object2D_Animated you_lost;
	Object2D_Interface playerHeart;
	Object2D_Interface playerGold;
	Object2D_Static menu_backGround;
	Object2D_Static play_glow;
	Object2D_Static exit_glow;
	Object2D_Static credits_glow;
	you_lost.CreateAnimatedObject2D(&shader_Animation_NL, you_lostTex, 5, 5, 0, 0);
	playerHeart.CreateInterfaceObject2D(&shader_Interface, playerHeartTex);
	playerGold.CreateInterfaceObject2D(&shader_Interface, playerGoldTex);
	menu_backGround.CreateStaticObject2D(&shader_Static_NL, menu_backGroundTex);
	play_glow.CreateStaticObject2D(&shader_Static_NL, play_glow_Tex);
	exit_glow.CreateStaticObject2D(&shader_Static_NL, exit_glow_Tex);
	credits_glow.CreateStaticObject2D(&shader_Static_NL, credits_glow_Tex);
	float you_lostPos[3] = { 0, 0, 1 };
	float you_lostSize[2] = { 1, 1 };
	float you_lostColor[4] = { 0, 0, 0, 1 };
	float yout_lostRot[3] = { 0, 0, 1 };
	float playerHeartPos[3] = { -0.9, 0.9, 1.0 };
	float playerHeartSize[2] = { 0.08, 0.08 };
	float playerGoldPos[3] = { -0.5, 0.9, 1.0 };
	float playerGoldSize[2] = { 0.08, 0.08 };
	float menu_backGroundPos[3] = { 0, 0, 1.0 };
	float menu_backGroundSize[2] = { 1, 1 };
	float menu_backGroundRot[3] = { 0, 0, 1 };
	float menu_backGroundColor[4] = { 0, 0, 0, 1 };
	float playButtonPos[2] = { forest.position[0] - 0.5f , forest.position[1] - 0.53f };
	float playButtonSize[2] = { forest.size[0] / 6.4f, forest.size[1] / 16.0f };
	float exitButtonPos[2] = { forest.position[0] - 0.45f , forest.position[1] - 0.73f };
	float exitButtonSize[2] = { forest.size[0] / 5.81f, forest.size[1] / 21.3f };
	float creditsButtonPos[2] = { 0.77f, -0.73f };
	float creditsButtonSize[2] = { 0.14, 0.05 };
	float buttonRot[3] = { 0, 0, 1 };
	float buttonColor[4] = { 0, 0, 0, 1 };

	Player player;

	Graph forestPath;
	vector<GameObject*> gameObjects;

	vector<Enemy*> enemies;
	int outwardEnemies = 0;
	int totalOutwardEnemies = 0;

	TimeStep skeletonTime;
	float skeletonTimer = 0.0f;
	const int totalSkeletons = 800;
	Skeleton *skeletons = new Skeleton[totalSkeletons];
	TimeStep skeletonReleaseTime;
	float skeletonReleaseTimer = 0.0f;
	float skeletonReleaseSeconds = 1.0f;
	int totalAliveSkeletons = 0;
	int skeletonsIterator = 0;
	TimeStep skeletonVoiceTime;
	float skeletonVoiceTimer = 0.0f;

	TimeStep ogreTime;
	float ogreTimer = 0.0f;
	const int totalOgres = 800;
	Ogre *ogres = new Ogre[totalOgres];
	TimeStep ogreReleaseTime;
	float ogreReleaseTimer = 0.0f;
	float ogreReleaseSeconds = 10.0f;
	int totalAliveOgres = 0;
	int ogresIterator = 0;

	int totalArcherTowers = 0;
	
	vector<ArcherTower*> archerTowers;

	MapObject *stump_1 = new MapObject[5];
	MapObject *trees_1 = new MapObject[10];
	MapObject *trees_2 = new MapObject[10];
	MapObject *stones_1 = new MapObject[10];
	MapObject *stones_2 = new MapObject[10];
	MapObject *bushes = new MapObject[20];

	vector<BuildingArea> buildingAreas;
	float buildingAreasPos[8][3] = { { -0.2, -0.14, 1.0 }, { 0.2, 0.21, 1.0 }, { -0.2, 0.21, 1.0 }, { 0.2, -0.14, 1.0 }, { -0.2, -0.7, 1.0 }, { 0.2, 0.7, 1.0 }, { 0.7, -0.14, 1.0 }, { -0.7, 0.21, 1.0 } };
	for (int i = 0; i < 8; i++)
	{
		BuildingArea *ba = new BuildingArea;
		buildingAreas.push_back(*ba);
		delete ba;
	}

	TimeStep lightTime;
	float lightTimer;
	float dayDurationSeconds = 60.0f;
	float lightLevel = 0.2f;

	bool leftMousePressed = false;
	bool rightMousePressed = false;

	bool inMenu = true;
	bool inCredits = false;
	bool firstLoad = true;
	bool quit = 0;
	while (quit == 0) {

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		float viewXY[2] = { viewX, viewY };
		SDL_GetMouseState(&mouseX, &mouseY);
		int mouseXY[2] = { mouseX, mouseY };
		float mouseCoords[2] = { mu.ScreenToVertexCoords(window, mouseXY)[0],   mu.ScreenToVertexCoords(window, mouseXY)[1] };
		
		if (inMenu)
		{
			if (firstLoad)
			{
				firstLoad = false;
				delete[] stump_1;
				delete[] trees_1;
				delete[] trees_2;
				delete[] stones_1;
				delete[] bushes;
				delete[] skeletons;

				player.CreatePlayer();
				skeletonTimer = 0.0f;
				skeletonReleaseSeconds = 1.0f;
				ogreReleaseSeconds = 10.0f;

				mainTheme.ResetMusic();
				battleMusic.ResetMusic();
				firstBattleMusicPlay = true;
				mainTheme.PlayAudio();
				selector.isOver = false;
				selector.selected = false;
				enemies.clear();
				gameObjects.clear();
				archerTowers.clear();

				lightTimer = 0.0f;
				totalArcherTowers = 0;
				totalAliveSkeletons = 0;
				skeletonsIterator = 0;
				outwardEnemies = 0;
				totalOutwardEnemies = 0;

				skeletons = new Skeleton[totalSkeletons];
				ogres = new Ogre[totalOgres];

				stump_1 = new MapObject[5];

				for (int i = 0; i < 5; i++)
				{
					stump_1[i].size[1] = mu.GenRandFloat(0.04f, 0.06f);
					stump_1[i].size[0] = mu.GenRandFloat(0.05f, 0.08f);
					float stump_1_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - stump_1_Pos[1]) / 2.0f) + (stump_1[i].size[1] / 2.0f) };
					stump_1[i].SetPosition(stump_1_Pos);
					stump_1[i].CreateMapObject(shader_Static, stump_1Tex, gameObjects);
				}

				trees_1 = new MapObject[10];

				for (int i = 0; i < 5; i++)
				{
					trees_1[i].size[1] = mu.GenRandFloat(0.15f, 0.2f);
					trees_1[i].size[0] = mu.GenRandFloat(0.05f, 0.10f);
					float tree_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - tree_Pos[1]) / 2.0f) + (trees_1[i].size[1] / 2.0f) };
					trees_1[i].SetPosition(tree_Pos);
					trees_1[i].CreateMapObject(shader_Static, tree_1Tex, gameObjects);
				}

				trees_2 = new MapObject[10];

				for (int i = 0; i < 5; i++)
				{
					trees_2[i].size[1] = mu.GenRandFloat(0.12f, 0.15f);
					trees_2[i].size[0] = mu.GenRandFloat(0.04f, 0.08f);
					float tree_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - tree_Pos[1]) / 2.0f) + (trees_2[i].size[1] / 2.0f) };
					trees_2[i].SetPosition(tree_Pos);
					trees_2[i].CreateMapObject(shader_Static, tree_2Tex, gameObjects);
				}

				stones_1 = new MapObject[10];

				for (int i = 0; i < 10; i++)
				{
					stones_1[i].size[1] = mu.GenRandFloat(0.05f, 0.06f);
					stones_1[i].size[0] = mu.GenRandFloat(0.05f, 0.1f);
					float stone_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - stone_Pos[1]) / 2.0f) + (stones_1[i].size[1] / 2.0f) };
					stones_1[i].SetPosition(stone_Pos);
					stones_1[i].CreateMapObject(shader_Static, stone_1Tex, gameObjects);
				}

				stones_2 = new MapObject[10];

				for (int i = 0; i < 10; i++)
				{
					stones_2[i].size[1] = mu.GenRandFloat(0.03f, 0.04f);
					stones_2[i].size[0] = mu.GenRandFloat(0.03f, 0.04f);
					float stone_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - stone_Pos[1]) / 2.0f) + (stones_2[i].size[1] / 2.0f) };
					stones_2[i].SetPosition(stone_Pos);
					stones_2[i].CreateMapObject(shader_Static, stone_2Tex, gameObjects);
				}

				bushes = new MapObject[20];
				for (int i = 0; i < 20; i++)
				{
					bushes[i].size[1] = mu.GenRandFloat(0.035f, 0.045f);
					bushes[i].size[0] = mu.GenRandFloat(0.025f, 0.05f);
					float bushes_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - bushes_Pos[1]) / 2.0f) + (bushes[i].size[1] / 2.0f) };
					bushes[i].SetPosition(bushes_Pos);
					bushes[i].CreateMapObject(shader_Static, bush_1Tex, gameObjects);
				}

				forest.CreateMap(shader_Static, forestTex);
				particle.CreateAnimatedObject2D(&shader_Animation_NL, particleTex, 1, 1, 0, 0);
				arrow.CreateAnimatedObject2D(&shader_Animation, arrowTex, 1, 1, 0, 0);
				buildingArea.CreateStaticObject2D(&shader_Static, buildingAreaTex);
				buildingAreaOver.CreateStaticObject2D(&shader_Static_NL, buildingAreaOverTex);
				tower_archer_over.CreateStaticObject2D(&shader_Static_NL, tower_archer_overTex);

				for (size_t i = 0; i < buildingAreas.size(); i++)
				{
					buildingAreas[i].CreateBuildingArea(buildingArea, buildingAreaOver, buildingAreasPos[i], &shader_Animation_NL, &shader_Static_NL, &shader_Animation, &shader_Static, towersMenuTex, text, &gameObjects);
					buildingAreas[i].LoadArcherTower(&archerTowers, towersUpgradeTex, towersUpgradeDTex, towerArcherTex, arrowTex, rangeTex, &arrowShot_Sounds, &totalArcherTowers, tower_archer_over, particle);
				}

			}

			menu_backGround.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, menu_backGroundRot, 0, menu_backGroundColor);
			if (selector.isUnderMouse(mouseCoords, playButtonPos, playButtonSize))
			{
				play_glow.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, buttonRot, 0, buttonColor);
				if (selector.isSelected(mouseCoords, playButtonPos, playButtonSize, leftMousePressed)) inMenu = false;
			}
			if (selector.isUnderMouse(mouseCoords, exitButtonPos, exitButtonSize))
			{
				exit_glow.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, buttonRot, 0, buttonColor);
				if (selector.isSelected(mouseCoords, exitButtonPos, exitButtonSize, leftMousePressed)) quit = 1;
			}
			if (selector.isUnderMouse(mouseCoords, creditsButtonPos, creditsButtonSize))
			{
				credits_glow.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, buttonRot, 0, buttonColor);
				if (selector.isSelected(mouseCoords, creditsButtonPos, creditsButtonSize, leftMousePressed))
				{
					inCredits = true;
					inMenu = false;
				}
			}

			float cloudsVel[2] = { mu.GenRandFloat(0.01, 0.02), mu.GenRandFloat(-0.0005, 0.005) };
			cloudsEmitter.SetVelocity(cloudsVel);
			cloudsEmitter.ChangeColor(cloudsChgColor, cloudsChgColorVel);
			cloudsEmitter.Emit();

			float leafVel[2] = { mu.GenRandFloat(0.001, 0.2), mu.GenRandFloat(-0.1, -0.05) };
			leafEmitter.SetVelocity(leafVel);
			leafEmitter.ChangeColor(leafChgColor, leafChgColorVel);
			leafEmitter.Rotate(30);
			leafEmitter.Emit();
		}
		else if (inCredits)
		{
			text.RenderText("Developers and stuff...", -0.8, 0.7, 1.5, glm::vec3(1.0, 1.0f, 0.0f));

			float earthVel[2] = { mu.GenRandFloat(-0.001, 0.001), mu.GenRandFloat(-0.002, -0.001) };
			earthEmitter.Rotate(180);
			earthEmitter.SetVelocity(earthVel);
			earthEmitter.ChangeSize(earthEmitterNewSize, 0.09);
			earthEmitter.Emit();
			earthEmitter.Emit();
			earthEmitter.Emit();

			waterBasisEmitter.ChangeColor(waterBasisChgColor, waterBasisChgColorVel);
			waterBasisEmitter.Emit();
			float waterVel[2] = { mu.GenRandFloat(-0.001, 0.001), mu.GenRandFloat(-0.08, -0.04) };
			waterEmitter.SetVelocity(waterVel);
			waterEmitter.ChangeColor(waterChgColor, waterChgColorVel);
			waterEmitter.ChangeSize(waterEmitterNewSize, 0.4);
			waterEmitter.Emit();

			fireBasisEmitter.ChangeColor(fireBasisChgColor, fireBasisChgColorVel);
			fireBasisEmitter.Emit();
			float fireVel[2] = { mu.GenRandFloat(-0.02, 0.02), mu.GenRandFloat(0.01, 0.05) };
			fireEmitter.SetVelocity(fireVel);
			fireEmitter.ChangeColor(fireChgColor, fireChgColorVel);
			fireEmitter.ChangeSize(fireEmitterNewSize, 0.05);
			fireEmitter.Emit();

		}
		else
		{
			if (firstBattleMusicPlay)
			{
				mainTheme.ResumeOrPauseMusic();
				battleMusic.PlayAudio();
				firstBattleMusicPlay = false;
			}

			skeletonTimer += skeletonTime.step(1);
			if (skeletonTimer > 10.0f)
			{
				if (skeletonReleaseSeconds > 10.0f)
				skeletonReleaseSeconds -= 0.1f;
				skeletonTimer = 0.0f;
			}

			ogreTimer += ogreTime.step(1);
			if (ogreTimer > 10.0f)
			{
				if (ogreReleaseSeconds > 10.0f)
					ogreReleaseSeconds -= 0.1f;
				ogreTimer = 0.0f;
			}

			// ILUMINAÇÃO DIA E NOITE
			if (lightTimer <= 2.0f * PI)
			{
				lightTimer += lightTime.step(1) * (2.0f * PI) / dayDurationSeconds;
			}
			else lightTimer = 0.0f;
			float lightPosTemp[2] = { cos(lightTimer) , sin(lightTimer) };
			float lightposition_day[3] = { mu.TextureToVertexCoords(lightPosTemp)[0], mu.TextureToVertexCoords(lightPosTemp)[1], lightLevel };
			float lightIntensity_day = (lightposition_day[1] + 1.0f) / 2.0f;
			float difuseLightColor[3] = { lightIntensity_day * 0.7f + abs(cos(lightTimer)) * 0.1f, lightIntensity_day * 0.7f, lightIntensity_day * 1.0f };
			float specularLightColor[3] = { lightIntensity_day * 0.7f + abs(cos(lightTimer)) * 0.1f, lightIntensity_day * 0.7f, lightIntensity_day * 1.0f };
			float ambientLightColor[3] = { lightIntensity_day * 0.5f + abs(cos(lightTimer)) * 0.08f, lightIntensity_day * 0.5f, lightIntensity_day * 0.5f };
			float lightposition_night[3] = { mu.TextureToVertexCoords(lightPosTemp)[0], mu.TextureToVertexCoords(lightPosTemp)[1], lightLevel };
			float lightIntensity_night = (-lightposition_night[1] + 1.0f) / 2.0f;
			float difuseLightColor2[3] = { lightIntensity_night * 0.3f + abs(cos(lightTimer)) * 0.1f, lightIntensity_night * 0.3f, lightIntensity_night * 0.8f };
			float specularLightColor2[3] = { lightIntensity_night * 0.3f + abs(cos(lightTimer)) * 0.1f, lightIntensity_night * 0.3f, lightIntensity_night * 0.8f };
			float ambientLightColor2[3] = { lightIntensity_day * 0.1f + abs(cos(lightTimer)) * 0.08f, lightIntensity_night * 0.1f, lightIntensity_night * 0.2f };
			float ambientLightColor3[3] = { 0.0f , 0.0f , 0.0f };
			float shininess = 1.0f;
			float falloff[3] = { .4f, 3.0f, 2.0f };
			Light light(lighShaderList, lightposition_day, difuseLightColor, specularLightColor, ambientLightColor, shininess, falloff);
			Light light2(lighShaderList, lightposition_night, difuseLightColor2, specularLightColor2, ambientLightColor2, shininess, falloff);
			Light light3(lighShaderList, lightposition_day, difuseLightColor, specularLightColor, ambientLightColor3, shininess, falloff);
			Light light4(lighShaderList, lightposition_night, difuseLightColor2, specularLightColor2, ambientLightColor3, shininess, falloff);
			LightSystem lightSys;
			light.EnableDirectionalLight();
			light2.EnableDirectionalLight();
			lightSys.AddLight(light);
			lightSys.AddLight(light2);
			lightSys.AddLight(light3);
			lightSys.AddLight(light4);
			lightSys.DrawLights(lighShaderList);

			//OBS: Quanto maior o Z do obj, mais proximo ele está da câmera. SEMPRE desenhar na ordem do Z.
			forest.DrawMapObject(viewXY);

			skeletonReleaseTimer += skeletonReleaseTime.step(1);
			if (skeletonReleaseTimer >= skeletonReleaseSeconds && skeletonsIterator < totalSkeletons)
			{
				skeletons[skeletonsIterator].CreateSkeleton(shader_Animation, skeleton_w_Tex, skeleton_d_Tex, forestPath, &skeletonDying_Sounds, gameObjects, enemies, player);
				skeletonReleaseTimer = 0.0f;
				skeletonsIterator++;
			}

			for (int i = 0; i < totalSkeletons; i++)
			{
				float skeletonsAlive = 0;
				if (skeletons[i].alive) skeletonsAlive++;
				totalAliveSkeletons = skeletonsAlive;
			}

			ogreReleaseTimer += ogreReleaseTime.step(1);
			if (ogreReleaseTimer >= ogreReleaseSeconds && ogresIterator < totalOgres)
			{
				ogres[ogresIterator].CreateOgre(shader_Animation, ogre_w_Tex, ogre_d_Tex, forestPath, &ogreDying_Sounds, gameObjects, enemies, player);
				ogreReleaseTimer = 0.0f;
				ogresIterator++;
			}

			for (int i = 0; i < totalOgres; i++)
			{
				float ogresAlive = 0;
				if (ogres[i].alive) ogresAlive++;
				totalAliveOgres = ogresAlive;
			}

			skeletonVoiceTimer += skeletonVoiceTime.step(1);
			if (totalAliveSkeletons > 0 && skeletonVoiceTimer > 30.0f)
			{
				skeletonVoice.PlayAudio();
				skeletonVoiceTimer = 0.0f;
			}

			bool enemyOutward = false;
			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i]->outward) {
					outwardEnemies++;
					enemyOutward = true;	
				} 
			}
			totalOutwardEnemies = outwardEnemies;
			outwardEnemies = 0;
			if (enemyOutward) {
				if (player.GetLife() > 0) player.SetLife(player.GetInitialLife() - totalOutwardEnemies);
				enemyOutward = false;
			}

			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				if (!gameObjects[i]->exists)
				{
					gameObjects.erase(gameObjects.begin() + i);
				}
			}

			sorter.QuickSort(gameObjects, 0, gameObjects.size() - 1);

			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->Draw(viewXY);
			}

			for (int i = 0; i < archerTowers.size(); i++)
			{
				archerTowers[i]->SetTarget(enemies);
			}

			for (size_t i = 0; i < buildingAreas.size(); i++)
			{
				buildingAreas[i].DrawBuildingArea(mouseCoords, viewXY, leftMousePressed, player);
			}

			//Objetos de interface aqui
			playerHeart.DrawInterface(playerHeartSize, playerHeartPos, viewXY);
			text.RenderText(to_string(player.GetLife()), playerHeartPos[0] + 0.075, playerHeartPos[1] - 0.01, 0.9, glm::vec3(1.0, 0.0f, 0.0f));
			playerGold.DrawInterface(playerGoldSize, playerGoldPos, viewXY);
			text.RenderText(to_string(player.GetGold()), playerGoldPos[0] + 0.07, playerGoldPos[1] - 0.01, 0.9, glm::vec3(1.0, 0.0f, 0.0f));

			if (player.GetLife() <= 0)
			{
				player.Lost();
				you_lost.DrawAnimated(you_lostSize, you_lostPos, viewXY, yout_lostRot, 0, you_lostColor, false, 15, false);
			}

		}

		//PROCESSA INPUTS
		leftMousePressed = false;
		rightMousePressed = false;

		window.ProcessInput(SDL_SCANCODE_UP, &viewY, viewY + 0.005f);
		window.ProcessInput(SDL_SCANCODE_DOWN, &viewY, viewY - 0.005f);
		window.ProcessInput(SDL_SCANCODE_RIGHT, &viewX, viewX + 0.005f);
		window.ProcessInput(SDL_SCANCODE_LEFT, &viewX, viewX - 0.005f);
		window.ProcessInput(SDL_SCANCODE_ESCAPE, &inMenu, true);
		window.ProcessInput(SDL_SCANCODE_ESCAPE, &firstLoad, true);
		window.ProcessInput(SDL_SCANCODE_ESCAPE, &inCredits, false);

		switch (window.SDL_ManageEvent())
		{
		case SDL_QUIT:
			quit = 1;
			SDL_Quit();
			Mix_Quit();
			glDeleteVertexArrays(1, vertices.GetVao());
			glDeleteBuffers(1, vertices.GetVbo());
			glDeleteBuffers(1, vertices.GetEbo());
			break;

		case SDL_MOUSEWHEEL:
			if (window.getEvnt().wheel.y > 0)
			{
				//if (lightLevel >= 0.0f)
				//lightLevel -= 0.04f;
			}
			else if (window.getEvnt().wheel.y < 0)
			{
				//if (lightLevel <= 5.0f)
				//lightLevel += 0.04f;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (window.getEvnt().button.button == (SDL_BUTTON_LEFT))
			{
				if (leftMousePressed) leftMousePressed = false;
				else leftMousePressed = true;
				rightMousePressed = false;
				//std::cout << "esquerdo pressionado!" << std::endl;
			}
			else if (window.getEvnt().button.button == (SDL_BUTTON_RIGHT))
			{
				if (rightMousePressed) rightMousePressed = false;
				else rightMousePressed = true;
				leftMousePressed = false;
				//std::cout << "direito pressionado!" << std::endl;
			}
			break;
		}

		window.SDL_SwapWindow();
	}

	return 0;
}

