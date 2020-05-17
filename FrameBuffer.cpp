#include "FrameBuffer.h"
#include "Image.h"


FrameBuffer::FrameBuffer(int w, int h) : 
	width(w),height(h),colorBuffer(nullptr),depthBuffer(nullptr)
{
	colorBuffer = new Image(w, h, 4, LDR);
	depthBuffer = new Image(w, h, 1, HDR);
}

FrameBuffer::~FrameBuffer()
{
	if (colorBuffer)
		delete colorBuffer;
	if (depthBuffer)
		delete depthBuffer;
	colorBuffer = nullptr;
	depthBuffer = nullptr;
}

void FrameBuffer::Resize(int w, int h)
{
	width = w;
	height = h;
	colorBuffer->Resize(w, h);
	depthBuffer->Resize(w, h);
}

void FrameBuffer::ClearColor(int r, int g, int b, int a)
{
	int size = width * height;
	unsigned char * p = colorBuffer->data;
	for (int i = 0; i < size; i++) {
		*(p + i * 4) = (unsigned char)r;
		*(p + i * 4 + 1) = (unsigned char)g;
		*(p + i * 4 + 2) = (unsigned char)b;
		*(p + i * 4 + 3) = (unsigned char)a;
	}
}



void FrameBuffer::ClearColor(const glm::vec4 & color)
{
	glm::vec4 c = color * 255.0f;
	int size = width * height;
	unsigned char * p = colorBuffer->data;
	for (int i = 0; i < size; i++) {
		*(p + i * 4) = (unsigned char)c.r;
		*(p + i * 4 + 1) = (unsigned char)c.g;
		*(p + i * 4 + 2) = (unsigned char)c.b;
		*(p + i * 4 + 3) = (unsigned char)c.a;
	}
}

void FrameBuffer::ClearDepth(float d = 1.0f)
{
	int size = width * height;
	float * p = depthBuffer->fdata;
	for (int i = 0; i < size; i++) {
		*(p + i) = d;
	}
}

inline float saturate(float a) {
	if (a < 0.0f)
		return 0.0f;
	if (a > 1.0f)
		return 1.0f;
	return a;
}

void FrameBuffer::WriteColor(int x, int y, int r, int g, int b, int a)
{
	int pos = 4 * (y * width + x);
	unsigned char * p = colorBuffer->data;
	*(p + pos) = (unsigned char)r;
	*(p + pos + 1) = (unsigned char)g;
	*(p + pos + 2) = (unsigned char)b;
	*(p + pos + 3) = (unsigned char)a;
}

void FrameBuffer::WriteColor(int x,int y, const glm::vec4 & color)
{
	
	int r, g, b, a;
	r = saturate(color.x) * 255;
	g = saturate(color.y) * 255;
	b = saturate(color.z) * 255;
	a = saturate(color.w) * 255;

	int pos = 4 * (y * width + x);
	unsigned char * p = colorBuffer->data;
	*(p + pos) = (unsigned char)r;
	*(p + pos + 1) = (unsigned char)g;
	*(p + pos + 2) = (unsigned char)b;
	*(p + pos + 3) = (unsigned char)a;
}

void FrameBuffer::WriteDepth(int x, int y, float d)
{
	int pos = y * width + x;
	float * p = depthBuffer->fdata;
	*(p + pos) = d;
}

float FrameBuffer::GetDepth(int w, int h)
{
	int pos = h * width + w;
	float * p = depthBuffer->fdata;
	return *(p + pos);
}

glm::vec4 FrameBuffer::GetColor(int w, int h)
{
	int pos = (h * width + w) * 4;
	unsigned char * p = colorBuffer->data;
	return glm::vec4(*(p + pos), *(p + pos + 1), *(p + pos + 2), *(p + pos + 3));
}
