#pragma once
#include <bitset>
#include "PhysEntity.h"

namespace SDLFramework {

    class PhysicsManager {
    public:
        enum class CollisionLayers {
            Friendly, // the 4 physics layers
            FriendlyProjectile, // 2
            Hostile, // 3
            HostileProjectile, // 4
            MaxLayers
        };

        enum class CollisionFlags {
            None = 0x00,
            Friendly = 0x01, // 1
            FriendlyProjectile = 0x02, // 2
            Hostile = 0x04, // 3
            HostileProjectile = 0x08, // 4
        };

        // Singleton pattern for PhysicsManager
        static PhysicsManager* Instance();
        static void Release();

        // Methods to register entities, unregister, set collision masks, and update physics
        unsigned long RegisterEntity(PhysEntity* entity, CollisionLayers layer);
        void UnregisterEntity(unsigned long id);

        // Method to set collision masks for layers
        void SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flags);

        // Update method to check collisions
        void Update();

    private:
        PhysicsManager();
        ~PhysicsManager();

       
        static PhysicsManager* sInstance;

      
        std::vector<PhysEntity*> mCollisionLayers[static_cast<unsigned int>(CollisionLayers::MaxLayers)];
        std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)> mLayerMasks[static_cast<unsigned int>(CollisionLayers::MaxLayers)];

        unsigned long mLastID;
    };

 
    inline PhysicsManager::CollisionFlags operator|(PhysicsManager::CollisionFlags a, PhysicsManager::CollisionFlags b) {
        return static_cast<PhysicsManager::CollisionFlags>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    }

    inline PhysicsManager::CollisionFlags operator&(PhysicsManager::CollisionFlags a, PhysicsManager::CollisionFlags b) {
        return static_cast<PhysicsManager::CollisionFlags>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
    }
}
