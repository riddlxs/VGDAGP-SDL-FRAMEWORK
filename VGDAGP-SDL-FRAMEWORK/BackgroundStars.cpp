#include "BackgroundStars.h"

BackgroundStars* BackgroundStars::sInstance = nullptr;

BackgroundStars* BackgroundStars::Instance() {
	if (sInstance == nullptr) {
		sInstance = new BackgroundStars();
	}
	return sInstance;
}

void BackgroundStars::Release() {
	delete sInstance;
	sInstance = nullptr;
}

void BackgroundStars::Scroll(bool scroll) {
	Star::Scroll(scroll);

}