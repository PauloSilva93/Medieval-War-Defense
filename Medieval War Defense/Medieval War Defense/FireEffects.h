#pragma once
#include "ProjectileEffects.h"

class FireEffects : public ProjectileEffects
{
public:
	float lifeTime = 0.5f;
	int totalParticles = 500;
	float emissionTime = -1;
	float changeSizeXY[2] = { 0.0f, 0.0f };
	float changeSizeVel = 1.5f;
	float color[4] = { 1.0f, 0.8f, 0.2f, 1.0f };
	float changeColor[4] = { 1.0f, 0.2f, 0.0f, 0.0f };
	float changeColorVel[4] = { 2.5f, 2.5f, 2.5f, 1.0f };
	float degrees = 0.0f;
	int fps = 1;

	void CreateParticleEffects(Object2D_Animated particle);

	void UpdateParticleEffects();

	void Draw(float viewXY[2]);
};