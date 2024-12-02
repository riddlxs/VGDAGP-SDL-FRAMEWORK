#pragma once
#include <bitset>
#include "PhysEntity.h"

namespace SDLFramework {

	class PhysicsManager {
	public:
		enum class CollisionLayers { // 4 phsyics layers
			Friendly, // 1
			FriendlyProjectile, // 2
			Hostile, // 3
			HostileProjectile, // 4
			MaxLayers
		};

		enum class CollisionFlags {
			None = 0x00,
			Friendly = 0x01,
			FriendlyProjectile = 0x02,
			Hostile = 0x04,
			HostileProjectile = 0x08
		};
		 //create the 4 objects
		PhysEntity* entity1 = new PhysEntity(); // Friendly entity
		PhysEntity* entity2 = new PhysEntity(); // FriendlyProjectile entity
		PhysEntity* entity3 = new PhysEntity(); // Hostile entity
		PhysEntity* entity4 = new PhysEntity(); // HostileProjectile entity

		static PhysicsManager* Instance();
		static void Release();

		unsigned long RegisterEntity(PhysEntity* entity, CollisionLayers layer);
		void UnregisterEntity(unsigned long id);

		void SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flags);

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