#ifndef CLIP_H
#define CLIP_H
#include "Globel.h"
#include "Vertex.h"

//前后 左上右下 
// Ax + By + C < 0 就在内侧
const std::vector<glm::vec4> ViewLines = {

	//Near z+w > 0
	glm::vec4(0,0,1,1),
	//left x+w > 0 
	glm::vec4(1,0,0,1),
	//top -y+w > 0
	glm::vec4(0,1,0,1),
	//right -x+w < 0
	glm::vec4(-1,0,0,1),
	//bottom -y+w > 0
	glm::vec4(0,-1,0,1)
};
float Near = 0.3f;


bool Inside(const glm::vec4 &line,const glm::vec4 &p) {
	float sb =line.x * p.x  + line.y * p.y  + line.z * p.z + line.w * p.w;
	return sb > 0;
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
	float da = v1.windowPos.x * line.x + v1.windowPos.y * line.y + v1.windowPos.z *line.z + line.w * v1.windowPos.w;
	float db = v2.windowPos.x * line.x + v2.windowPos.y * line.y + v2.windowPos.z *line.z + line.w * v2.windowPos.w;

	float weight = da / (da-db);
	return V2F::lerp(v1, v2, weight);
}


//输入 三个顶点 输出 裁剪后的顶点组
//注意 顶点的时针顺序要跟裁剪边的遍历顺序一致

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