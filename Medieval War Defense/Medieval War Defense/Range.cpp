#include "Range.h"

	void Range::CreateRangeObject(Shader& shader, Texture& textures)
	{
		rangeObject.CreateStaticObject2D(&shader, textures);
	}

	void Range::Draw(float view[2])
	{
		if (visible) rangeObject.DrawStatic(size, position, view, rotation, degrees, color);
	}
