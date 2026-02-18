#include "color.h"
#include <stdlib.h>



namespace actor {


	Color generateColor() {
		float r = 1.0 * (rand() % 10);
		float g = 1.0 * (rand() % 10);
		float b = 1.0 * (rand() % 10);
		return { r,g,b ,1.0};
	}
}