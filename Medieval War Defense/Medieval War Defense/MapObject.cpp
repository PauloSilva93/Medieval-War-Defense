#include "MapObject.h"

	void MapObject::CreateMapObject(Shader& shader, Texture& textures, vector<GameObject*>& gameObjects)
	{
		mapObject.CreateStaticObject2D(&shader, textures);
		gameObjects.push_back(this);
	}

	void MapObject::Draw(float view[2])
	{
		mapObject.DrawStatic(size, position, view, rotation, degrees, color);
	}