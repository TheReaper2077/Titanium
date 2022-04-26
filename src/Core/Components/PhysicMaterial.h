#pragma once

namespace ti {
	namespace Component {
		struct PhysicMaterial {
			float static_friction;
			float kinetic_friction;
			float rolling_friction;
			float bounciness;
		};
	}
}