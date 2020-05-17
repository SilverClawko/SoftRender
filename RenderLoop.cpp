#include "RenderLoop.h"
#include "Graphics.h"
#include "Vertex.h"
#include "StateMachine.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Texture.h"
#include "MaterialBase.h"
#include "Model.h"
#include "Light.h"


RenderLoop::RenderLoop(QObject *parent)
	: QObject(parent), scale(1.0), sys(nullptr)
{
	sys = StateMachine::GetInstance();
}

RenderLoop::~RenderLoop()
{
}

void RenderLoop::Stop() {
	shouldClosed = true;
}


void RenderLoop::Zoom(float delta)
{
	scale += delta;
	if (scale < 0.1)
		scale = 0.1;
	if (scale > 10)
		scale = 10;
	sys->modelScale = scale;
	//sys->modelMatrix[3][3] = 1;
}

void RenderLoop::MainLoop()
{
	//Mesh box = Mesh::CreateBox(glm::vec3(0.5, 0, 0), 0.25);
	//Texture2D * tex = new Texture2D("container.jpg");
	//box.material->SetMainTex(tex);
	//Model wBox("box\\Wooden_stuff.obj");
	//Texture2D * wBoxT = new Texture2D("box\\Wooden_box_01_BaseColor.png");
	//wBox.meshes[0]->material->SetMainTex(wBoxT);
	/*
	Model * neptune = new Model("neptune/neptune.obj");
	neptune->scale = 0.005;
	Texture2D * mouse = new Texture2D("neptune/Texf_mouse.jpg");
	Texture2D * face = new Texture2D("neptune/Tex002f_body01.jpg");
	Texture2D * body = new Texture2D("neptune/Texf_body02.jpg");
	Texture2D * eye = new Texture2D("neptune/Tex001f_eye.jpg");
	neptune->meshes[0]->material->SetTexture(mouse, 0);
	neptune->meshes[1]->material->SetTexture(face, 0);
	neptune->meshes[2]->material->SetTexture(body, 0);
	neptune->meshes[3]->material->SetTexture(eye, 0);
	delete mouse;
	delete face;
	delete body;
	delete eye;
	*/
	/*
	Model * wBox = new Model("box/Wooden_stuff.obj");
	Texture2D * mt = new Texture2D("box/Wooden_box_01_BaseColor.png");
	Texture2D * nl = new Texture2D("box/Wooden_box_01_Normal.png");
	Texture2D * ml = new Texture2D("box/Wooden_box_01_Metallic.png");
	Texture2D * rn = new Texture2D("box/Wooden_box_01_Roughness.png");
	Texture2D * ao = new Texture2D("box/Wooden_box_01_AO.png");

	wBox->meshes[0]->material->SetTexture(mt, 0);
	wBox->meshes[0]->material->SetTexture(nl, 1);
	wBox->meshes[0]->material->SetTexture(ml, 2);
	wBox->meshes[0]->material->SetTexture(rn, 3);
	wBox->meshes[0]->material->SetTexture(ao, 4);
	delete mt;
	delete nl;
	delete ml;
	delete rn;
	delete ao;
	*/
	Model * helmet = new Model("assets/helmet/helmet.obj");
	Texture2D * mt = new Texture2D("assets/helmet/helmet_basecolor.tga");
	Texture2D * nl = new Texture2D("assets/helmet/helmet_normal.tga");
	Texture2D * ml = new Texture2D("assets/helmet/helmet_metalness.tga");
	Texture2D * rn = new Texture2D("assets/helmet/helmet_roughness.tga");
	Texture2D * ao = new Texture2D("assets/helmet/helmet_occlusion.tga");
	helmet->meshes[0]->material->SetTexture(mt, 0);
	helmet->meshes[0]->material->SetTexture(nl, 1);
	helmet->meshes[0]->material->SetTexture(ml, 2);
	helmet->meshes[0]->material->SetTexture(rn, 3);
	helmet->meshes[0]->material->SetTexture(ao, 4);
	sys->AddModel(helmet);
	//先存object 再统一画
	//先shadow pass
	while (!shouldClosed) {

		//shadowmap
		if (sys->drawShadow) {
			sys->SetViewPortMatrix(1024, 1024);
			sys->writeColor = false;
			sys->writeDepth = true;
			sys->faceCullMode = Front;
			for (int i = 0; i < sys->dirLights.size(); i++) {
				sys->dirLights[i]->SetShadowMap(nullptr);
				FrameBuffer * SBO = new FrameBuffer(1024, 1024);
				sys->graphics->SetRenderTarget(SBO);
				SBO->ClearDepth(1.0f);
				sys->DrawShadow(sys->dirLights[i]);
				sys->dirLights[i]->SetShadowMap(new Texture2D(SBO->depthBuffer));
				delete SBO;
			}
		}
		sys->SetViewPortMatrix(sys->width, sys->height);
		sys->writeColor = true;
		sys->writeDepth = true;
		sys->faceCullMode = Back;
		sys->graphics->SetRenderTarget(sys->backBuffer);
		sys->ClearColor(0, 0, 0);
		sys->ClearDepth(1.0f);
		sys->DrawScene();

		sys->SwapBuffer();
		emit Vsync(sys->GetFrame() ,0,0);
		sys->fps++;
	}
}
