#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "Texture.h"

enum faceDir {
	front,
	back,
	left,
	right,
	top,
	bottom
};

class CubeMap : public Texture {
private:
	float * fdata;
	std::vector<glm::vec2> faceCoords;

public:
	int faceWidth = -1, faceHeight = -1;


	CubeMap():Texture(),fdata(nullptr),faceCoords(6) {}
	CubeMap(const std::string &Path) : faceCoords(6) {
		LoadTexture(Path);
	}
	CubeMap(const CubeMap& c) : Texture(c), faceCoords(6) {
		if (c.fdata) {
			fdata = (float *)malloc(width * height * channels * 4);
			memcpy(fdata, c.fdata, width * height * channels * 4);
		}
	}
	CubeMap &operator = (const CubeMap &c) {
		Texture::operator=(static_cast<Texture>(c));
		if (c.fdata) {
			fdata = (float *)malloc(width * height * channels * 4);
			memcpy(fdata, c.fdata, width * height * channels * 4);
		}
	}
	virtual void LoadTexture(const std::string &Path) {
		stbi_set_flip_vertically_on_load(true);
		if (fdata)
			free(fdata);
		fdata = stbi_loadf(Path.c_str(), &width, &height, &channels, 0);
		faceWidth = width / 4;
		faceHeight = height / 3;
		faceCoords[left] = glm::vec2(0,faceHeight);
		faceCoords[back] = glm::vec2(faceWidth, faceHeight);
		faceCoords[bottom] = glm::vec2(faceWidth, 0);
		faceCoords[top] = glm::vec2(faceWidth,  2 * faceHeight);
		faceCoords[right] = glm::vec2(2 * faceWidth, faceHeight);
		faceCoords[front] = glm::vec2(3 * faceWidth, faceHeight);
	}

	glm::vec4 Sample3D(const glm::vec3 &v) {
		float m = max(fabs(v.x), max(fabs(v.y), fabs(v.z)));
		if (m == -v.x) {
			glm::vec2 uv((-v.z/fabs(v.x)+1)/2 * faceWidth,(v.y/fabs(v.x)+1)/2 * faceHeight);
			uv += faceCoords[right];
			uv.x /= width; uv.y /= height;
			return Sample2D(uv);
		}
		else if (m == v.x) {
			glm::vec2 uv((v.z / fabs(v.x) + 1) / 2 * faceWidth, (v.y / fabs(v.x) + 1) / 2 * faceHeight);
			uv += faceCoords[left];
			uv.x /= width; uv.y /= height;
			return Sample2D(uv);
		}
		else if (m == v.y) {
			glm::vec2 uv((-v.x / fabs(v.y) + 1) / 2 * faceWidth, (-v.z / fabs(v.y) + 1) / 2 * faceHeight);
			uv += faceCoords[top];
			uv.x /= width; uv.y /= height;
			return Sample2D(uv);
		}
		else if (m == -v.y) {
			glm::vec2 uv((-v.x / fabs(v.y) + 1) / 2 * faceWidth, (v.z / fabs(v.y) + 1) / 2 * faceHeight);
			uv += faceCoords[bottom];
			uv.x /= width; uv.y /= height;
			return Sample2D(uv);
		}
		else if (m == v.z) {
			glm::vec2 uv((-v.x / fabs(v.z) + 1) / 2 * faceWidth, (v.y / fabs(v.z) + 1) / 2 * faceHeight);
			uv += faceCoords[back];
			uv.x /= width; uv.y /= height;
			return Sample2D(uv);
		}
		else {
			glm::vec2 uv((v.x / fabs(v.z) + 1) / 2 * faceWidth, (v.y / fabs(v.z) + 1) / 2 * faceHeight);
			uv += faceCoords[front];
			uv.x /= width; uv.y /= height;
			return Sample2D(uv);
		}

	}


	virtual glm::vec4 Sample2D(const glm::vec2 &texcoord) {
		//GL_CLAMP_EDGE
		float x = saturate(texcoord.x);
		float y = saturate(texcoord.y);
		return GetColorf(x*width, y*height);
	}

	glm::vec4 GetColorf(int x, int y) {
		if (x <0 || x >= width || y < 0 || y>= height)
			return glm::vec4(0, 0, 0, 0);
		int xy = channels * (y * width + x);
		return glm::vec4(*(fdata + xy), *(fdata + xy + 1), *(fdata + xy + 2), 1.0);
	}
};



#endif