#include "Selection.h"

bool Selection::isSelected(float pointXY[2], float rectPosXY[2], float rectSizeXY[2], bool leftMouse)
{

	if (!selected && leftMouse && mu.chkCollisionPoint_Rect(pointXY, rectPosXY, rectSizeXY))
	{
		selected = true;
		return true;
	}
	if (selected && leftMouse)
	{
		selected = false;
		return false;
	}
	if (selected)
	{
		return true;
	}

	return false;
}

bool Selection::isUnderMouse(float pointXY[2], float rectPosXY[2], float rectSizeXY[2])
{
	if (mu.chkCollisionPoint_Rect(pointXY, rectPosXY, rectSizeXY)) return true;
	else return false;
}
