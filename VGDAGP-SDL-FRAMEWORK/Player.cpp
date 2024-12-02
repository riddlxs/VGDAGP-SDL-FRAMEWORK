#include "Player.h"
#include "PhysicsManager.h"

Player::Player(bool isPlayerOne) {
    mTimer = Timer::Instance();
    mInput = InputManager::Instance();
    mAudio = AudioManager::Instance();

    mVisible = true;  // Set visible immediately
    mAnimating = false;
    mWasHit = false;

    mScore = 0;
    mLives = 2;

    mMoveSpeed = 100.0f;
    mMoveBounds = Vector2(0.0f, 800.0f);

    mShip = new Texture("PlayerShips.png", 0, 0, 60, 64);
    mShip->Parent(this);
    mShip->Position(Vec2_Zero);

    mDeathAnimation = new AnimatedTexture("PlayerExplosion.png", 0, 0, 128, 128, 4, 1.0f, AnimatedTexture::Horizontal);
    mDeathAnimation->Parent(this);
    mDeathAnimation->Position(Vec2_Zero);
    mDeathAnimation->SetWrapMode(AnimatedTexture::Once);

    AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
    AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(18.0f, 10.0f));
    AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(-18.0f, 10.0f));

    mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);

    this->isPlayer1 = isPlayerOne;

    // Offset Player 2's position
    if (!isPlayerOne) {
        mShip->Position(Vector2(200, 0));  // Set Player 2 at a different position
    }
    mRotation = 0.0f;
}


Player::~Player() {
    mTimer = nullptr;
    mInput = nullptr;
    mAudio = nullptr;

    delete mShip;
    mShip = nullptr;

    delete mDeathAnimation;
    mDeathAnimation = nullptr;
}

void Player::HandleMovementForPlayer1() {
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_Q)){
        // Rotate left (counterclockwise)
        mRotation -= 90.0f * mTimer->DeltaTime();  // Rotate left by 90 degrees per second
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_E)){
        // Rotate right (clockwise)
        mRotation += 90.0f * mTimer->DeltaTime();  // Rotate right by 90 degrees per second
    }

    // Ensure the rotation stays within 0 to 360 degrees
    if (mRotation < 0.0f) mRotation += 360.0f;
    if (mRotation >= 360.0f) mRotation -= 360.0f;

    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_A)) {
        Translate(-Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_D)) {
        Translate(Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
    }
}

void Player::HandleMovementForPlayer2() {
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_UP)){
        // Rotate left (counterclockwise)
        mRotation -= 90.0f * mTimer->DeltaTime();
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_DOWN)){
        // Rotate right (clockwise)
        mRotation += 90.0f * mTimer->DeltaTime();
    }

    // Ensure the rotation stays within 0 to 360 degrees
    if (mRotation < 0.0f) mRotation += 360.0f;
    if (mRotation >= 360.0f) mRotation -= 360.0f;

    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_LEFT)) {
        Translate(-Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_RIGHT)) {
        Translate(Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
    }
}

void Player::Update() {
    if (mAnimating) {
        mDeathAnimation->Update();
        mAnimating = mDeathAnimation->IsAnimating();
    }
    else {
        if (Active()) {
            // Check which player it is (Player 1 or Player 2)
            if (isPlayer1) {
                // Player 1 handles WASD movement
                HandleMovementForPlayer1();
            }
            else {
                // Player 2 handles Arrow key movement
                HandleMovementForPlayer2();
            }
        }
    }
}

void Player::Render() {
    if (mVisible) {
        if (mAnimating) {
            mDeathAnimation->Render();
        }
        else {
            mShip->Render();
        }

        PhysEntity::Render();
    }
}

void Player::Visible(bool visible) {
    mVisible = visible;
}

bool Player::IsAnimating() {
    return mAnimating;
}

int Player::Score() {
    return mScore;
}

int Player::Lives() {
    return mLives;
}

void Player::AddScore(int change) {
    mScore += change;
}

void Player::WasHit() {
    mLives -= 1;
    mAnimating = true;
    mDeathAnimation->ResetAnimation();
    mAudio->PlaySFX("SFX/PlayerExplosion.wav");
}

bool Player::IgnoreCollisions() {
    return !mVisible || mAnimating;
}

void Player::Hit(PhysEntity* other) {
    mLives -= 1;
    mAnimating = true;
    mDeathAnimation->ResetAnimation();
}
