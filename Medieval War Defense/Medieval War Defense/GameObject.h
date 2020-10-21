#pragma once

class GameObject
{

public:
	static int objID;
	int internalObjID;
	float position[3];
	float rotation[3];
	float degrees;
	float color[4];
	bool exists;
	bool outward;

	GameObject();

	virtual void Draw(float viewXY[2]) {}

	void SetPosition(float position[3]);

};