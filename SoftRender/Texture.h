
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
		data = stbi_load(Path.c_str(), &width, &height, &channels, 0);
	}
	~Texture() {
		if (data)
			free(data);
	}
	Texture(const Texture & t) {
		width = t.width;
		height = t.height;
		channels = t.channels;
		data = (unsigned char *)malloc(width * height * channels);
		memcpy(data, t.data, width * height * channels);
	}
	Texture & operator=(const Texture & t) {
		width = t.width;
		height = t.height;
		channels = t.channels;
		data = (unsigned char *)malloc(width * height * channels);
		memcpy(data, t.data, width * height * channels);
		return *this;
	}

	void LoadTexture(const std::string &Path) {
		if(data)
			free(data);
		data = stbi_load(Path.c_str(), &width, &height, &channels, 0);
	}
	glm::vec4 Sample2D(const glm::vec2 &texcoord) {
		//GL_REPEAT
		float x = texcoord.x - (int)texcoord.x;
		float y = texcoord.y - (int)texcoord.y;
			x = x < 0 ? -x : x;
			y = y < 0 ? -y : y;
		return GetColor(x*width, y*height)/255.0f;
	}
	glm::vec4 GetColor(int x, int y) {
		if (x <0 || x > width || y < 0 || y> height)
			return glm::vec4(0,0,0,0);
		int xy = channels * (y * width + x);
		return glm::vec4(*(data + xy), *(data + xy + 1), *(data + xy + 2), *(data + xy + 3));
	}
};

#endif