#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Transform
{
private:
	glm::mat4 *uv;
	glm::mat4 *model;
	glm::mat4 *projection;
	glm::mat4 *view;

	float currentTransformSizeX = 0.0f;
	float currentTransformSizeY = 0.0f;
	float currentTranslateSizeX = 0.0f;
	float currentTranslateSizeY = 0.0f;

public:

	Transform(glm::mat4 &uv, glm::mat4 &model, glm::mat4 &projection, glm::mat4 &view);

	void ResetToIdentity();

	void ApplyTransform(Shader shader, const char* matrixName, glm::mat4 matrix);

	void Scale(float sizeX, float sizeY, glm::mat4 *matrix);

	void Translate(float XAxis, float YAxis, float ZAxis, glm::mat4 *matrix);

	void Rotate(float degreeAngle, float axis[3], glm::mat4 *matrix);

	void ScaleToBottonLeft(float sizeX, float sizeY, glm::mat4 *matrix);

	void TranslateFromBottonLeft(float XAxis, float YAxis, float ZAxis, glm::mat4 *matrix);

	void SetView(float viewX, float viewY, float viewZ, glm::mat4 *matrix);
};
