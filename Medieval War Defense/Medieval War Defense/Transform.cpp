#include "Transform.h"

Transform::Transform(glm::mat4 &uv, glm::mat4 &model, glm::mat4 &projection, glm::mat4 &view)
{
	this->uv = &uv;
	this->model = &model;
	this->projection = &projection;
	this->view = &view;
}

void Transform::ResetToIdentity()
{
	*uv = glm::mat4(1.0f);
	*model = glm::mat4(1.0f);
	*projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.5f, 1.5f);
	*view = glm::mat4(1.0f);
}

void Transform::ApplyTransform(Shader shader, const char* matrixName, glm::mat4 matrix)
{
	shader.use();
	shader.setMat4(matrixName, matrix);
}

void Transform::Scale(float sizeX, float sizeY, glm::mat4 *matrix)
{
	*matrix = glm::scale(*matrix, glm::vec3(sizeX, sizeY, 1.0f));
}

void Transform::Translate(float XAxis, float YAxis, float ZAxis, glm::mat4 *matrix)
{
	*matrix = glm::translate(*matrix, glm::vec3(XAxis, YAxis, ZAxis));
}

void Transform::Rotate(float degreeAngle, float axis[3], glm::mat4 *matrix)
{
	*matrix = glm::rotate(*matrix, glm::radians(degreeAngle), glm::vec3(axis[0], axis[1], axis[2]));
}

void Transform::ScaleToBottonLeft(float sizeX, float sizeY, glm::mat4 *matrix)
{
	*matrix = glm::scale(*matrix, glm::vec3(sizeX, sizeY, 1.0f));
	float finalSizeX = (1.0f / sizeX) - sizeX * (1.0f / sizeX);
	float finalSizeY = (1.0f / sizeY) - sizeY * (1.0f / sizeY);
	// Garante a mudança da origem para o vértice inferior esquerdo.
	*matrix = glm::translate(*matrix, glm::vec3(-finalSizeX, -finalSizeY, 0.0));
	currentTransformSizeX = finalSizeX;
	currentTransformSizeY = finalSizeY;
}

void Transform::TranslateFromBottonLeft(float XAxis, float YAxis, float ZAxis, glm::mat4 *matrix)
{
	// Garante o intervalo de translação [0, 1]
	float finalTranslationX = XAxis * currentTransformSizeX * 2.0f;
	float finalTranslationY = YAxis * currentTransformSizeY * 2.0f;
	*matrix = glm::translate(*matrix, glm::vec3(finalTranslationX, finalTranslationY, ZAxis));
}

void Transform::SetView(float viewX, float viewY, float viewZ, glm::mat4 *matrix)
{
	float viewVecData[] = { viewX, viewY, viewZ };

	glm::vec3 viewVec = glm::make_vec3(viewVecData);

	*matrix = glm::translate(*matrix, viewVec);
}