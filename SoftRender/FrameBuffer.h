
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Vertex.h"
#include <array>

//帧缓冲
class FrameBuffer {

public:

	int Width, Height;
	std::vector<unsigned char> colorBuffer;
	std::vector<float> depthBuffer;

	~FrameBuffer() = default;

	FrameBuffer(const int &w = 800, const int &h = 600) {
		Width = w;
		Height = h;
		//RGBA
		colorBuffer.resize(w*h * 4,0);
		depthBuffer.resize(w*h, 1.0f);
	}

	void Resize(const int &w, const int &h) {
		Width = w;
		Height = h;
		colorBuffer.resize(w*h * 4,0);
		depthBuffer.resize(w*h, 1.0f);
	}

	// OpenGL的深度缓冲 [-1,1) 靠近屏幕是负
	void ClearColorBuffer(const glm::vec4 & color) {
		unsigned char * p = colorBuffer.data();
		for (int i = 0; i < Width*Height * 4; i += 4) {
			*(p + i) = color.r * 255;
			*(p + i + 1) = color.g * 255;
			*(p + i + 2) = color.b * 255;
			*(p + i + 3) = color.a * 255;
		}	
		depthBuffer.assign(Width*Height, 1.0f);
	}


	float GetDepth(const int &x, const int  &y) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return 1.0;
		return *(depthBuffer.data() + y* Width + x );
	}


	void WritePoint(const int &x, const int &y, const glm::vec4 &color) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return;
		int xy = (y * Width + x);
		unsigned char * p = colorBuffer.data();
		*(p + xy*4) = saturate(color.r) * 255;
		*(p + xy*4 + 1) = saturate(color.g) * 255;
		*(p + xy*4 + 2) = saturate(color.b) * 255;
		*(p + xy*4 + 3) = saturate(color.a) * 255;
	}
	// Z-Buff [0,1]
	void WriteDepth(const int &x,const int &y, const float &depth) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return;
		float * p = depthBuffer.data();
		*(p+ y*Width + x) = depth;
	}
};
#endif