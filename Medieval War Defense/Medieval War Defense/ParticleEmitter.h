#pragma once
#include "Particle.h"
#include "MathUtils.h"
#include <vector>
#include <array>

class ParticleEmitter
{
public:
	Object2D_Animated animatedObject;
	float positionXYZ[3] = { 0.0f, 0.0f, 0.0f };
	float velocityXY[2] = { 0.0f, 0.0f };
	float size[2];
	float viewXY[2] = { 0.0f, 0.0f };
	float degrees;
	float lifeTime; 
	int totalParticles;
	float color[4];
	int fps;
	float newSizeXY[2];
	float changeSizeVel;
	float changeColorRGBA[4];
	float changeVelocityRGBA[4];
	float accelerationXY[2] = { 0.0f, 0.0f };

	Particle *currentParticle = nullptr;
	TimeStep time;
	float counter = 0.0f;
	float originalEmissionTime = 0.0f;
	bool firstRun = true;
	bool randBtw2Pts = false;
	bool randBtwList = false;
	float twoPointsList[2][2];
	vector<std::array<float, 2>> pointsList;
	vector<Particle>particleList;
	MathUtils mu;
	float emissionRate = 0.0f;
	float emissionDelay = 0.0f;
	float emissionTime = 10.0f;
	bool endlessEmission = false;
	float particlePos[3];

	void SetParticleList(vector<Particle>particleList);

	void SetParticleProperties(Object2D_Animated &animatedObject, float size[2], float degrees, float lifeTime, int totalParticles, float color[4], int fps);

	void Emit();

	void SetViewXY(float viewXY[2]);

	void ChangeSize(float newSizeXY[2], float changeSizeVel);

	void ChangeColor(float changeColorRGBA[4], float changeVelocityRGBA[4]);

	void Rotate(float degrees);

	void AddVelocity(float accelerationXY[2]);

	void RandPosBtw2Pts(float pointsList[2][2]);

	void RandBtwList(vector<std::array<float, 2>> pointsList);

	void SetEmissionDelay(float secondsDelay);

	void SetEmissionTime(float emissionTime);

	void SetVelocity(float velocityXY[2]);

	void SetPosition(float positionXYZ[3]);

	float* GetParticlePos();
};