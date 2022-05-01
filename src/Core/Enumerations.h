#pragma once

namespace ti {
	enum Projection {
		ORTHOGRAPHIC,
		PERSPECTIVE,

		Projection_COUNT
	};

	enum Primitive {
		POINT,
		LINE,
		LINE_STRIP,
		TRIANGLE,
		TRIANGLE_FAN,
		TRIANGLE_STRIP,

		Primitive_COUNT
	};

	enum CameraType {
		FPS,
		TPS,
		Editor,

		CameraType_COUNT
	};
	
	enum ForceMode {
		Impulse,
		Acceleration,
		Force,
		VelocityChange,

		ForceMode_COUNT
	};

	enum LightMode {
		Point,
		Flash,
		Spot,
		Area,
		Directional,

		LightMode_COUNT
	};
}