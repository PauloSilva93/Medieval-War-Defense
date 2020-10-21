#include "Texture.h"

void Texture::LoadTexture(GLuint *textureID, const char* texturePath, bool transparent)
{
	static GLint width, height, nrChannels;
	static GLubyte *data;

	glGenTextures(1, textureID); // que são geradas da mesma forma

	glBindTexture(GL_TEXTURE_2D, *textureID); // Todas as operações com target "GL_TEXTURE_2D" serão atribuídas a "texture"
	// Define os parâmetros de mapeamento da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// Método de mapeamento padrão no eixo x
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// Método de mapeamento padrão no eixo y
	// Define os parâmetros de filtro da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carrega a imagem
	stbi_set_flip_vertically_on_load(true); //Vira a imagem ao contrário
	//Carrega a imagem usando 3 ints para armazenar largura, altura e canais de cores respectivamente
	data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		// Cria a textura. É importante que largura e altura correspondam aos argumentos passados em stbi_load()
		if (transparent)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D); //Gera os mipmaps
	}
	else
	{
		cout << "Fail to load texture: " << string(texturePath) << endl;
	}
	stbi_image_free(data); //Libera os dados de imagem carregados, já não precisando mais deles
}

void Texture::AssignTextureUnits(Shader shader, const char *name, int unit)
{
	shader.use();
	shader.setInt(name, unit);
}

void Texture::ActivateTexture(int textureUnit, GLuint *textureID)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, *textureID);
}
