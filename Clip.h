#ifndef CLIP_H
#define CLIP_H

#include "Global.h"

class VertexOut;

enum FaceCullMode;

class Clip
{
public:

	static float near;
	static float far;

	static std::vector<VertexOut> SutherlandHodgeman(const VertexOut &v1, const VertexOut &v2, const VertexOut &v3);

	static bool ClipSpaceCull(const glm::vec4 &v1, const glm::vec4 &v2, const glm::vec4 &v3);

	static bool FaceCull(FaceCullMode face, const glm::vec4 &v1, const glm::vec4 &v2, const glm::vec4 &v3);
};


#endif
