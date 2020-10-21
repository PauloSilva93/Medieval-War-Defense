#include "Object2D_Animated.h"

void Object2D_Animated::CreateAnimatedObject2D(Shader *shader, Texture textures, const int frames_X, const int frames_Y, const int blankFrames, float initialFrame)
{
	vertices.LoadVertices();
	this->shader = shader;
	this->textures = textures;
	this->frames_X = frames_X;
	this->frames_Y = frames_Y;
	this->blankFrames = blankFrames;
	this->initialFrame = initialFrame;

	frameSize_X = 1.0f / frames_X;
	frameSize_Y = 1.0f / frames_Y;
	totalFrames = frames_X * frames_Y - blankFrames;

	currentFrame = initialFrame;
	animationFinished = false;
}

void Object2D_Animated::DrawAnimated(float sizeXY[2], float positionXYZ[3], float viewXY[2], float rotationXYZ[3], float degrees, float color[4], bool invertedXTexture, int fps, bool loop)
{

	*this->sizeXY = sizeXY;
	*this->positionXYZ = positionXYZ;
	*this->viewXY = viewXY;
	*this->rotationXYZ = rotationXYZ;
	this->degrees = degrees;
	*this->color = color;
	this->invertedXTexture = invertedXTexture;
	this->fps = fps;
	this->loop = loop;

	shader->use();
	shader->setVec4("color", color[0], color[1], color[2], color[3]);

	timeStep = time.step(fps);

	int frameCounter = 0;
	const int xy = 2;

	float **frames = new float*[totalFrames];
	for (int i = 0; i < totalFrames; ++i)
	{
		frames[i] = new float[xy];
	}
	if (currentFrame == (totalFrames - 1))
	{
		animationFinished = true;
		if (loop) currentFrame = 0.0f;
		else currentFrame = (totalFrames - 1);
	}
	else if (currentFrame + timeStep < (totalFrames - 1)) {
		if (timeStep > 0.0f) // Quando o ESC é pressionado timeStep recebe valores negativos, ocasionando erros
			currentFrame += timeStep;
	}
	else if (currentFrame + timeStep >= (totalFrames - 1)) {
		currentFrame = (totalFrames - 1);
	}

	for (int j = 0; j < frames_Y; j++)
	{
		int i;

		if (!invertedXTexture) i = 0;
		else i = frames_X - 1;

		bool condition = true;

		for (; condition; )
		{

			if (frameCounter < totalFrames)
			{
				frames[frameCounter][0] = float(i);
				frames[frameCounter][1] = (frames_Y - 1.0f) - float(j);

				frameCounter++;
			}

			if (!invertedXTexture) i++;
			else i--;
			if (!invertedXTexture && !(i < frames_X))
			{
				condition = false;
			}

			if (invertedXTexture && !(i > -1))
			{
				condition = false;
			}

		}
	}

	Transform transform(uv, model, projection, view);
	transform.ResetToIdentity();
	if (!invertedXTexture)
	{
		transform.Scale(frameSize_X, frameSize_Y, &uv);
		shader->use();
		shader->setBool("isInverted", false);
	}

	else {
		transform.Scale(-frameSize_X, frameSize_Y, &uv);
		shader->use();
		shader->setBool("isInverted", true);
	}

	transform.Translate(frames[(int)currentFrame][0], frames[(int)currentFrame][1], 0.0, &uv);

	transform.Translate(positionXYZ[0], positionXYZ[1], positionXYZ[2], &model);
	transform.Rotate(degrees, rotationXYZ, &model);
	transform.Scale(sizeXY[0], sizeXY[1], &model);
	transform.SetView(viewXY[0], viewXY[1], -1.2f, &view);

	transform.ApplyTransform(*shader, "uv", uv);
	transform.ApplyTransform(*shader, "view", view);
	transform.ApplyTransform(*shader, "model", model);
	transform.ApplyTransform(*shader, "projection", projection);

	for (size_t i = 0; i < textures.texturesPaths.size(); i++)
	{
		textures.ActivateTexture(i, &textures.textureID[i]);
	}

	glBindVertexArray(*vertices.GetVao());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	for (int i = 0; i < totalFrames; ++i) {
		delete[] frames[i];
	}
	delete[] frames;
}