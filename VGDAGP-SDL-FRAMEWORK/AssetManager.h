#pragma once
#include "Graphics.h"
#include <map>

namespace SDLFramework {
	class AssetManager {
	public:
		static AssetManager* Instance();
		static void Release();

		SDL_Texture* GetTexture(std::string fileName, bool managed = false);

		void DestroyTexture(SDL_Texture* texture);

	private:
		AssetManager();
		~AssetManager();

		void UnloadTexture(SDL_Texture* texture);

		static AssetManager* sInstance;

		//Key			Value
		std::map<std::string, SDL_Texture*> mTextures;
		std::map<SDL_Texture*, unsigned> mTextureRefCount;
	};
}
