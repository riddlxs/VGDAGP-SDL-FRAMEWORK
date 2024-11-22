#pragma once
#include "Collider.h"
#include <vector>

namespace SDLFramework {
	//Base class for any object we want to have colliders
	class PhysEntity : public GameEntity {
	public:
		PhysEntity();
		virtual ~PhysEntity();

		unsigned long GetId();

		bool CheckCollision(PhysEntity* other);

		virtual void Hit(PhysEntity* other);

		virtual void Render() override;

	protected:
		unsigned long mId;

		std::vector<Collider*> mColliders;

		Collider* mBroadPhaseCollider;

		virtual bool IgnoreCollisions();

		void AddCollider(Collider* collider, Vector2 locaPos = Vec2_Zero);
	};
}