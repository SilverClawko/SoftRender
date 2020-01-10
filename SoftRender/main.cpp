
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

	camera = new Camera(glm::vec3(0, 0, 5),
		glm::vec3(0, 1, 0), glm::vec3(0, 0, 0),
		60.0f,
		SCR_WIDTH, SCR_HEIGHT);

	dw = new Draw(SCR_WIDTH, SCR_HEIGHT);
	dw->EnableCull(Back);



	ViewPortMatrix = GetViewPortMatrix(0,0,SCR_WIDTH,SCR_HEIGHT);

	BlinnPhongShader shader;



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

	PBRShader pbr;
	Model wBox("box\\Wooden_stuff.obj");
	Material wBoxMat;
	wBoxMat.SetShader(&pbr);
	Texture wBoxTexture("box\\Wooden_box_01_BaseColor.png");
	wBoxMat.SetTexture(&wBoxTexture);
	Texture wBoxNormal("box\\Wooden_box_01_Normal.png");
	wBoxMat.SetNormalMap(&wBoxNormal);
	Texture wBoxHeight("box\\Wooden_box_01_Height.png");
	wBoxMat.SetHeightMap(&wBoxHeight);
	wBox.SetMaterial(0, wBoxMat);


	Mesh box = CreateBox(glm::vec3(1, 0.0, -1), 0.5);
	Material mat;
	mat.SetShader(&shader);
	Texture boxt("container.jpg");
	mat.SetTexture(&boxt);
	Object obj(box,mat);

	DirectionLight dir(glm::vec3(-1,-1,-1));
	dirLights = &dir;
	dirLtNums = 1;
	//PointLight pt(glm::vec3(1,1,1));
	//shader.SetPointLight(&pt);
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
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1, 0, 0));
//	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100, 100, 100));
	UpdateNormalMatrix();
	dw->DrawModel(wBox);

	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1, 0, 0));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.01, 0.01, 0.01));
	UpdateNormalMatrix();
	dw->DrawModel(model);


	//ModelMatrix = glm::mat4(1.0f);
	//UpdateNormalMatrix();
	//ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0));

	//dw->DrawObject(ground);
	
	//dw->DrawObject(Lamp);
	dw->Show();
	fps++;

	glfwSwapBuffers(window);
	glfwPollEvents();
}
	glfwTerminate();
	return 0;
}

