#pragma once

#include "GameObject.h"
#include "Object2D_Static.h"

class Range : public GameObject
{
public:
	Object2D_Static rangeObject;
	float size[2] = { 0.1f, 0.1f };
	bool visible = true;

	void CreateRangeObject(Shader& shader, Texture& textures);

	void Draw(float view[2]);
};