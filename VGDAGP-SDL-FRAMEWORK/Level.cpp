#include "Level.h"

void Level::StartStage() {
	mStageStarted = true;
}

Level::Level(int stage, PlaySideBar* sideBar, Player* player1, Player* player2) {
	mTimer = Timer::Instance();
	mSideBar = sideBar;
	mSideBar->SetLevel(stage);
	mBackgroundStars = BackgroundStars::Instance();


	mStage = stage;
	mStageStarted = false;
	mLabelTimer = 0.0f;

	mStageLabel = new Texture("STAGE", "emulogic.ttf", 32, { 75, 75, 200 });
	mStageLabel->Parent(this);
	mStageLabel->Position(Graphics::SCREEN_WIDTH * 0.35f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageNumber = new Scoreboard({ 75, 75, 200 });
	mStageNumber->Score(stage);
	mStageNumber->Parent(this);
	mStageNumber->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageLabelOnScreen = 0.0f;
	mStageLabelOffScreen = 1.5f;

	mReadyLabel = new Texture("READY", "emulogic.ttf", 32, { 150, 0, 0 });
	mReadyLabel->Parent(this);
	mReadyLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);

	mReadyLabelOnScreen = mStageLabelOffScreen;
	mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

	mPlayer = player1;  // Assign player1
	mPlayer2 = player2; // Assign player2
	mPlayerHit = false;
	mRespawnDelay = 3.0f;
	mRespawnLabelOnScreen = 2.0f;

	mGameOverLabel = new Texture("GAME OVER!", "emulogic.ttf", 32, { 150, 0 ,0 });
	mGameOverLabel->Parent(this);
	mGameOverLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);

	mGameOverDelay = 6.0f;
	mGameOverTimer = 0.0f;
	mGameOverLabelOnScreen = 1.0f;

	mCurrentState = Running;

}



Level::~Level() {
	mTimer = nullptr;
	mSideBar = nullptr;
	mBackgroundStars = nullptr;

	delete mStageLabel;
	mStageLabel = nullptr;
	delete mStageNumber;
	mStageNumber = nullptr;
	delete mReadyLabel;
	mReadyLabel = nullptr;

	delete mGameOverLabel;
	mGameOverLabel = nullptr;
}

Level::LevelStates Level::State() {
	return mCurrentState;
}

void Level::HandleStartLabels() {
	mLabelTimer += mTimer->DeltaTime();

	if (mLabelTimer >= mStageLabelOffScreen) {
		mBackgroundStars->Scroll(true);
		mPlayer->Active(true);
		mPlayer->Visible(true);


		if (mStage > 1) {
			StartStage();
		}
		else {
			//READY label will only display at the start of level 1
			if (mLabelTimer >= mReadyLabelOffScreen) {
				StartStage();
			}
		}
	}
}


void Level::HandleCollisions() {
	if (!mPlayerHit) {
		//Quick sanity test
		if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_X)) {
			mPlayer->WasHit();
			mSideBar->SetShips(mPlayer->Lives());

			mPlayerHit = true;
			mRespawnTimer = 0.0f;
			mPlayer->Active(false);
			mBackgroundStars->Scroll(false);
		}
	}
}

void Level::HandlePlayerDeath() {
	if (!mPlayer->IsAnimating()) {
		if (mPlayer->Lives() > 0) {
			if (mRespawnTimer == 0.0f) {
				//The player has finished their death animation and we want them to respawn
				//Hide them until the respawn timer has finished
				mPlayer->Visible(false);
			}

			mRespawnTimer += mTimer->DeltaTime();
			if (mRespawnTimer >= mRespawnDelay) {
				//This is when we respawn the player
				mPlayer->Active(true);
				mPlayer->Visible(true);
				mPlayerHit = false;
				mBackgroundStars->Scroll(true);
			}
		}
		else {
			//This is GAME OVER
			if (mGameOverTimer == 0.0f) {
				mPlayer->Visible(false);
			}

			mGameOverTimer += mTimer->DeltaTime();
			if (mGameOverTimer >= mGameOverDelay) {
				mCurrentState = GameOver;
			}
		}
	}
}

void Level::Update() {
	if (!mStageStarted) {
		HandleStartLabels();
	}
	else {
		HandleCollisions();

		if (mPlayerHit) {
			HandlePlayerDeath();
		}
		else {
			//TODO: Temporary logic until enemeies implemented
			if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_N)) {
				mCurrentState = Finished;
			}
		}
	}
}

void Level::Render() {
	if (!mStageStarted) {
		if (mLabelTimer > mStageLabelOnScreen && mLabelTimer < mStageLabelOffScreen) {
			mStageLabel->Render();
			mStageNumber->Render();
		}
		else if (mLabelTimer > mReadyLabelOnScreen && mLabelTimer < mReadyLabelOffScreen) {
			mReadyLabel->Render();
		}
	}
	else {
		if (mPlayerHit) {
			if (mRespawnTimer >= mRespawnLabelOnScreen) {
				mReadyLabel->Render();
			}

			if (mGameOverTimer >= mGameOverLabelOnScreen) {
				mGameOverLabel->Render();
			}
		}
		mPlayer->Render();
		mPlayer2->Render();
	}
}