#ifndef MODEL_H
#define MODEL_H


#include "Mesh.h"

class Material;

class Model {

public:
	glm::vec4 position;
	float pitch;
	float yaw;
	float roll;
	float scale;
	std::vector<Mesh *> meshes;
	std::string name;

	Model(Mesh * m);
	Model(const std::string &filename);
	~Model();

	glm::mat4 GetModelMatrix() const;
	void LoadObj(const std::string &filename);
	void AddMesh(Mesh * m);
	void RemoveMesh(int num);
	
	static Model * CreateFloor(const glm::vec3 &center, float radius);
	static Model * CreateSkyBox();

private:
	void BuildMesh(
		const std::vector<glm::vec3> &vertex,
		const std::vector<glm::vec3> &normal,
		const std::vector<glm::vec2> &texcoord,
		const std::vector<glm::vec4> &tangent,
		const std::vector<glm::mat3> &face
	);
};



#endif