#include "Object2D_Interface.h"

void Object2D_Interface::CreateInterfaceObject2D(Shader *shader, Texture textures)
{
	vertices.LoadVertices();
	this->shader = shader;
	this->textures = textures;
}

void Object2D_Interface::DrawInterface(float sizeXY[2], float positionXYZ[3], float viewXY[2])
{

	*this->sizeXY = sizeXY;
	*this->positionXYZ = positionXYZ;
	*this->viewXY = viewXY;

	Transform transform(uv, model, projection, view);
	transform.ResetToIdentity();

	transform.Translate(positionXYZ[0], positionXYZ[1], positionXYZ[2], &model);
	transform.Scale(sizeXY[0], sizeXY[1], &model);
	transform.SetView(viewXY[0], viewXY[1], -1.0f, &view);

	transform.ApplyTransform(*shader, "view", view);
	transform.ApplyTransform(*shader, "uv", uv);
	transform.ApplyTransform(*shader, "projection", projection);
	transform.ApplyTransform(*shader, "model", model);

	for (size_t i = 0; i < textures.texturesPaths.size(); i++)
	{
		textures.ActivateTexture(i, &textures.textureID[i]);
	}

	glBindVertexArray(*vertices.GetVao());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}