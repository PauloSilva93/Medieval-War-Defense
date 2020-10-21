#include "PoisonEffects.h"


void PoisonEffects::CreateParticleEffects(Object2D_Animated particle)
{
	emitter.SetParticleList(particleList);
	emitter.SetParticleProperties(particle, size, degrees, lifeTime, totalParticles, color, fps);
	emitter.SetEmissionDelay(emissionDelay);
	emitter.SetEmissionTime(emissionTime);
}

void PoisonEffects::UpdateParticleEffects()
{
	float velocity[2] = { mu.GenRandFloat(-0.1f, 0.1f), mu.GenRandFloat(0.0f, 0.2f) };
	float accl[2] = { mu.GenRandFloat(-0.5f, 0.5f), mu.GenRandFloat(0.05f, 0.05f) };
	//emitter.SetVelocity(velocity);
	emitter.AddVelocity(accl);
	emitter.ChangeSize(changeSizeXY, changeSizeVel);
	emitter.ChangeColor(changeColor, changeColorVel);
	emitter.SetPosition(position);
}

void PoisonEffects::Draw(float viewXY[2])
{
	emitter.SetViewXY(viewXY);
	emitter.Emit();
}
