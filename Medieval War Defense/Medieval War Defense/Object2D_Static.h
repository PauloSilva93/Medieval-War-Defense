#pragma once
#include "Object2D.h"
#include "TimeStep.h"
#include "Vertices.h"
#include "Texture.h"
#include "Transform.h"

class Object2D_Static : public Object2D
{

public:
	float *rotationXYZ[3];
	float *color[4];
	float degrees;
	int numTextures;

	void CreateStaticObject2D(Shader *shader, Texture textures);
	void DrawStatic(float sizeXY[2], float positionXYZ[3], float viewXY[2], float rotationXYZ[3], float degrees, float color[4]);
};