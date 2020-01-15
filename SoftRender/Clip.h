#ifndef CLIP_H
#define CLIP_H
#include "Globel.h"
#include "Vertex.h"


const std::vector<glm::vec4> ViewLines = {
	//Near
	glm::vec4(0,0,1,1),
	//far
	//glm::vec4(0,0,-1,1),
	//left
	glm::vec4(1,0,0,1),
	//top
	glm::vec4(0,1,0,1),
	//right
	glm::vec4(-1,0,0,1),
	//bottom 
	glm::vec4(0,-1,0,1)
};



bool Inside(const glm::vec4 &line,const glm::vec4 &p) {

	return line.x * p.x + line.y * p.y + line.z * p.z + line.w * p.w >= 0;
}

bool AllVertexsInside(const std::vector<V2F> v) {
	for (int i = 0; i < v.size(); i++) {
		if (fabs(v[i].windowPos.x) > fabs(v[i].windowPos.w))
			return false;
		if (fabs(v[i].windowPos.y) > fabs(v[i].windowPos.w))
			return false;
		if (fabs(v[i].windowPos.z) > fabs(v[i].windowPos.w))
			return false;
	}
	return true;
}

//交点，通过端点插值
V2F Intersect(const V2F &v1,const V2F &v2,const glm::vec4 &line) {
	float da = v1.windowPos.x * line.x + v1.windowPos.y * line.y + v1.windowPos.z *line.z + v1.windowPos.w * line.w;
	float db = v2.windowPos.x * line.x + v2.windowPos.y * line.y + v2.windowPos.z *line.z + v2.windowPos.w * line.w;

	float weight = da / (da-db);
	return V2F::lerp(v1, v2, weight);
}


//输入 三个顶点 输出 裁剪后的顶点组
std::vector<V2F> SutherlandHodgeman(const V2F &v1, const V2F &v2, const V2F &v3) {
	std::vector<V2F> output = {v1,v2,v3};
	if (AllVertexsInside(output)) {
		return output;
	}
	for (int i = 0; i < 5; i++) {
		std::vector<V2F> input(output);
		output.clear();
		for (int j = 0; j < input.size(); j++) {
			V2F current = input[j];
			V2F last = input[(j + input.size() - 1) % input.size()];
 			if (Inside(ViewLines[i], current.windowPos)) {
				if (!Inside(ViewLines[i],last.windowPos)) {
					V2F intersecting = Intersect(last, current, ViewLines[i]);
					output.push_back(intersecting);
				}
				output.push_back(current);
			}
			else if(Inside(ViewLines[i], last.windowPos)){
				V2F intersecting = Intersect(last, current, ViewLines[i]);
				output.push_back(intersecting);
			}
		}
	}
	return output;
}

#endif