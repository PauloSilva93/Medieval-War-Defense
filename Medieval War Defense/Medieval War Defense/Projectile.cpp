#include "Projectile.h"

	void Projectile::CreateProjectile(Shader& shader_animation, Texture& projectileTex)
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

	void Projectile::Draw(float viewXY[2])
	{
		projectile.DrawAnimated(size, position, viewXY, rotation, degrees, color, isInvertedXTexture, fps, loop);
	}
