#include "PlayScreen.h"

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mStars = BackgroundStars::Instance();

	mSideBar = new PlaySideBar();
	mSideBar->Parent(this);
	mSideBar->Position(Graphics::SCREEN_WIDTH * 0.87f, Graphics::SCREEN_HEIGHT * 0.05f);
}

PlayScreen::~PlayScreen() {
	mTimer = nullptr;
	mAudio = nullptr;

	mStars = nullptr;

	delete mSideBar;
	mSideBar = nullptr;
}

void PlayScreen::StartNewGame() {
	mSideBar->SetHighScore(645987);
	mSideBar->SetShips(2);
	//This is PURELY FOR QUICK TESTING
	StartNextLevel();
}

void PlayScreen::StartNextLevel() {
	mSideBar->SetLevel(75);
}

void PlayScreen::Update() {
	mSideBar->Update();
}

void PlayScreen::Render() {
	mSideBar->Render();
}