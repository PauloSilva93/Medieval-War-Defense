#pragma once

#include "GameObject.h"
#include "Object2D_Static.h"

class Map : public GameObject
{
public:
	Object2D_Static map;
	float size[2] = { 1.0f, 1.0f };

	void CreateMap(Shader& shader, Texture& textures);

	void DrawMapObject(float view[2]);
};
