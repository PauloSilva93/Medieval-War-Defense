#pragma once
#include "TimeStep.h"
#include "Object2D_Animated.h"

class Particle
{
public:
	TimeStep time;
	float positionXYZ[3];
	float sizeXY[2];
	float velocity[2];
	float acceleration[2] = { 0.0f ,0.0f };
	float viewXY[2];
	float lifeTime;
	float color[4];
	float rotationXYZ[3] = { 0.0f, 0.0f, 1.0f };
	float degrees;
	bool dead;
	float renderTime;
	float originalSize[2];
	float originalColor[4];
	float originalDegrees;
	float livingTime = 0.0f;

	Particle(float positionXYZ[3], float sizeXY[2], float velXY[2], float degrees, float lifeTime, float color[4]);

	void Advance();

	void ChangeSize(float changeSizeXY[2], float changeVelocity);

	void ChangeColor(float changeColorRGBA[4], float changeVelocityRGBA[4]);

	void Rotate(float degrees);

	void AddVelocity(float xAccl, float yAccl);

	bool IsDead();

	void Draw(Object2D_Animated *animatedObject, int fps);

	float* getPositionXYZ();

};