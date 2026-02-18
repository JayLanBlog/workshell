#pragma once

namespace actor {
	struct Color {
		float r, g, b, a = 1.0;
	};
	Color generateColor();
}