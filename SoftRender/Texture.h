
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Globel.h"

class Texture {

public:
	int width;
	int height;
	int channels;
	unsigned char * data;

	Texture() : width(-1),height(-1),channels(-1),data(nullptr)
	{}
	Texture(const std::string &Path) {
		LoadTexture(Path);
	}
	virtual ~Texture() {
		if (data)
			free(data);
	}
	Texture(const Texture & t) {
		width = t.width;
		height = t.height;
		channels = t.channels;
		if (t.data) {
			data = (unsigned char *)malloc(width * height * channels);
			memcpy(data, t.data, width * height * channels);
		}
	}
	Texture & operator=(const Texture & t) {
		width = t.width;
		height = t.height;
		channels = t.channels;
		if (t.data) {
			data = (unsigned char *)malloc(width * height * channels);
			memcpy(data, t.data, width * height * channels);
		}
		return *this;
	}

	virtual void LoadTexture(const std::string &Path) {
		stbi_set_flip_vertically_on_load(false);
		if(data)
			free(data);
		data = stbi_load(Path.c_str(), &width, &height, &channels, 0);
	}

	virtual glm::vec4 Sample2D(const glm::vec2 &texcoord) {
		//GL_REPEAT
		float x = texcoord.x - (int)texcoord.x;
		float y = texcoord.y - (int)texcoord.y;
		return GetColor(x*width, y*height)/255.0f;
	}


	glm::vec4 GetColor(int x, int y) {
		if (x <0 || x >= width || y < 0 || y>= height)
			return glm::vec4(0,0,0,0);
		switch (channels) {
		case 1:
			return GetColor1(x, y);
			break;
		case 2:
			return GetColor2(x, y);
			break;
		case 3:
			return GetColor3(x, y);
			break;
		case 4:
			return GetColor4(x, y);
			break;
		default:
			return glm::vec4(0, 0, 0, 0);
		}
	}
	glm::vec4 GetColor1(int x, int y) {
		int xy = y * width + x;
		return glm::vec4(*(data + xy), 0, 0, 0);
	}
	glm::vec4 GetColor2(int x, int y) {
		int xy = 2*(y * width + x);
		return glm::vec4(*(data + xy), *(data + xy + 1), 0, 0);
	}
	glm::vec4 GetColor3(int x, int y) {
		int xy = 3 * (y * width + x);
		return glm::vec4(*(data + xy), *(data + xy + 1), *(data + xy + 2), 0);
	}
	glm::vec4 GetColor4(int x, int y) {
		int xy = 4 * (y * width + x);
		return glm::vec4(*(data + xy), *(data + xy + 1), *(data + xy + 2), *(data + xy + 3));
	}
};

#endif