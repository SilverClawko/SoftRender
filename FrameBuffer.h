#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Global.h"

class Image;

class FrameBuffer
{

public:
	int width;
	int height;
	Image * colorBuffer;
	Image * depthBuffer;

	FrameBuffer(int w, int h);
	~FrameBuffer();

	void Resize(int w, int h);
	void ClearColor(int r = 0, int g = 0, int b = 0, int a = 255);
	void ClearColor(const glm::vec4 &color);
	void ClearDepth(float d);
	void WriteColor(int x, int y, int r, int g, int b, int a = 255);
	void WriteColor(int x, int y, const glm::vec4 &color);
	void WriteDepth(int x, int y, float d);

	float GetDepth(int w, int h);
	glm::vec4 GetColor(int w, int h);
};

#endif