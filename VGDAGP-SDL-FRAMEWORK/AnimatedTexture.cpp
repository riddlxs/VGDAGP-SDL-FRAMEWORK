#include "AnimatedTexture.h"  // This file contains the class declaration

namespace SDLFramework {

    // Constructor definition
    AnimatedTexture::AnimatedTexture(std::string filename, int x, int y, int width, int height,
        int frameCount, float animSpeed, AnimDir animationDir, bool managed)
        : Texture(filename, x, y, width, height, managed)  // Initialize base class Texture
    {
        mStartx = x;
        mStarty = y;
        mFrameCount = frameCount;
        mAnimationSpeed = animSpeed;
        mAnimationDirection = animationDir;
        mWrapMode = Once;  // Default wrap mode
        mAnimationDone = false;
        mTimer = new Timer();
        mTimePerFrame = 1.0f / animSpeed;
        mAnimationTimer = 0.0f;
    }

    // Destructor definition
    AnimatedTexture::~AnimatedTexture() {
        delete mTimer;  // Clean up the Timer object
    }

    // Other methods can be defined here
    void AnimatedTexture::SetWrapMode(WrapMode mode) {
        mWrapMode = mode;
    }

    bool AnimatedTexture::IsAnimating() {
        return !mAnimationDone;
    }

    void AnimatedTexture::ResetAnimation() {
        mAnimationTimer = 0.0f;
        mAnimationDone = false;
    }

    void AnimatedTexture::Update() {
        // Update logic for the animation
        // Example: Increase animation timer and check if it's time to move to the next frame
    }

    void AnimatedTexture::RunAnimation() {
        // Logic for actually running the animation
    }
}
