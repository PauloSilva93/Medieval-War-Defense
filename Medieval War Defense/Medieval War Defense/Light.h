#pragma once
#include "Shader.h"
#include "MathUtils.h"

class Light
{
public:
	Shader *shader;
	MathUtils mu;
	float *positionXYZ;
	float *diffuse_lightColor;
	float *specular_lightColor;
	float *ambient_lightColor;
	float shininess;
	float *falloff;
	bool pointLight = true;
	bool spotLight = false;
	float light_Spot[3];
	float spotCutOff;
	float outerCutOff;

	Light(Shader shader[], float positionXYZ[3], float diffuse_lightColor[3], float specular_lightColor[3], float ambient_lightColor[3], float shininess, float falloff[3]);

	void EnableSpotLight(float degrees, float spotCutOff, float outerCutOff);

	void EnableDirectionalLight();
};
