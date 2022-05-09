#include "Physics.h"

void ti::System::Physics::Update(double dt) {
	using namespace ti::Component;

	for (auto& entity: registry->View<Tag, Transform, Rigidbody>()) {
		auto& transform = registry->Get<Transform>(entity);
		auto& rigidbody = registry->Get<Rigidbody>(entity);

		if (!rigidbody.use_gravity) continue;

		transform.position.y -= dt;
	}
}
