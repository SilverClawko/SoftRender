#include <fstream>
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(int w, int h, int c, Format f):
	data(nullptr), fdata(nullptr)
{
	CreateImage(w, h, c, f);
}

Image::Image(const std::string &filename) :
	data(nullptr), fdata(nullptr)
{
	LoadImage(filename);
}

Image::Image(const Image & i) :
	width(i.width),height(i.height),channel(i.channel),format(i.format),
	data(nullptr),fdata(nullptr)
{
	int size = width * height * channel;
	CreateImage(width, height, channel, format);
	if (format == LDR) {
		std::copy(i.data, i.data + size, data);
	}
	else {
		std::copy(i.fdata, i.fdata + size, fdata);
		//memcpy(fdata, i.fdata, size * sizeof(float));
	}
}

Image::~Image()
{
	Release();
}

Image & Image::operator=(const Image & i)
{
	width = i.width, height = i.height, channel = i.channel, format = i.format;
	int size = width * height * channel;
	CreateImage(width, height, channel, format);
	if (format == LDR) {
		std::copy(i.data, i.data + size, data);
	}
	else {
		std::copy(i.fdata, i.fdata + size, fdata);
	}
	return *this;
}

void Image::CreateImage(int w, int h, int c, Format f)
{
	int size = w * h * c;
	if (f == HDR) {
		fdata = (float*)malloc(w*h*c * sizeof(float));
		data = nullptr;
		width = w;
		height = h;
		channel = c;
		format = HDR;
	}
	else {
		data = (unsigned char*)malloc(w*h*c);
		fdata = nullptr;
		width = w;
		height = h;
		channel = c;
		format = LDR;
	}
}

void Image::Resize(int w, int h)
{
	Release();
	CreateImage(w, h, channel, format);
}

void Image::Release()
{
	if (data)
		free(data);
	if (fdata)
		free(fdata);
	data = nullptr;
	fdata = nullptr;
	width = 0;
	height = 0;
	channel = 0;
}

void Image::LoadImage(const std::string &filename)
{
	Release();
	const char *extension = strrchr(filename.c_str(), '.') + 1;
	if (strcmp(extension, "hdr") == 0) {
		//stbi_set_flip_vertically_on_load(true);
		fdata = stbi_loadf(filename.c_str(), &width, &height, &channel, 0);
		//linear 2 srgb
		format = HDR;
		//Linear2Srgb();
		//stbi_set_flip_vertically_on_load(false);
	}
	else {
		data = stbi_load(filename.c_str(), &width, &height, &channel, 0);
		format = LDR;
	}
	
}




void Image::SaveImage(const std::string &filename)
{
}

inline void Image::Srgb2Linear()
{
	if (format == HDR) {
		for (int i = 0; i < width * height * channel; i++) {
			*(fdata + i) = pow(*(fdata + i), 2.2);
		}
	}
}

inline void Image::Linear2Srgb()
{
	if (format == HDR) {
		for (int i = 0; i < width * height * channel; i++) {
			*(fdata + i) = pow(*(fdata + i), 0.45);
		}
	}
}

void Image::Float2Char()
{
	data = (unsigned char *)malloc(width * height * channel);
	for (int i = 0; i < width * height; i++) {
		for (int c = 0; c < channel; c++) {
			*(data + i + c) = (unsigned char)(*(fdata + i + c)*255.0f);
		}
	}
	free(fdata);
	fdata = nullptr;
	format = HDR;
}

void Image::Char2Float()
{
	fdata = (float *)malloc(width * height * channel * sizeof(float));
	for (int i = 0; i < width * height; i++) {
		for (int c = 0; c < channel; c++) {
			*(fdata + i + c) = *(fdata + i + c) / 255.0f;
		}
	}
	free(data);
	data = nullptr;
	format = LDR;
}


