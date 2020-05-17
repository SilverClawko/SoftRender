#include "StateMachine.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Image.h"
#include "Transform.h"
#include "Camera.h"
#include "Graphics.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"
#include "MaterialBase.h"
#include "PBRShader.h"
#include "BlinnShader.h"
#include "ShadowShader.h"
#include "SkyBoxShader.h"

StateMachine * StateMachine::localInstance = nullptr;

StateMachine::StateMachine() :
	frontBuffer(nullptr),
	backBuffer(nullptr),
	width(0),
	height(0),
	graphics(nullptr),
	fps(0),
	triangle(0),
	camera(nullptr),
	viewPortMatrix(glm::mat4(1.0f)),
	ambient(glm::vec4(0.1f,0.1f,0.1f,1.0f)),
	modelScale(1.0f),
	skyBox(nullptr),
	currentSkybox(0),
	skyboxMap(3,nullptr),
	irradiancdeMap(3,nullptr),
	radianceMap(3,nullptr),
	faceCull(true),
	viewCull(true),
	faceCullMode(Back),
	writeDepth(true),
	writeColor(true),
	renderMode(Fill),
	renderScene(Blinn),
	drawSkyBox(false),
	drawShadow(true)
{
}

StateMachine::~StateMachine() {

}

StateMachine * StateMachine::GetInstance()
{
	if (localInstance == nullptr)
	{
		localInstance = new StateMachine();
	}
	return localInstance;
}

//初始化灯光摄像机
void StateMachine::Init(int w, int h, float fov)
{
	width = w;
	height = h;
	SetViewPortMatrix(w, h);
	frontBuffer = new FrameBuffer(w, h);
	backBuffer = new FrameBuffer(w, h);
	camera = new Camera(w, h, fov);
	DirectionLight * light = new DirectionLight();
	dirLights.push_back(light);
	models.reserve(10);
	graphics = new Graphics();
	graphics->SetRenderTarget(backBuffer);
	
	Model * floor = Model::CreateFloor(glm::vec3(0, -0.5, 0), 1.0f);
	floor->name = "floor";
	floor->meshes[0]->name = "floor_mesh";
	Texture2D * floorTex = new Texture2D("assets/container.jpg");
	floor->meshes[0]->material->SetTexture(floorTex, 0);
	delete floorTex;
	models.push_back(floor);
	
	//Mesh * box = Mesh::CreateBox(glm::vec3(0.0f), 1.0f);
	skyBox = Model::CreateSkyBox();
	skyboxMap[0] = new Texture3D("assets/skybox/valley_skybox.hdr");
	irradiancdeMap[0] = new Texture3D("assets/skybox/valley_irradiance.hdr");
	radianceMap[0] = new Texture3D("assets/skybox/valley_radiance.hdr");
	skyboxMap[1] = new Texture3D("assets/skybox/city_skybox.hdr");
	irradiancdeMap[1] = new Texture3D("assets/skybox/city_irradiance.hdr");
	radianceMap[1] = new Texture3D("assets/skybox/city_radiance.hdr");
}

void StateMachine::Resize(int w, int h)
{
	CleanUp();
	width = w;
	height = h;
	frontBuffer = new FrameBuffer(w, h);
	backBuffer = new FrameBuffer(w, h);
	SetViewPortMatrix(w, h);
}

void StateMachine::AddModel(Model * m)
{
	models.push_back(m);
}

void StateMachine::RemoveModel(int num)
{
	if (num >= models.size())
		return;
	if(models[num])
		delete models[num];
	models.erase(models.begin() + num);
}

void StateMachine::AddDirectionLight(DirectionLight * l)
{
	dirLights.push_back(l);
}

void StateMachine::ChangeRenderScene()
{
	
	mutex.lock();
	if (renderScene == Blinn) {
		renderScene = PBR;
		for (int i = 0; i < models.size(); i++) {
			for (int j = 0; j < models[i]->meshes.size(); j++) {
				models[i]->meshes[j]->material->SetShader(PBRShader::GetInstance(),1);
			}
		}
	}
	else {
		renderScene = Blinn;
		for (int i = 0; i < models.size(); i++) {
			for (int j = 0; j < models[i]->meshes.size(); j++) {
				models[i]->meshes[j]->material->SetShader(BlinnShader::GetInstance(), 1);
			}
		}
	}
	mutex.unlock();
	
}

void StateMachine::CleanUp()
{
	if (frontBuffer)
		delete frontBuffer;
	if (backBuffer)
		delete backBuffer;
	frontBuffer = nullptr;
	backBuffer = nullptr;
	width = 0;
	height = 0;
}

void StateMachine::ClearColor(int r, int g, int b, int a)
{
	backBuffer->ClearColor(r, g, b, a);
}

void StateMachine::ClearDepth(float d)
{
	backBuffer->ClearDepth(d);
}

void StateMachine::SwapBuffer()
{
	FrameBuffer * p = frontBuffer;
	frontBuffer = backBuffer;
	backBuffer = p;
	graphics->SetRenderTarget(backBuffer);
}



inline void StateMachine::SetViewPortMatrix(int w, int h)
{
	viewPortMatrix = Transform::GetViewPortMatrix(0, 0, w, h);
}

void StateMachine::DrawShadow(Light * light)
{
	mutex.lock();
	glm::mat4 globelScale = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
	if (renderMode == Fill) {
		//Draw Shadow
		for (int j = 0; j < models.size(); j++) {
			Uniform u(globelScale * models[j]->GetModelMatrix(), light->viewMatrix, light->projectorMatrix);
			graphics->DrawModel(*models[j], u, 1);
		}
	}
	mutex.unlock();
}

void StateMachine::DrawScene()
{
	mutex.lock();
	glm::mat4 globelScale = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	triangle = 0;

	for (int i = 0; i < models.size(); i++) {
		for (int j = 0; j < dirLights.size(); j++) {
			Uniform u(globelScale * models[i]->GetModelMatrix(), viewMatrix, projectionMatrix);
			u.cameraPos = glm::vec4(camera->position, 1.0f);
			u.ambient = ambient;
			u.dirLight = dirLights[j];
			u.irradiance = irradiancdeMap[currentSkybox];
			u.radiance = radianceMap[currentSkybox];
			graphics->DrawModel(*models[i], u, 2);
		}
	} 
	if (drawSkyBox) {
		faceCullMode = Front;
		writeDepth = false;
		Uniform u(glm::mat4(1.0f), viewMatrix, projectionMatrix);
		u.cubemap = skyboxMap[currentSkybox];
		graphics->DrawModel(*skyBox, u, 2);
		faceCullMode = Back;
	}
	mutex.unlock();
}


unsigned char * StateMachine::GetFrame()
{
	return frontBuffer->colorBuffer->data;
}

void StateMachine::Destroy()
{
	CleanUp();
	if (camera)
		delete camera;
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}
	for (int i = 0; i < dirLights.size(); i++) {
		delete dirLights[i];
	}
	if (skyBox)
		delete skyBox;
	for (int i = 0; i < 3; i++) {
		if (skyboxMap[i])
			delete skyboxMap[i];
		if (irradiancdeMap[i])
			delete irradiancdeMap[i];
		if (radianceMap[i])
			delete radianceMap[i];
	}
	Shader * shader = Shader::GetInstance();
	shader->Destroy();
	shader = BlinnShader::GetInstance();
	shader->Destroy();
	shader = ShadowShader::GetInstance();
	shader->Destroy();
	shader = SkyBoxShader::GetInstance();
	shader->Destroy();
	shader = PBRShader::GetInstance();
	shader->Destroy();
	camera = nullptr;
	skyBox = nullptr;
	if (localInstance)
		delete localInstance;
	localInstance = nullptr;
}


