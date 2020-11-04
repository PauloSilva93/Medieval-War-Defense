#pragma once

#include "GameObject.h"
#include "Object2D_Animated.h"

class Projectile : public GameObject
{
public:
	Object2D_Animated projectile;
	int frames_X;
	int frames_Y;
	int blankFrames;
	float initialFrame;
	float size[2];
	bool isInvertedXTexture;
	int fps;
	bool loop;

	void CreateProjectile(Shader& shader_animation, Texture& projectileTex);

	void Draw(float viewXY[2]);
};
