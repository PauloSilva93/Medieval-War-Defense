#pragma once

#include "GameObject.h"
#include "Object2D_Static.h"

class MapObject : public GameObject
{
public:
	Object2D_Static mapObject;
	float size[2] = { 0.1f, 0.1f };

	void CreateMapObject(Shader& shader, Texture& textures, vector<GameObject*>& gameObjects);

	void Draw(float view[2]);
};
