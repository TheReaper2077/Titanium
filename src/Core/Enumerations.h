#pragma once

namespace ti {
	enum Projection {
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	enum Primitive {
		POINT,
		LINE,
		LINE_STRIP,
		TRIANGLE,
		TRIANGLE_FAN,
		TRIANGLE_STRIP,
	};
	
	enum ForceMode {
		Impulse,
		Acceleration,
		Force,
		VelocityChange
	};

	enum LightMode {
		Point,
		Flash,
		Spot,
		Area,
		Directional,
	};
}