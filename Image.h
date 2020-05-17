#ifndef IMAGE_H
#define IMAGE_H
#include "Global.h"

enum Format {
	LDR,
	HDR
};

class Image {

public:

	Format format;
	int width;
	int height;
	int channel;

	unsigned char *data;
	float *fdata;

	Image(int w, int h, int c, Format f);
	Image(const std::string &);
	Image(const Image & i);
	~Image();

	Image & operator= (const Image & i);
	void CreateImage(int w, int h, int c, Format f);
	void Resize(int w,int h);
	void Release();
	void LoadImage(const std::string & filename);
	void SaveImage(const std::string & filename);
	inline void Srgb2Linear();
	inline void Linear2Srgb();
	void Float2Char();
	void Char2Float();

};

#endif