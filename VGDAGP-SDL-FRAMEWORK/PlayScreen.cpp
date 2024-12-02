#include "PlayScreen.h"

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mStars = BackgroundStars::Instance();

	mSideBar = new PlaySideBar();
	mSideBar->Parent(this);
	mSideBar->Position(Graphics::SCREEN_WIDTH * 0.87f, Graphics::SCREEN_HEIGHT * 0.05f);

	mStartLabel = new Texture("START", "emulogic.ttf", 32, { 150, 0, 0 });
	mStartLabel->Parent(this);
	mStartLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);

	mLevel = nullptr;
	mLevelStartDelay = 1.0f;
	mLevelStarted = false;

	mPlayer = nullptr;
	mPlayer2 = nullptr;
}

PlayScreen::~PlayScreen() {
	mTimer = nullptr;
	mAudio = nullptr;

	mStars = nullptr;

	delete mSideBar;
	mSideBar = nullptr;

	delete mStartLabel;
	mStartLabel = nullptr;

	delete mLevel;
	mLevel = nullptr;

	delete mPlayer;
	mPlayer = nullptr;
	delete mPlayer2; // Clean up Player 2 as well as player one!
	mPlayer2 = nullptr;
}


void PlayScreen::StartNewGame() {
	delete mPlayer;
	mPlayer = new Player(true);
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.8f);
	mPlayer->Active(true);

	// Initialize Player 2
	delete mPlayer2;
	mPlayer2 = new Player(false);
	mPlayer2->Parent(this);
	mPlayer2->Position(Graphics::SCREEN_WIDTH * 0.6f, Graphics::SCREEN_HEIGHT * 0.8f); // Player 2 should be on the right side
	mPlayer2->Active(true);

	mSideBar->SetHighScore(645987);
	mSideBar->SetShips(mPlayer->Lives());
	mSideBar->SetPlayerScore(mPlayer->Score());
	mSideBar->SetLevel(0);

	mStars->Scroll(false);
	mGameStarted = false;
	mLevelStarted = false;
	mLevelStartTimer = 0.0f;
	mCurrentStage = 0;

	//mAudio->PlayMusic("MUS/GameStart.wav", 0);
}



void PlayScreen::StartNextLevel() {
	mCurrentStage++;
	mLevelStartTimer = 0.0f;
	mLevelStarted = true;

	delete mLevel;
	mLevel = new Level(mCurrentStage, mSideBar, mPlayer, mPlayer2);  // Passing both players
}


bool PlayScreen::GameOver() {
	//This is essentially an if statement as a return on a single line
	//If mLevelStarted == false, return false
	//OTHERWISE, we return true/false if the state == GameOver
	return !mLevelStarted ? false : (mLevel->State() == Level::GameOver);
}

void PlayScreen::Update() {
	if (mGameStarted) {
		if (!mLevelStarted) {
			mLevelStartTimer += mTimer->DeltaTime();
			if (mLevelStartTimer >= mLevelStartDelay) {
				StartNextLevel();
			}
		}
		else {
			// The level has started or is in session
			mLevel->Update();

			if (mLevel->State() == Level::Finished) {
				mLevelStarted = false;
			}
		}

		// This is us saying that we are in a level of some kind
		if (mCurrentStage > 0) {
			mSideBar->Update();
		}

		mPlayer->HandleMovementForPlayer1();  // Update Player 1's movement (WASD)
		mPlayer->Update();

		if (mPlayer2) {
			mPlayer2->HandleMovementForPlayer2();  // Update Player 2's movement (Arrow keys)
			mPlayer2->Update();
		}
	}
	else {
		if (!Mix_PlayingMusic()) {
			// We have finished playing the intro music from StartNewGame
			mGameStarted = true;
		}
	}
}



void PlayScreen::Render() {
	if (!mGameStarted) {
		mStartLabel->Render();
	}

	if (mGameStarted) {
		if (mLevelStarted) {
			mLevel->Render();
		}

		mPlayer->Render();   // Make sure this is rendering Player 1
		mPlayer2->Render();  // Make sure this is rendering Player 2
	}

	mSideBar->Render();
}