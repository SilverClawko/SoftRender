
#include "Draw.h"
#include <iostream>
#include <thread>
#include <string>
#include <Windows.h>

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


int fps = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
	dw->Resize(width,height);
	camera->UpdateAspect(width, height);
	ViewPortMatrix = GetViewPortMatrix(0, 0, SCR_WIDTH, SCR_HEIGHT);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//dw->camera->MoveUp(yoffset * 0.1);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera->RotatePitch(5.0);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera->RotateYaw(5.0);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera->RotateYaw(-5.0);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera->RotatePitch(-5.0);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->MoveForward(0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->MoveForward(-0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->MoveRight(-0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->MoveRight(0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera->MoveUp(0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera->MoveUp(-0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		dw->changeRenderMode();
	}
}

void ShowFps(GLFWwindow * window) {

	while (1) {
		Sleep(1000);
		std::string text = "SoftRenderV0.1 fps:" + std::to_string(fps);
		glfwSetWindowTitle(window, text.c_str());
		fps = 0;
	}
}

int main() {

	//初始化
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SoftRenderV0.1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	camera = new Camera(glm::vec3(0, 0, 3),
		glm::vec3(0, 1, 0), glm::vec3(0, 0, 0),
		60.0f,
		SCR_WIDTH, SCR_HEIGHT,0.3f,100);

	dw = new Draw(SCR_WIDTH, SCR_HEIGHT);
	dw->EnableCull(Back);
	dw->changeRenderMode();


	ViewPortMatrix = GetViewPortMatrix(0,0,SCR_WIDTH,SCR_HEIGHT);

	BlinnPhongShader shader;


	/*
	Material bodyMat;
	bodyMat.SetShader(&shader);
	Texture bodyTexture("neptune\\Texf_body02.jpg");
	bodyMat.SetTexture(&bodyTexture);

	Material faceMat;
	faceMat.SetShader(&shader);
	Texture faceTexture("neptune\\Tex002f_body01.jpg");
	faceMat.SetTexture(&faceTexture);

	Material mouseMat;
	mouseMat.SetShader(&shader);
	Texture mouseTexture("neptune\\Texf_mouse.jpg");
	mouseMat.SetTexture(&mouseTexture);

	Material eyeMat;
	eyeMat.SetShader(&shader);
	Texture eyeTexture("neptune\\Tex001f_eye.jpg");
	eyeMat.SetTexture(&eyeTexture);


	
	Model model("neptune\\neptune.obj");
	model.SetMaterial(0, mouseMat);
	model.SetMaterial(1, faceMat);
	model.SetMaterial(2, bodyMat);
	model.SetMaterial(3, eyeMat);
	*/
	SkyBoxShader sks;
	CubeMap hdr("MonValley_G_DirtRoad\\output_skybox.hdr");
	sks.SetCubeMap(&hdr);
	Mesh box = CreateBox(glm::vec3(0.0), 1);
	Material hdrMat;
	hdrMat.SetShader(&sks);
	Object SkyBox(box, hdrMat);

	PBRShader pbr;
	CubeMap irr("MonValley_G_DirtRoad\\output_iem.hdr");
	pbr.SetIrradianceMap(&irr);
	pbr.SetReflectionMap1(&hdr);
	pbr.SetReflectionMap2(&irr);
	Texture brdf("ibl_brdf_lut.png");
	pbr.SetBRDFLUT(&brdf);
	Model wBox("box\\Wooden_stuff.obj");
	Material wBoxMat;
	wBoxMat.SetShader(&pbr);
	Texture wBoxTexture("box\\Wooden_box_01_BaseColor.png");
	wBoxMat.SetAlbedo(&wBoxTexture);
	Texture wBoxNormal("box\\Wooden_box_01_Normal.png");
	wBoxMat.SetNormalMap(&wBoxNormal);
	Texture wBoxMetallic("box\\Wooden_box_01_Metallic.png");
	wBoxMat.SetMetallicMap(&wBoxMetallic);
	Texture wBoxRoughness("box\\Wooden_box_01_Roughness.png");
	wBoxMat.SetRoughnessMap(&wBoxRoughness);
	Texture wBoxAo("box\\Wooden_box_01_AO.png");
	wBoxMat.SetAOMap(&wBoxAo);
	wBox.SetMaterial(0, wBoxMat);

	
	Model gun("gun\\Cerberus_LP.obj");
	Material gunMat;
	gunMat.SetShader(&pbr);
	Texture gunTexture("gun\\Cerberus_A.png");
	gunMat.SetAlbedo(&gunTexture);
	Texture gunNormal("gun\\Cerberus_N.png");
	gunMat.SetNormalMap(&gunNormal);
	Texture gunMetallic("gun\\Cerberus_M.png");
	gunMat.SetMetallicMap(&gunMetallic);
	Texture gunRoughness("gun\\Cerberus_R.png");
	gunMat.SetRoughnessMap(&gunRoughness);
	gun.SetMaterial(0, gunMat);
	



	//Mesh p = CreatePlane(glm::vec3(-1,0,-1),glm::vec3(-1,0,1),glm::vec3(1,0,1),glm::vec3(1,0,-1),glm::vec3(0,1,0));
	//Texture pt("container.jpg");
	//Material pm;
	//pm.SetShader(&shader);
	//pm.SetAlbedo(&pt);
	//Object plane(box,pm);


	DirectionLight dir(glm::vec3(0,0,-1),glm::vec3(1,1,1),glm::vec3(1,1,1),1.0);
	dirLights = &dir;
	dirLtNums = 1;
	PointLight pt(glm::vec3(1,1,1));
	ptLights = &pt;
	ptLtNums = 1;
	SpotLight sp(camera->Position,camera->Front);
	spLights = &sp;
	spLtNums = 1;

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//加载opengl函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	//Mesh tri = CreateTriangle(glm::vec3(0,2,0),glm::vec3(2,0,0),glm::vec3(-2,0,0));


	std::thread t(ShowFps,window);
	t.detach();



while (!glfwWindowShouldClose(window))
{
	processInput(window);
	dw->ClearBuffer(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

	ViewMatrix = camera->ViewMatrix();
	ProjectMatrix = camera->PerspectiveMatrix();
	dw->UpdateViewPlanes();
	sp.Position = camera->Position;
	sp.Direction = camera->Front;


	
	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0, 0, 1));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.01, 0.01, 0.01));
	//ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3, 3, 3));
	UpdateNormalMatrix();
	dw->EnableDepthWrite();
	dw->EnableCull(Back);
	//dw->DrawObject(plane);
	dw->DrawModel(gun);

	
	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1, 0, 0));
	UpdateNormalMatrix();
	dw->EnableDepthWrite();
	dw->EnableCull(Back);
	dw->DrawModel(wBox);
	
	
	ModelMatrix = glm::mat4(1.0f);
	UpdateNormalMatrix();
	dw->EnableCull(Front);
	dw->DisableDepthWrite();
	dw->DrawObject(SkyBox);
	
	dw->Show();
	fps++;

	glfwSwapBuffers(window);
	glfwPollEvents();
}
	glfwTerminate();
	return 0;
}

