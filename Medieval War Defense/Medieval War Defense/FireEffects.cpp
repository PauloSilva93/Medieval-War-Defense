#include "FireEffects.h"


void FireEffects::CreateParticleEffects(Object2D_Animated particle)
{
	emitter.SetParticleList(particleList);
	emitter.SetParticleProperties(particle, size, degrees, lifeTime, totalParticles, color, fps);
	emitter.SetEmissionDelay(emissionDelay);
	emitter.SetEmissionTime(emissionTime);
}

void FireEffects::UpdateParticleEffects()
{
	float velocity[2] = { mu.GenRandFloat(-0.05f, 0.05f), mu.GenRandFloat(0.0f, 0.1f) };
	float accl[2] = { mu.GenRandFloat(-0.25f, 0.25f), mu.GenRandFloat(0.025f, 0.025f) };
	emitter.SetVelocity(velocity);
	emitter.AddVelocity(accl);
	emitter.ChangeSize(changeSizeXY, changeSizeVel);
	emitter.ChangeColor(changeColor, changeColorVel);
	emitter.SetPosition(position);
}

void FireEffects::Draw(float viewXY[2])
{
	emitter.SetViewXY(viewXY);
	emitter.Emit();
}
