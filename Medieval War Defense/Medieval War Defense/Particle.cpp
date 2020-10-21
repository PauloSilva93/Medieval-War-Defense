#include "Particle.h"

Particle::Particle(float positionXYZ[3], float sizeXY[2], float velXY[2], float degrees, float lifeTime, float color[4])
{
	this->positionXYZ[0] = positionXYZ[0];
	this->positionXYZ[1] = positionXYZ[1];
	this->positionXYZ[2] = positionXYZ[2];
	this->sizeXY[0] = sizeXY[0];
	this->sizeXY[1] = sizeXY[1];
	this->velocity[0] = velXY[0];
	this->velocity[1] = velXY[1];
	this->degrees = degrees;
	this->lifeTime = lifeTime;
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
	this->color[3] = color[3];
	this->originalSize[0] = sizeXY[0];
	this->originalSize[1] = sizeXY[1];
	this->originalColor[0] = color[0];
	this->originalColor[1] = color[1];
	this->originalColor[2] = color[2];
	this->originalColor[3] = color[3];
	this->originalDegrees = degrees;
	this->dead = false;
}

void Particle::Advance()
{
	this->renderTime = this->time.step(1);
	this->livingTime += this->renderTime;
	this->velocity[0] += (this->acceleration[0] * renderTime);
	this->velocity[1] += (this->acceleration[1] * renderTime);
	this->positionXYZ[0] += this->velocity[0] * renderTime;
	this->positionXYZ[1] += this->velocity[1] * renderTime;

	if (this->positionXYZ[0] < (-1.0f - sizeXY[0]) || this->positionXYZ[0] > (1.0f + sizeXY[0])) this->dead = true;
	if (this->positionXYZ[1] < (-1.0f - sizeXY[1]) || this->positionXYZ[1] > (1.0f + sizeXY[1])) this->dead = true;

	if ((this->lifeTime - this->livingTime) <= 0)
	{
		this->dead = true;
	}
}

void Particle::ChangeSize(float changeSizeXY[2], float changeVelocity)
{
	if (this->sizeXY[0] < 0.0f) sizeXY[0] = 0.0f;
	if (this->sizeXY[1] < 0.0f) sizeXY[1] = 0.0f;
	if (this->sizeXY[0] > 10.0f) sizeXY[0] = 10.0f;
	if (this->sizeXY[1] > 10.0f) sizeXY[1] = 10.0f;

	float newSizeXY[2] = { abs(changeSizeXY[0] - this->originalSize[0]) , abs(changeSizeXY[1] - this->originalSize[1]) };

	for (int i = 0; i < 2; i++)
	{
		if (this->originalSize[i] < newSizeXY[i])
		{
			if ((this->originalSize[i] + newSizeXY[i] * livingTime * changeVelocity) >= changeSizeXY[i])
			{
				this->sizeXY[i] = changeSizeXY[i];
			}
			else
			{
				this->sizeXY[i] = this->originalSize[i] + newSizeXY[i] * this->livingTime * changeVelocity;
			}
		}
		else
		{
			if ((this->originalSize[i] - newSizeXY[i] * livingTime * changeVelocity) <= changeSizeXY[i])
			{
				this->sizeXY[i] = changeSizeXY[i];
			}
			else
			{
				this->sizeXY[i] = this->originalSize[i] - newSizeXY[i] * this->livingTime * changeVelocity;
			}
		}
	}
}

void Particle::ChangeColor(float changeColorRGBA[4], float changeVelocityRGBA[4])
{
	for (int i = 0; i < 4; i++)
	{
		float newColor = abs(changeColorRGBA[i] - originalColor[i]);
		if (this->originalColor[i] < changeColorRGBA[i])
		{
			if (this->originalColor[i] + newColor * this->livingTime * changeVelocityRGBA[i] >= changeColorRGBA[i])
			{
				this->color[i] = changeColorRGBA[i];
			}
			else
			{
				this->color[i] = this->originalColor[i] + newColor * this->livingTime * changeVelocityRGBA[i];
			}
		}
		else
		{
			if (this->originalColor[i] - newColor * this->livingTime * changeVelocityRGBA[i] <= changeColorRGBA[i])
			{
				this->color[i] = changeColorRGBA[i];
			}
			else
			{
				this->color[i] = this->originalColor[i] - newColor * this->livingTime * changeVelocityRGBA[i];
			}
		}
	}

}

void Particle::Rotate(float degrees)
{
	this->degrees = this->originalDegrees + (degrees * livingTime);
}

void Particle::AddVelocity(float xAccl, float yAccl)
{
	this->acceleration[0] = xAccl * livingTime;
	this->acceleration[1] = yAccl * livingTime;
}

bool Particle::IsDead() { return this->dead; }

void Particle::Draw(Object2D_Animated *animatedObject, int fps)
{
	bool invertedXTexture = 0;
	float initialFrame = 0;
	bool loop = true;
	animatedObject->DrawAnimated(sizeXY, positionXYZ, this->viewXY, this->rotationXYZ, this->degrees, this->color, invertedXTexture, fps, loop);
}

float* Particle::getPositionXYZ()
{
	return this->positionXYZ;
}
