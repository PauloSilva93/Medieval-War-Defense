#pragma once
#include "GLAD/glad.h"
#include "Shader.h"
#include "stb_image.h"
#include <vector>

class Texture
{

public:
	vector<const char*> texturesPaths;
	vector<GLuint> textureID;
	
	void LoadTexture(GLuint *textureID, const char* texturePath, bool transparent);

	template < typename T, size_t N>
	void LoadTextures(T(&pathsArray)[N])
	{
		for (int i = 0; i < N; i++)
		{
			texturesPaths.push_back(pathsArray[i]);
			textureID.push_back(i);
			LoadTexture(&textureID[i], pathsArray[i], true);
		}

	}

	void AssignTextureUnits(Shader shader, const char *name, int unit);

	void ActivateTexture(int textureUnit, GLuint *textureID);
};