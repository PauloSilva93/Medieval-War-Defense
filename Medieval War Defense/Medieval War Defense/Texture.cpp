#include "Texture.h"

void Texture::LoadTexture(GLuint *textureID, const char* texturePath, bool transparent)
{
	static GLint width, height, nrChannels;
	static GLubyte *data;

	glGenTextures(1, textureID); // que s�o geradas da mesma forma

	glBindTexture(GL_TEXTURE_2D, *textureID); // Todas as opera��es com target "GL_TEXTURE_2D" ser�o atribu�das a "texture"
	// Define os par�metros de mapeamento da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// M�todo de mapeamento padr�o no eixo x
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// M�todo de mapeamento padr�o no eixo y
	// Define os par�metros de filtro da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carrega a imagem
	stbi_set_flip_vertically_on_load(true); //Vira a imagem ao contr�rio
	//Carrega a imagem usando 3 ints para armazenar largura, altura e canais de cores respectivamente
	data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		// Cria a textura. � importante que largura e altura correspondam aos argumentos passados em stbi_load()
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
	stbi_image_free(data); //Libera os dados de imagem carregados, j� n�o precisando mais deles
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
