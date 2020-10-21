#include "Vertices.h"
#include <iostream>

void Vertices::LoadVertices()
{
	
	GLfloat verticesObj[] =
	{	 // posições             // coordenadas de textura
		 1.0f ,  1.0f  , 0.0f,       1.0f, 1.0f,   // topo direito
		 1.0f , -1.0f   , 0.0f,      1.0f, 0.0f,   // base direita
		-1.0f , -1.0f  , 0.0f,      0.0f, 0.0f,   // base esquerda
		-1.0f ,  1.0f   , 0.0f,      0.0f, 1.0f    // topo esquerdo
	};

	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesObj), verticesObj, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	// Configuração do atributo aPos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Configuração do atributo aTexCoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint* Vertices::GetVao()
{
	return &vao;
}

GLuint* Vertices::GetVbo()
{
	return &vbo;
}

GLuint* Vertices::GetEbo()
{
	return &ebo;
}