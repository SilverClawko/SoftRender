#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Global.h"


class FrameBuffer;
class Vertex;
class VertexOut;
class StateMachine;
class Shader;
class Mesh;
class Model;
class Uniform;

class Graphics
{
private:
	FrameBuffer * renderTarget;
	Shader * shader;

public:
	StateMachine * sys;
	
	Graphics();
	~Graphics();


	void SetRenderTarget(FrameBuffer * i);

	void DrawModel(const Model &model, Uniform &u, int pass);

	void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);
	//void RasterizeTriangle(int minX,int maxX,int minY,int maxY, const VertexOut &v1, const VertexOut &v2, const VertexOut &v3);
	void RasterizeTriangle(const VertexOut &v1, const VertexOut &v2, const VertexOut &v3);
	void DrawLine(const VertexOut &from, const VertexOut &to);
	void PerspectiveRestore(VertexOut &v);
	void PerspectiveDivision(VertexOut &v);
	void ViewPortMapping(VertexOut &v);

	glm::vec3 CalcWeights(const glm::vec2 &v1, const glm::vec2 &v2, const glm::vec2 &v3);

};


#endif