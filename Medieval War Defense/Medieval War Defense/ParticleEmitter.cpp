#include "ParticleEmitter.h"


void ParticleEmitter::SetParticleList(vector<Particle>particleList)
{
	this->particleList = particleList;
}

void ParticleEmitter::SetParticleProperties(Object2D_Animated &animatedObject, float size[2], float degrees, float lifeTime, int totalParticles, float color[4], int fps)
{
	this->animatedObject = animatedObject;
	this->size[0] = size[0];
	this->size[1] = size[1];
	this->degrees = degrees;
	this->lifeTime = lifeTime;
	this->totalParticles = totalParticles;
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
	this->color[3] = color[3];
	this->fps = fps;	
}

void ParticleEmitter::Emit()
{
	
	if (this->firstRun)
	{
		this->originalEmissionTime = emissionTime;
		this->counter = 0;
		this->firstRun = false;
	}
	else if (!endlessEmission && emissionTime > 0.0f)
	{
		this->counter += time.step(1);
		this->emissionTime = this->originalEmissionTime - this->counter;
	}
	else if (emissionTime > 0.0f) this->counter += time.step(1);

	if (emissionTime < 0.0f) emissionTime = 0.0f;


	if (this->particleList.size() <= size_t(totalParticles) && this->emissionTime > 0.0f && counter >= emissionRate)
	{
		emissionRate = counter + emissionDelay;

		if (randBtw2Pts)
		{
			positionXYZ[0] = { mu.GenRandFloat(twoPointsList[0][0], twoPointsList[1][0]) };
			positionXYZ[1] = { mu.GenRandFloat(twoPointsList[0][1], twoPointsList[1][1]) };
		}
		else if (randBtwList)
		{
			std::array <float, 2> point = mu.ChooseRandom(pointsList);
			positionXYZ[0] = point[0];
			positionXYZ[1] = point[1];
		}
		this->particleList.push_back(Particle(positionXYZ, size, velocityXY, degrees, lifeTime, color));
	}

	for (int i = this->particleList.size() - 1; i > 0; --i)
	{
		this->currentParticle = &particleList.at(i);
		this->currentParticle->viewXY[0] = viewXY[0];
		this->currentParticle->viewXY[1] = viewXY[1];
		if (this->currentParticle->IsDead())
		{
			this->particleList.erase(this->particleList.begin() + i);
		}
		this->currentParticle->Advance();
	}

	for (size_t i = 1; i < this->particleList.size(); i++)
	{
		this->particleList[i].Draw(&animatedObject, fps);
	}
}

void ParticleEmitter::ChangeSize(float newSizeXY[2], float changeSizeVel)
{
	this->newSizeXY[0] = newSizeXY[0];
	this->newSizeXY[1] = newSizeXY[1];
	this->changeSizeVel = changeSizeVel;
	for (int i = this->particleList.size() - 1; i > 0; --i)
	{
		this->currentParticle = &particleList.at(i);
		this->currentParticle->ChangeSize(this->newSizeXY, this->changeSizeVel);
		this->currentParticle->Advance();
	}
}

void ParticleEmitter::ChangeColor(float changeColorRGBA[4], float changeVelocityRGBA[4])
{
	this->changeColorRGBA[0] = changeColorRGBA[0];
	this->changeColorRGBA[1] = changeColorRGBA[1];
	this->changeColorRGBA[2] = changeColorRGBA[2];
	this->changeColorRGBA[3] = changeColorRGBA[3];
	this->changeVelocityRGBA[0] = changeVelocityRGBA[0];
	this->changeVelocityRGBA[1] = changeVelocityRGBA[1];
	this->changeVelocityRGBA[2] = changeVelocityRGBA[2];
	this->changeVelocityRGBA[3] = changeVelocityRGBA[3];

	for (int i = this->particleList.size() - 1; i > 0; --i)
	{
		this->currentParticle = &particleList.at(i);
		this->currentParticle->ChangeColor(this->changeColorRGBA, this->changeVelocityRGBA);
		this->currentParticle->Advance();
	}
}

void ParticleEmitter::Rotate(float degrees)
{
	this->degrees = degrees;
	for (int i = this->particleList.size() - 1; i > 0; --i)
	{
		this->currentParticle = &particleList.at(i);
		this->currentParticle->Rotate(this->degrees);
		this->currentParticle->Advance();
	}
}

void ParticleEmitter::AddVelocity(float accelerationXY[2])
{
	this->accelerationXY[0] = accelerationXY[0];
	this->accelerationXY[1] = accelerationXY[1];

	for (int i = particleList.size() - 1; i > 0; i--)
	{
		this->currentParticle = &particleList.at(i);
		this->currentParticle->AddVelocity(this->accelerationXY[0], this->accelerationXY[1]);
		this->currentParticle->Advance();
	}
}

void ParticleEmitter::RandPosBtw2Pts(float twoPointsList[2][2])
{
	this->twoPointsList[0][0] = twoPointsList[0][0];
	this->twoPointsList[0][1] = twoPointsList[0][1];
	this->twoPointsList[1][0] = twoPointsList[1][0];
	this->twoPointsList[1][1] = twoPointsList[1][1];
	this->randBtw2Pts = true;
	this->randBtwList = false;
}

void ParticleEmitter::RandBtwList(vector<std::array<float, 2>> pointsList)
{
	this->pointsList = pointsList;
	this->randBtwList = true;
	this->randBtw2Pts = false;
}

void ParticleEmitter::SetEmissionDelay(float secondsDelay)
{
	this->emissionDelay = secondsDelay;
}

void ParticleEmitter::SetEmissionTime(float emissionTime)
{
	if (emissionTime < 0.0f) endlessEmission = true;
	else this->emissionTime = emissionTime;
}

void ParticleEmitter::SetVelocity(float velocityXY[2])
{
	this->velocityXY[0] = velocityXY[0];
	this->velocityXY[1] = velocityXY[1];
}

void ParticleEmitter::SetViewXY(float viewXY[2])
{
	this->viewXY[0] = viewXY[0];
	this->viewXY[1] = viewXY[1];
}

void ParticleEmitter::SetPosition(float positionXYZ[3])
{
	this->positionXYZ[0] = positionXYZ[0];
	this->positionXYZ[1] = positionXYZ[1];
	this->positionXYZ[2] = positionXYZ[2];
}

float* ParticleEmitter::GetParticlePos()
{
	for (int i = particleList.size() - 1; i > 0; i--)
	{
		this->currentParticle = &particleList.at(i);
		this->particlePos[0] = this->currentParticle->getPositionXYZ()[0];
		this->particlePos[1] = this->currentParticle->getPositionXYZ()[1];
		this->particlePos[2] = this->currentParticle->getPositionXYZ()[2];
		return this->particlePos;
	}
	return nullptr;
}
