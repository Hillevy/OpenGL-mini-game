#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "load.h"

class TextureManager
{
	private:

	public:
		TextureManager();

		Texture* currentBindTexture = NULL;

		void bindTexture(Texture* texture);
		void beginTexturing();
		void endTexturing();
};
#endif

