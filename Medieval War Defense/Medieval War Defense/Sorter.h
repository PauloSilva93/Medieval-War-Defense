#pragma once
#include <vector>
#include "GameObject.h"

class Sorter
{
public:
	void QuickSort(std::vector<GameObject*> &gameObject, int left, int right);
};