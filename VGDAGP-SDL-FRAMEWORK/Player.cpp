#include "Player.h"
#include "PhysicsManager.h"
#include "Vec2.h" 

Player::Player(bool isPlayerOne) {
    mTimer = Timer::Instance();
    mInput = InputManager::Instance();
    mAudio = AudioManager::Instance();

    mVisible = true;  // Set visible immediately
    mAnimating = false;
    mWasHit = false;

    mScore = 0;
    mLives = 2;

    mScaleX = 1.0f;
    mScaleY = 1.0f;

    mMoveSpeed = 100.0f;
    mMoveBounds = Vector2(0.0f, 800.0f);

    // Initialize mShip depending on whether it's Player 1 or Player 2
    if (isPlayerOne) {
        mShip = new Texture("PlayerShips.png", 0, 0, 60, 64);  // For Player 1
    }
    else {
        mShip = new Texture("PlayerShips.png", 0, 0, 60, 64);  // For Player 2
    }


    mShip->Parent(this);

    mDeathAnimation = new AnimatedTexture("PlayerExplosion.png", 0, 0, 128, 128, 4, 1.0f, AnimatedTexture::Horizontal);
    mDeathAnimation->Parent(this);
    mDeathAnimation->Position(Vec2_Zero);
    mDeathAnimation->SetWrapMode(AnimatedTexture::Once);

    AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
    AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(18.0f, 10.0f));
    AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(-18.0f, 10.0f));

    mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);

    this->isPlayer1 = isPlayerOne;

    mRotation = 0.0f;

    // Log the initialization of the player
    std::cout << "Creating Player " << (isPlayerOne ? "1" : "2") << " ship at position ("
        << mShip->Position().x << ", " << mShip->Position().y << ")\n";
}

Player::~Player() {
    mTimer = nullptr;
    mInput = nullptr;
    mAudio = nullptr;

    // Properly clean up the resources
    delete mShip;
    mShip = nullptr;

    delete mDeathAnimation;
    mDeathAnimation = nullptr;
}

void Player::HandleMovementForPlayer1() {
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_Q)) {
        // Rotate left (counterclockwise)
        mRotation -= 90.0f * mTimer->DeltaTime();  // Rotate left by 90 degrees per second
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_E)) {
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
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_W)) {
        mScaleX += 0.1f;  // Increase scale horizontally
        mScaleY += 0.1f;  // Increase scale vertically
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_S)) {
        mScaleX -= 0.1f;  // Decrease scale horizontally
        mScaleY -= 0.1f;  // Decrease scale vertically
    }
}

void Player::HandleMovementForPlayer2() {
    std::cout << "Handling movement for Player 2" << std::endl;
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_UP)) {
        std::cout << "Player 2 UP key pressed. Position: " << mShip->Position().x << ", " << mShip->Position().y << "\n";
        mRotation -= 90.0f * mTimer->DeltaTime();
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_DOWN)) {
        std::cout << "Player 2 DOWN key pressed. Position: " << mShip->Position().x << ", " << mShip->Position().y << "\n";
        mRotation += 90.0f * mTimer->DeltaTime();
    }

    // Ensure the rotation stays within 0 to 360 degrees
    if (mRotation < 0.0f) mRotation += 360.0f;
    if (mRotation >= 360.0f) mRotation -= 360.0f;

    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_LEFT)) {
        std::cout << "Player 2 LEFT key pressed. Position: " << mShip->Position().x << ", " << mShip->Position().y << "\n";
        Translate(-Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_RIGHT)) {
        std::cout << "Player 2 RIGHT key pressed. Position: " << mShip->Position().x << ", " << mShip->Position().y << "\n";
        Translate(Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_P)) {
        mScaleX += 0.1f;  // Increase scale horizontally
        mScaleY += 0.1f;  // Increase scale vertically
    }
    if (InputManager::Instance()->KeyDown(SDL_SCANCODE_O)) {
        mScaleX -= 0.1f;  // Decrease scale horizontally
        mScaleY -= 0.1f;  // Decrease scale vertically
    }
}

void Player::Update() {
    if (mAnimating) {
        mDeathAnimation->Update();
        mAnimating = mDeathAnimation->IsAnimating();
    }
    else {
        if (Active()) {
            // Log position before handling movement
            std::cout << "Before Movement - Player " << (isPlayer1 ? "1" : "2") << " Position: ("
                << mShip->Position().x << ", " << mShip->Position().y << ")\n";

            mScaleX = std::max(0.5f, std::min(mScaleX, 3.0f));  // Scale between 0.5x and 3x
            mScaleY = std::max(0.5f, std::min(mScaleY, 3.0f));  // Scale between 0.5x and 3x


            if (isPlayer1) {
                HandleMovementForPlayer1();
            }
            else {
                HandleMovementForPlayer2();
            }
            // Log position after handling movement
            std::cout << "After Movement - Player " << (isPlayer1 ? "1" : "2") << " Position: ("
                << mShip->Position().x << ", " << mShip->Position().y << ")\n";
        }
    }
}

void Player::Render() {
    if (mVisible) {
        std::cout << "Rendering Player " << (isPlayer1 ? "1" : "2")
            << " at position (" << mShip->Position().x << ", "
            << mShip->Position().y << ")\n";

        if (mAnimating) {
            mDeathAnimation->Render();
        }
        else {
            mShip->RenderEx(Position(World), mRotation, Vec2(mScaleX, mScaleY));

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
