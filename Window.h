#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "SkyBox.h"
#include "HeightMap.h"
#include "Water.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "Sphere.h"
#include "ShadowMap.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view

	static void initialize_objects();
	static void clean_up();

	static GLFWwindow* create_window(int width, int height);

	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void poll_movement();
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void Do_Movement();

	static void renderSceneObjects();
	static void renderToShadowDepthBuffer();
	static void renderToWaterRefractionBuffer();
	static void renderToWaterReflectionBuffer();
	static void renderToScreen();
};

#endif
