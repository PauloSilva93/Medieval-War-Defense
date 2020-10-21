#pragma once
#include <vector>
#include "Light.h"

class LightSystem
{

public:
	vector<Light>lights;

	void AddLight(Light light);

	template < typename T, size_t N>
	void DrawLights(T(&shaderList)[N])
	{
		int totalShaders = sizeof(shaderList) / sizeof(*shaderList);
		for (int s = 0; s < totalShaders; s++)
		{
			for (size_t i = 0; i < this->lights.size(); i++)
			{
				lights[i].shader[s].use();
				lights[i].shader[s].setInt("totalLights", lights.size());
				lights[i].shader[s].setVec3("LightSource[" + std::to_string(i) + "].lightPos", lights[i].positionXYZ[0], lights[i].positionXYZ[1], lights[i].positionXYZ[2]);
				lights[i].shader[s].setVec4("LightSource[" + std::to_string(i) + "].diffuse_lightColor", lights[i].diffuse_lightColor[0], lights[i].diffuse_lightColor[1], lights[i].diffuse_lightColor[2], 1.0f);
				lights[i].shader[s].setVec4("LightSource[" + std::to_string(i) + "].specular_lightColor", lights[i].specular_lightColor[0], lights[i].specular_lightColor[1], lights[i].specular_lightColor[2], 1.0f);
				lights[i].shader[s].setVec4("LightSource[" + std::to_string(i) + "].ambient_lightColor", lights[i].ambient_lightColor[0], lights[i].ambient_lightColor[1], lights[i].ambient_lightColor[2], 1.0f);
				lights[i].shader[s].setFloat("LightSource[" + std::to_string(i) + "].shininess", lights[i].shininess);
				lights[i].shader[s].setVec3("LightSource[" + std::to_string(i) + "].falloff", lights[i].falloff[0], lights[i].falloff[1], lights[i].falloff[1]);
				lights[i].shader[s].setBool("LightSource[" + std::to_string(i) + "].pointLight", lights[i].pointLight);
				lights[i].shader[s].setBool("LightSource[" + std::to_string(i) + "].spotLight", lights[i].spotLight);
				lights[i].shader[s].setVec3("LightSource[" + std::to_string(i) + "].light_Spot", lights[i].light_Spot[0], lights[i].light_Spot[1], lights[i].light_Spot[2]);
				lights[i].shader[s].setFloat("LightSource[" + std::to_string(i) + "].spotCutOff", lights[i].spotCutOff);
				lights[i].shader[s].setFloat("LightSource[" + std::to_string(i) + "].outerCutOff", lights[i].outerCutOff);
			}
		}

	}
};
