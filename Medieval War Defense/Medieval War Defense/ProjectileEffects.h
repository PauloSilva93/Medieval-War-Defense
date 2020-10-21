#pragma once
#include "GameObject.h"
#include "MathUtils.h"
#include "ParticleEmitter.h"


class ProjectileEffects : public GameObject
{
public:
	MathUtils mu;
	ParticleEmitter emitter;
	vector<Particle>particleList;
	float pointList[2][2] = { {-1.0f, -1.0f}, {1.0f, -1.0f} };
	float size[2] = { 0.02f , 0.02f };
	float lifeTime = 10.0f;
	int totalParticles = 100;
	float emissionTime = -1;
	float changeSizeXY[2] = { 0.1f, 0.1f };
	float changeSizeVel = 0.0f;
	float randVelX[2] = { -0.1f, 0.1f };
	float randVelY[2] = { 0.1f, 0.1f };
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float changeColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float changeColorVel[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float degrees = 0.0f;
	int fps = 1;
	float emissionDelay = 0.0f;

	virtual void CreateParticleEffects(Object2D_Animated particle) {}
	virtual void UpdateParticleEffects() {}
};