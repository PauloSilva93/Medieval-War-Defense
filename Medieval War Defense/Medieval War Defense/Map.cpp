#include "Map.h"

	void Map::CreateMap(Shader& shader, Texture& textures)
	{
		map.CreateStaticObject2D(&shader, textures);;
	}
	void Map::DrawMapObject(float view[2])
	{
		map.DrawStatic(size, position, view, rotation, degrees, color);
	}