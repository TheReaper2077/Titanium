#pragma once

enum Layout {
	position,
	normal,
	color,
	uv0,
	uv1,
	uv2,
	uv3,
	uv4,
	uv5,
	uv6,
	uv7,
};

#define LAYOUT_POSITION_BIT (1 << position)
#define LAYOUT_NORMAL_BIT (1 << normal)
#define LAYOUT_COLOR_BIT (1 << color)
#define LAYOUT_UV0_BIT (1 << uv0)
#define LAYOUT_UV1_BIT (1 << uv1)
#define LAYOUT_UV2_BIT (1 << uv2)
#define LAYOUT_UV3_BIT (1 << uv3)
#define LAYOUT_UV4_BIT (1 << uv4)
#define LAYOUT_UV5_BIT (1 << uv5)
#define LAYOUT_UV6_BIT (1 << uv6)
#define LAYOUT_UV7_BIT (1 << uv7)

