#include "GameObject.h"

GameObject::GameObject() : position{ 0.0f, 0.0f, 0.0f }, rotation{ 0.0f, 0.0f, 1.0f }, degrees(0.0f), color{ 0.0f, 0.0f, 0.0f, 1.0f }, exists(true), outward(false), internalObjID(objID++){}

void GameObject::SetPosition(float position[3])
{
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];
}

