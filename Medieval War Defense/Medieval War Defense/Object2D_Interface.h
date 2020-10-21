#pragma once
#include "Object2D.h"
#include "TimeStep.h"
#include "Vertices.h"
#include "Texture.h"
#include "Transform.h"

class Object2D_Interface : public Object2D
{
private:

public:
	void CreateInterfaceObject2D(Shader *shader, Texture textures);
	void DrawInterface(float sizeXY[2], float positionXYZ[3], float viewXY[2]);
};
