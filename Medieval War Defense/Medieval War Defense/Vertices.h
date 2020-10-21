#pragma once
#include <GLAD/glad.h>

class Vertices
{
private:
	GLuint vao, vbo, ebo;

public:
	void LoadVertices();

	GLuint* GetVao();

	GLuint* GetVbo();

	GLuint* GetEbo();

};
