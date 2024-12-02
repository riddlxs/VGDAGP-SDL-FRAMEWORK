#include "PhysEntity.h"
#include "PhysicsManager.h"
#include "PhysicsHelper.h"

namespace SDLFramework {

    // Constructor: Initializes variables
    PhysEntity::PhysEntity() {
        mBroadPhaseCollider = nullptr;
        mId = 0;
    }

    // Destructor: Cleans up colliders and broad-phase collider
    PhysEntity::~PhysEntity() {
        for (auto collider : mColliders) {
            delete collider;
        }
        mColliders.clear();

        delete mBroadPhaseCollider;
        mBroadPhaseCollider = nullptr;

        if (mId != 0) {
            PhysicsManager::Instance()->UnregisterEntity(mId);
        }
    }

    // Add a collider to the entity
    void PhysEntity::AddCollider(Collider* collider, Vector2 localPos) {
        collider->Parent(this);           // Associate the collider with this entity
        collider->Position(localPos);     // Set the local position for the collider
        mColliders.push_back(collider);   // Add the collider to the entity's list of colliders

        // Update the broad-phase collider based on the furthest distance of all colliders
        if (mColliders.size() > 1 || mColliders[0]->GetType() != Collider::ColliderType::Circle) {
            float furthestDistance = mColliders[0]->GetFurthestPoint().Magnitude();
            float dist = 0.0f;

            // Loop through all colliders to find the furthest point
            for (int i = 1; i < mColliders.size(); i++) {
                dist = mColliders[i]->GetFurthestPoint().Magnitude();

                if (dist > furthestDistance) {
                    furthestDistance = dist;
                }
            }

            // Delete the old broad-phase collider and create a new one
            delete mBroadPhaseCollider;
            mBroadPhaseCollider = new CircleCollider(furthestDistance, true);
            mBroadPhaseCollider->Parent(this);
            mBroadPhaseCollider->Position(Vec2_Zero); // Reset position to (0, 0) or calculate based on entity position
        }
    }

    // Get the entity's ID
    unsigned long PhysEntity::GetId() {
        return mId;
    }

    // Collision check: If this entity ignores collisions, return false
    bool PhysEntity::IgnoreCollisions() {
        return false;
    }

    // Render the entity and its colliders
    void PhysEntity::Render() {
        for (auto collider : mColliders) {
            collider->Render();
        }

        if (mBroadPhaseCollider) {
            mBroadPhaseCollider->Render();
        }
    }

    // Check if this entity collides with another PhysEntity
    bool PhysEntity::CheckCollision(PhysEntity* other) {
        if (IgnoreCollisions() || other->IgnoreCollisions()) {
            return false; // Early exit if one of the entities ignores collisions
        }

        bool narrowPhaseCheck = false;

        // Perform broad-phase collision check using the broad-phase colliders
        if (mBroadPhaseCollider && other->mBroadPhaseCollider) {
            narrowPhaseCheck = ColliderVsColliderCheck(mBroadPhaseCollider, other->mBroadPhaseCollider);
        }

        // If broad-phase collision passed, check narrow-phase collisions
        if (narrowPhaseCheck) {
            for (int i = 0; i < mColliders.size(); i++) {
                for (int j = 0; j < other->mColliders.size(); j++) {
                    if (ColliderVsColliderCheck(mColliders[i], other->mColliders[j])) {
                        return true; // A collision has been detected
                    }
                }
            }
        }

        return false; // No collision detected
    }

}