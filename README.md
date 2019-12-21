# SoftRender

一个简单的软件渲染器，渲染生成颜色缓冲数组之后使用glDrawPixels绘制到屏幕上

实现功能：obj模型加载与渲染，纹理加载与渲染，Blinn-Phong光照模型，点光源/平行光/聚光灯的光照计算

运行环境： GLAD + GLFW + GLM

配置环境：https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/

!https://github.com/SilverClawko/SoftRender/blob/master/ss1.png
!https://github.com/SilverClawko/SoftRender/blob/master/ss2.png
!https://github.com/SilverClawko/SoftRender/blob/master/ss3.png

文件说明：
Globel.h 全局变量，MVP矩阵存放

Math.h 数学库，求解矩阵和插值等数学工具函数

Vertex.h 顶点的数据结构

Mesh.h 网格的数据结构

Material.h 材质的数据结构

Texture.h 纹理数据结构，从文件中加载纹理

Object.h 对象的数据结构

Model.h 模型的数据结构，能够读取.obj文件

FrameBuffer.h 帧缓冲的数据结构，保存颜色缓冲和深度缓冲

Shader.h 基础着色器

BlinnPhongShader.h 标准光照着色器

Light.h 定向光，点光源，聚光灯三种光照的数据结构与光照计算

Clip.h 用于计算多边形的视口裁剪

Camera.h FPS摄像机

Draw.h 渲染管线类，管理整个渲染流程

main.cpp 使用示例
