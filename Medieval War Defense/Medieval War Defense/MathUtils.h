#pragma once
#include "SDLWindow.h"
#include <cmath>
#include <vector>
#include <array>

using namespace std;

class MathUtils
{
private:
	float coordXY[2];
public:
	
	float* PolarToCartesianCoords(float degrees);

	float* ScreenToVertexCoords(SDLWindow window, int screenCoords[2]);

	float* ScreenToTextureCoords(SDLWindow window, int screenCoords[2]);

	float* TextureToVertexCoords(float textureXY[2]);

	float* VertexToTextureCoords(float vertexXY[2]);

	float* VertexToScreenCoords(SDLWindow window, float vertexCoords[2]);

	float GenRandFloat(float minFloat, float maxFloat);

	float CompDistance(float vec1[3], float vec2[3]);

	float CompDistance(std::array<float, 3> vec1, float vec2[3]);

	bool chkCollisionPoint_Rect(float pointXY[2], float rectPosXY[2], float rectSizeXY[2]);

	bool chkCollisionPoint_Ellipse(float pointXY[2], float ellipsePosXY[2], float ellipseSizeXY[2]);

	template < typename T>
	T ChooseRandom(vector<T> objectList)
	{
		MathUtils mu;
		int n = mu.GenRandFloat(0.0, 1.0) * objectList.size();
		return objectList.at(n);
	}
};
