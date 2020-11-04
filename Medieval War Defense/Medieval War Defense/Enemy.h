#pragma once

#include "MathUtils.h"
#include "Graph.h"
#include "TimeStep.h"
#include "Object2D_Animated.h"
#include "GameObject.h"
#include "Sound.h"
#include "Player.h"


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
	vector<GameObject*>* gameObjects;
	vector<Sound*>* enemyDyingSounds;
	vector<Enemy*>* enemies;
	Player* player = nullptr;
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

	virtual string name() const = 0;

	void CreateEnemy(Shader& shader_animation, Texture& textures_walk, Texture& textures_die, Graph mapPath, vector<Sound*>* enemyDyingSounds, vector<GameObject*>& gameObjects, vector<Enemy*>* enemies, Player& player);

	void Draw(float viewXY[2]);

	void decreaseLife(float value);

	void decreaseSpeed(float value);

	void increaseSpeed(float value);

};
