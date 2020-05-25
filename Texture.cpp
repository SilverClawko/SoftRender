#include "Texture.h"
#include "Image.h"


Texture2D::Texture2D() : image(nullptr)
{
}

Texture2D::Texture2D(const std::string &filename) : image(nullptr)
{
	image = new Image(filename);
}

Texture2D::Texture2D(Image * img) : image(nullptr)
{
	image = new Image(*img);
}

Texture2D::Texture2D(const Texture2D & t) : image(nullptr)
{
	image = new Image(*t.image);
}

Image * Texture2D::GetImage() const
{
	return image;
}

Texture2D::~Texture2D()
{
	if (image)
		delete image;
	image = nullptr;
}

glm::vec4 Texture2D::Sample2D(const glm::vec2 & texcoord)
{
	//1.3 -> 0.3    
	float u = texcoord.x;
	float v = texcoord.y;
	if (u > 1.0f || u < 0.0f) {
		u = u - (float)floor(u);
	}
	if (v > 1.0f || v < 0.0f) {
		v = v - (float)floor(v);
	}
	int x = (int)(u * (image->width - 1));
	int y = (int)(v * (image->height - 1));
	int pos = (y * image->width + x) * image->channel;
	if (image->format == HDR) {
		float * p = image->fdata;
		if (image->channel == 1)
			return glm::vec4(*(p + pos), 1.0f, 1.0f, 1.0f);
		if (image->channel == 3)
			return glm::vec4(*(p + pos), *(p + pos + 1), *(p + pos + 2), 1.0f);
		if (image->channel == 4)
			return glm::vec4(*(p + pos), *(p + pos + 1), *(p + pos + 2), *(p + pos + 3));
		if (image->channel == 2)
			return glm::vec4(*(p + pos), *(p + pos + 1), 1.0f, 1.0f);
	}
	else {
		unsigned char * p = image->data;
		if (image->channel == 3)
			return glm::vec4(*(p + pos) / 255.0f, *(p + pos + 1) / 255.0f, *(p + pos + 2) / 255.0f, 1.0f);
		if (image->channel == 4)
			return glm::vec4(*(p + pos) / 255.0f, *(p + pos + 1) / 255.0f, *(p + pos + 2) / 255.0f, *(p + pos + 3) / 255.0f);
		if (image->channel == 1)
			return glm::vec4(*(p + pos) / 255.0f, 1.0f, 1.0f, 1.0f);
		if (image->channel == 2)
			return glm::vec4(*(p + pos) / 255.0f, *(p + pos + 1) / 255.0f, 1.0f, 1.0f);
	}
	return glm::vec4(1.0f);
}


Texture3D::Texture3D(const std::string &filename) 
{
	faces.reserve(6);
	std::vector<glm::vec2> faceCoords(6);
	Image * image = new Image(filename);
	int faceWidth = image->width / 4;
	int faceHeight = image->height / 3;
	int channels = image->channel;

	faceCoords[left] = glm::vec2(0, faceHeight);
	faceCoords[back] = glm::vec2(faceWidth, faceHeight);
	faceCoords[bottom] = glm::vec2(faceWidth, 0);
	faceCoords[top] = glm::vec2(faceWidth, 2 * faceHeight);
	faceCoords[right] = glm::vec2(2 * faceWidth, faceHeight);
	faceCoords[front] = glm::vec2(3 * faceWidth, faceHeight);
	for (int i = 0; i < 6; i++) {
		Image * tmp = new Image(faceWidth, faceHeight, channels, HDR);
		float * src = image->fdata;
		float * dst = tmp->fdata;
		for (int y = 0; y < faceHeight; y++) {
			for (int x = 0; x < faceWidth; x++)
			{
				int fx = faceCoords[i].x + x;
				int fy = faceCoords[i].y + y;
				int pos = (y * faceWidth + x)* channels;
				int fpos = (fy * image->width + fx) * channels;
				for (int c = 0; c < channels; c++) {
					*(dst + pos + c) = *(src + fpos + c);
				}
			}
		}
		faces.push_back(new Texture2D(tmp));
		delete tmp;
	}
	delete image;
}

Texture3D::Texture3D(const Texture3D & t) 
{
	for (int i = 0; i < 6; i++) {
		faces.push_back(new Texture2D(*t.faces[i]));
	}
}

Texture3D::~Texture3D()
{}

inline float max(float a, float b) {
	return a > b ? a : b;
}

glm::vec4 Texture3D::SampleCUBE(const glm::vec3 & v)
{
	float absX = fabs(v.x);
	float absY = fabs(v.y);
	float absZ = fabs(v.z);
	float ma, sc, tc;
	FaceDir faceIndex;
	ma = max(max(absX, absY), absZ);
	// +X 左 +Y 上 +Z 前  右手系
	if (ma == absX) {   
		if (v.x > 0) {                 
			faceIndex = right;      
			sc = -v.z;
			tc = -v.y;
		}
		else {                               
			faceIndex = left;
			sc = +v.z;
			tc = -v.y;
		}
	}
	else if (ma == absY) {             
		if (v.y > 0) {
			faceIndex = bottom;
			sc = +v.x;
			tc = +v.z;
		}
		else {                                
			faceIndex = top;
			sc = +v.x;
			tc = -v.z;
		}
	}
	else {                                
		if (v.z > 0) {
			faceIndex = back;
			sc = +v.x;
			tc = -v.y;
		}
		else {                                
			faceIndex = front;
			sc = -v.x;
			tc = -v.y;
		}
	}
	glm::vec2 uv = glm::vec2((sc / ma + 1) / 2, (tc / ma + 1) / 2);
	//return glm::vec4(uv, 1.0, 1.0);
	return faces[faceIndex]->Sample2D(uv);
	/*
	float m = max(fabs(v.x), max(fabs(v.y), fabs(v.z)));
	if (m == -v.x) {
		glm::vec2 uv((-v.z / fabs(v.x) + 1) / 2, (v.y / fabs(v.x) + 1) / 2);
		return faces[right]->Sample2D(uv);
	}
	else if (m == v.x) {
		glm::vec2 uv((v.z / fabs(v.x) + 1) / 2 , (v.y / fabs(v.x) + 1) / 2 );		
		return faces[left]->Sample2D(uv);
	}
	else if (m == v.y) {
		glm::vec2 uv((-v.x / fabs(v.y) + 1) / 2 , (-v.z / fabs(v.y) + 1) / 2 );
		return faces[top]->Sample2D(uv);
	}
	else if (m == -v.y) {
		glm::vec2 uv((-v.x / fabs(v.y) + 1) / 2 , (v.z / fabs(v.y) + 1) / 2 );
		return faces[bottom]->Sample2D(uv);
	}
	else if (m == v.z) {
		glm::vec2 uv((-v.x / fabs(v.z) + 1) / 2 , (v.y / fabs(v.z) + 1) / 2 );
		return faces[back]->Sample2D(uv);
	}
	else {
		glm::vec2 uv((v.x / fabs(v.z) + 1) / 2 , (v.y / fabs(v.z) + 1) / 2);
		return faces[front]->Sample2D(uv);
	}
	*/
}



