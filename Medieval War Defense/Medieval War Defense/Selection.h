#pragma once
#include "MathUtils.h"

class Selection
{
public:
	MathUtils mu;
	bool selected = false;
	bool isOver = false;

	bool isSelected(float pointXY[2], float rectPosXY[2], float rectSizeXY[2], bool leftMouse);

	bool isUnderMouse(float pointXY[2], float rectPosXY[2], float rectSizeXY[2]);
};
