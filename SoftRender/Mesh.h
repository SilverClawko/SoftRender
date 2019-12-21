#pragma once

#ifndef MESH_H
#define MESH_H

#include "Vertex.h"

//网格
//基础单位是三角形

class Mesh
{
public:
	std::vector<Vertex> VBO;
	std::vector<unsigned int> EBO;

	Mesh() = default;
	Mesh(const int & vNum,const int & iNum) {
		VBO.resize(vNum);
		EBO.resize(iNum);
	}
	~Mesh() = default;

	Mesh(const Mesh& mesh)
		:VBO(mesh.VBO), EBO(mesh.EBO) {}

	Mesh& operator=(const Mesh& mesh)
	{
		if (&mesh == this)
			return *this;
		VBO = mesh.VBO;
		EBO = mesh.EBO;
		return *this;
	}
	Mesh& operator+=(const Mesh& mesh)
	{
		AddMesh(mesh);
		return *this;
	}
	void AddMesh(const Mesh & mesh) {
		int offset = VBO.size();
		VBO.insert(VBO.end(),mesh.VBO.begin(), mesh.VBO.end());
		EBO.reserve(EBO.size() + mesh.EBO.size());
		for (int i = 0; i < mesh.EBO.size(); i++) {
			EBO.push_back(mesh.EBO[i] + offset);
		}
	}

	void AddTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) {
		VBO.push_back(v1);
		VBO.push_back(v2);
		VBO.push_back(v3);
		EBO.push_back(0);
		EBO.push_back(1);
		EBO.push_back(2);
	}
};

	//存一个三角形
	Mesh CreateTriangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3) {
		Mesh result(3,3);
		//逆时针的三角形
		//顶点顺序 0 1 2
		//纹理坐标 (0,0) (1,0) (0.5,1)
		result.VBO[0].position = glm::vec4(p1, 1.0f);
		result.VBO[0].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		result.VBO[0].color = glm::vec4(255,0,0,255);
		result.VBO[0].texcoord = glm::vec2(0.0f, 0.0f);
		result.VBO[1].position = glm::vec4(p2, 1.0f);
		result.VBO[1].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		result.VBO[1].color = glm::vec4(0,255,0,255);
		result.VBO[1].texcoord = glm::vec2(1.0f, 0.0f);
		result.VBO[2].position = glm::vec4(p3, 1.0f);
		result.VBO[2].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		result.VBO[2].color = glm::vec4(0,0,255,255);
		result.VBO[2].texcoord = glm::vec2(0.5f, 1.0f);
		result.EBO[0] = 0;
		result.EBO[1] = 1;
		result.EBO[2] = 2;

		return result;
	}

	//左上 左下 右下 右上
	Mesh CreatePlane(const glm::vec3 &leftTop,const glm::vec3 &leftBottom,const glm::vec3 & rightBottom,const glm::vec3 &rightTop,const glm::vec3 &normal){
		Mesh result(4,6);
		result.VBO[0].position = glm::vec4(leftTop,1.0f);
		result.VBO[0].normal = normal;
		result.VBO[0].color = glm::vec4(1.0f, 0,0,1.0f);
		result.VBO[0].texcoord = glm::vec2(0.0f, 1.0f);
		result.VBO[1].position = glm::vec4(rightTop,1.0f);
		result.VBO[1].normal = normal;
		result.VBO[1].color = glm::vec4(0,1.0f,0,1.0);
		result.VBO[1].texcoord = glm::vec2(1.0f, 1.0f);
		result.VBO[2].position = glm::vec4(rightBottom,1.0f);
		result.VBO[2].normal = normal;
		result.VBO[2].color = glm::vec4(0,0,1.0f,0);
		result.VBO[2].texcoord = glm::vec2(1.0f, 0.0f);
		result.VBO[3].position = glm::vec4(leftBottom,1.0f);
		result.VBO[3].normal = normal;
		result.VBO[3].color = glm::vec4(1.0f,0,1.0,1.0f);
		result.VBO[3].texcoord = glm::vec2(0.0f, 0.0f);
		result.EBO[0] = 0;
		result.EBO[1] = 2;
		result.EBO[2] = 1;
		result.EBO[3] = 0;
		result.EBO[4] = 3;
		result.EBO[5] = 2;
		return result;
	}

	Mesh CreateBox(const glm::vec3 &center,float radius) {
		Mesh result;
		
		Mesh front = CreatePlane(
			center + glm::vec3(-radius, radius, radius), 
			center + glm::vec3(-radius, -radius, radius),
			center + glm::vec3(radius, -radius, radius),
			center + glm::vec3(radius, radius, radius),
			glm::vec3(0,0,1)
		);
		result.AddMesh(front);
		
		Mesh left = CreatePlane(
			center + glm::vec3(-radius, radius, -radius), 
			center + glm::vec3(-radius, -radius, -radius),
			center + glm::vec3(-radius, -radius, radius), 
			center + glm::vec3(-radius, radius, radius),
			glm::vec3(-1,0,0)
		);
		result.AddMesh(left);
		
		Mesh right = CreatePlane(
			center + glm::vec3(radius, radius, radius),
			center + glm::vec3(radius, -radius, radius),
			center + glm::vec3(radius, -radius, -radius),
			center + glm::vec3(radius, radius, -radius),
			glm::vec3(1,0,0)
		);
		result.AddMesh(right);
		
		Mesh back = CreatePlane(
			center + glm::vec3(radius, radius, -radius),
			center + glm::vec3(radius, -radius, -radius),
			center + glm::vec3(-radius, -radius, -radius),
			center + glm::vec3(-radius, radius, -radius),
			glm::vec3(0,0,-1)
		);
		result.AddMesh(back);
		
		Mesh up = CreatePlane(
			center + glm::vec3(-radius, radius, -radius),
			center + glm::vec3(-radius, radius, radius),
			center + glm::vec3(radius, radius, radius),
			center + glm::vec3(radius, radius, -radius),
			glm::vec3(0,1,0)
		);
		result.AddMesh(up);
		
		Mesh down = CreatePlane(
			center + glm::vec3(-radius, -radius, radius),
			center + glm::vec3(-radius, -radius, -radius),
			center + glm::vec3(radius, -radius, -radius),
			center + glm::vec3(radius, -radius, radius),
			glm::vec3(0,-1,0)
		);
		result.AddMesh(down);
		
		return result;
	}

#endif