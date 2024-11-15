#pragma once
#pragma once
#include "GameEntity.h"
#include "AssetManager.h"

namespace SDLFramework {
	class Texture : public GameEntity {
	public:
		Texture(std::string filename, bool managed = false);
		Texture(std::string filename, int x, int y, int width, int height, bool managed = false);
		~Texture();

		Vector2 ScaledDimensions();

		void SetSourceRect(SDL_Rect* sourceRect);

		void Render() override;

	private:
		SDL_Texture* mTex;
		Graphics* mGraphics;

		int mWidth;
		int mHeight;

		bool mClipped;
		SDL_Rect mSourceRect;
		SDL_Rect mDestinationRect;
	};
}