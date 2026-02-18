#pragma once
#include "base/shader.h"
#include "color.h"
using namespace actor;
namespace component {	
	struct Canvas {
		float  width, height;
		float  x, y;
	};
	
	struct QuadParam
	{
		float width;
		float height;
		float x,y;
		Canvas canvas;
		float scale = 1.0f;
		Color color = {0.0,1.0,0.0,1.0};
		GLint textureId;
	};

	struct Quad {
		unsigned int quadVAO;
		void Generate();
		void Draw(); 
	};

	void Initialize(glm::vec2 viewSize);
	
	void QuadDraw(QuadParam param);

	void ImageDraw(QuadParam param);

	void Destroy();
}