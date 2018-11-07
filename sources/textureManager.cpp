#include "textureManager.h"
#include "load.h"

TextureManager::TextureManager() {
	currentBindTexture = new Texture();
}

void TextureManager::bindTexture(Texture* texture) {
	currentBindTexture = texture;
}

void TextureManager::beginTexturing() {

	glBindTexture(GL_TEXTURE_2D, currentBindTexture->OpenGL_ID[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on repete la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0

	if (currentBindTexture->isRGBA)
	{
		glEnable(GL_ALPHA_TEST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, currentBindTexture->img_color->lenx,
			currentBindTexture->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, currentBindTexture->img_all);
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, currentBindTexture->img_color->lenx,
			currentBindTexture->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, currentBindTexture->img_color->data);
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // indique qu'il faut melanger la texture avec la couleur courante

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);

	glAlphaFunc(GL_GREATER, 0.3f);
	glPolygonMode(GL_BACK, GL_FILL);

	glPushMatrix();
}

void TextureManager::endTexturing() {
	glDisable(GL_TEXTURE_2D);
	if (currentBindTexture->isRGBA)
	{
		glDisable(GL_ALPHA_TEST);
	}
	glPopMatrix();
}