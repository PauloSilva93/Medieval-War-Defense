#include "Sorter.h"

void Sorter::QuickSort(std::vector<GameObject*> &gameObject, int left, int right)
{
	int i = left, j = right;
	float pivot = gameObject[(left + right) / 2]->position[2];

	while (i <= j)
	{
		while (gameObject[i]->position[2] < pivot)
			i++;
		while (gameObject[j]->position[2] > pivot)
			j--;
		if (i <= j)
		{
			std::swap(gameObject[i], gameObject[j]);
			i++;
			j--;
		}
	}
	if (left < j)
		QuickSort(gameObject, left, j);
	if (i < right)
		QuickSort(gameObject, i, right);
}
