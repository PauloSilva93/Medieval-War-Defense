#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include <map>
#include "SDLWindow.h"
#include "MathUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Text
{
public:
	std::map<GLchar, Character> Characters;
	GLuint VAO = 0;
	GLuint VBO = 0;
	SDLWindow *window = nullptr;
	MathUtils mu = {};
	Shader *shader_Text = nullptr;
	void CreateText(Shader *shader_Text, SDLWindow *window, const char* ttfPath);

	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};
