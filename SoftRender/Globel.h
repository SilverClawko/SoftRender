#ifndef GLOBEL_H
#define GLOBEL_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>

glm::mat4 ModelMatrix;
glm::mat4 ViewMatrix;
glm::mat4 ProjectMatrix;
glm::mat4 ViewPortMatrix;
//∑®œﬂ±‰ªªæÿ’Û
glm::mat3 NormalMatrix;

const float PI = 3.14159265359;
const glm::vec3 Ambient = glm::vec3(0.5,0.5,0.5);

class Camera;
class Draw;
class Material;

class DirectionLight;
class SpotLight;
class PointLight;

DirectionLight * dirLights;
int dirLtNums = 0;
SpotLight * spLights;
int spLtNums = 0;
PointLight * ptLights;
int ptLtNums = 0;


Camera * camera;
Draw * dw;
Material * currentMat;


#endif