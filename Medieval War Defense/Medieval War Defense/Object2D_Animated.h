#pragma once
#include "Object2D.h"
#include "TimeStep.h"
#include "Vertices.h"
#include "Texture.h"
#include "Transform.h"

class Object2D_Animated : public Object2D
{

public:
	float *rotationXYZ[3];
	float *color[4];
	float degrees;
	bool invertedXTexture;
	int frames_X;
	int frames_Y;
	int blankFrames;
	int totalFrames;
	float frameSize_X;
	float frameSize_Y;
	float initialFrame;
	float timeStep;
	float currentFrame;
	float fps;
	bool loop;
	bool animationFinished;

	void CreateAnimatedObject2D(Shader *shader, Texture textures, const int frames_X, const int frames_Y, const int blankFrames, float initialFrame);
	void DrawAnimated(float sizeXY[2], float positionXYZ[3], float viewXY[2], float rotationXYZ[3], float degrees, float color[4], bool invertedXTexture, int fps, bool loop);
};