#include "MathUtils.h"

float* MathUtils::PolarToCartesianCoords(float degrees)
{
	float PI = 3.14159265359f;
	float arc = degrees * (2.0f * PI / 360.0f);
	coordXY[0] = cos(arc);
	coordXY[1] = sin(arc);
	return coordXY;
}

float* MathUtils::ScreenToVertexCoords(SDLWindow window, int screenCoords[2])
{
	coordXY[0] = float(screenCoords[0]) / float(window.getWindowWidth()) * 2.0f - 1.0f;
	coordXY[1] = (float(screenCoords[1]) / float(-window.getWindowHeight()) + 1.0f) * 2.0f - 1.0f;
	return coordXY;
}

float* MathUtils::VertexToScreenCoords(SDLWindow window, float vertexCoords[2])
{
	coordXY[0] = float(window.getWindowWidth()) / 2.0f + vertexCoords[0] * (float(window.getWindowWidth() / 2.0f));
	coordXY[1] = float(window.getWindowHeight()) / 2.0f + vertexCoords[1] * (float(window.getWindowHeight() / 2.0f));
	return coordXY;
}

float* MathUtils::ScreenToTextureCoords(SDLWindow window, int screenCoords[2])
{
	coordXY[0] = float(screenCoords[0]) / float(window.getWindowWidth());
	coordXY[1] = 1.0f - (float(screenCoords[1]) / float(window.getWindowHeight()));
	return coordXY;
}

float* MathUtils::TextureToVertexCoords(float textureXY[2])
{
	coordXY[0] = textureXY[0] * 2.0f - 1.0f;
	coordXY[1] = textureXY[1] * 2.0f - 1.0f;
	return coordXY;
}

float* MathUtils::VertexToTextureCoords(float vertexXY[2])
{
	coordXY[0] = vertexXY[0] / 2.0f + 0.5f;
	coordXY[1] = vertexXY[1] / 2.0f + 0.5f;
	return coordXY;
}

float MathUtils::GenRandFloat(float minFloat, float maxFloat)
{
	float min = minFloat;
	float diff = maxFloat - minFloat;
	float max = (diff == 0 ? 0 : rand() % (int(diff * 1000)) / 1000.0f);
	return min + max;
}

float MathUtils::CompDistance(float vec1[3], float vec2[3])
{
	float vecDist[3] = { vec1[0] - vec2[0], vec1[1] - vec2[1], vec1[2] - vec2[2] };
	float distance = sqrt(pow(vecDist[0], 2) + pow(vecDist[1], 2) + pow(vecDist[2], 2));
	return distance;
}

float MathUtils::CompDistance(std::array<float, 3> vec1, float vec2[3])
{
	float vecDist[3] = { vec1[0] - vec2[0], vec1[1] - vec2[1], vec1[2] - vec2[2] };
	float distance = sqrt(pow(vecDist[0], 2) + pow(vecDist[1], 2) + pow(vecDist[2], 2));
	return distance;
}

bool MathUtils::chkCollisionPoint_Rect(float pointXY[2], float rectPosXY[2], float rectSizeXY[2])
{
	float initX = rectPosXY[0] - rectSizeXY[0];
	float initY = rectPosXY[1] - rectSizeXY[1];
	float finalX = rectPosXY[0] + rectSizeXY[0];
	float finalY = rectPosXY[1] + rectSizeXY[1];

	if ((initX <= pointXY[0] && pointXY[0] <= finalX) && (initY <= pointXY[1] && pointXY[1] <= finalY)) return true;
	else return false;
}

bool MathUtils::chkCollisionPoint_Ellipse(float pointXY[2], float ellipsePosXY[2], float ellipseSizeXY[2])
{
	float x = pointXY[0], y = pointXY[1];
	float p0_x = ellipsePosXY[0] - ellipseSizeXY[0] ;
	float p1_x = ellipsePosXY[0] + ellipseSizeXY[0] ;
	float p0_y = ellipsePosXY[1] - ellipseSizeXY[1] ;
	float p1_y = ellipsePosXY[1] + ellipseSizeXY[1] ;

	float m_x = (1.0f / 2.0f) * (p0_x + p1_x);
	float m_y = (1.0f / 2.0f) * (p0_y + p1_y);
	float sigma_x = (1.0f / 2.0f) * abs(-p0_x + p1_x);
	float sigma_y = (1.0f / 2.0f) * abs(-p0_y + p1_y);

	if (((pow(x - m_x, 2)) / pow(sigma_x, 2)) + ((pow(y - m_y, 2)) / pow(sigma_y, 2)) < 1.0f)
		return true;
	return false;
}
