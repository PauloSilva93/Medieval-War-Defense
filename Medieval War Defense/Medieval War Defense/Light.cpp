#include "Light.h"

Light::Light(Shader shader[], float positionXYZ[3], float diffuse_lightColor[3], float specular_lightColor[3], float ambient_lightColor[3], float shininess, float falloff[3])
{
	this->shader = shader;
	positionXYZ[0] = mu.VertexToTextureCoords(positionXYZ)[0];
	positionXYZ[1] = mu.VertexToTextureCoords(positionXYZ)[1];
	this->positionXYZ = positionXYZ;
	this->diffuse_lightColor = diffuse_lightColor;
	this->specular_lightColor = specular_lightColor;
	this->ambient_lightColor = ambient_lightColor;
	this->shininess = shininess;
	this->falloff = falloff;
}

void Light::EnableSpotLight(float degrees, float spotCutOff, float outerCutOff)
{
	this->spotLight = true;
	float light_Spot[3] = { mu.PolarToCartesianCoords(degrees)[0], mu.PolarToCartesianCoords(degrees)[1], 0.0 };

	this->light_Spot[0] = light_Spot[0];
	this->light_Spot[1] = light_Spot[1];
	this->light_Spot[2] = light_Spot[2];

	this->spotCutOff = spotCutOff;
	this->outerCutOff = outerCutOff;
}

void Light::EnableDirectionalLight()
{
	this->pointLight = false;
}
