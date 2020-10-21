#pragma once
#include "TimeStep.h"
#include "Vertices.h"
#include "Texture.h"
#include "Transform.h"

class Object2D
{

public:
	Vertices vertices;
	Texture textures;
	Shader *shader;
	TimeStep time;
	float *sizeXY[2];
	float *positionXYZ[3];
	float *viewXY[2];

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 uv = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);

};
