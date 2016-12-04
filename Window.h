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
#include "C:\Users\anara\Desktop\CSE167Proj1_3\HA21\CSE167StarterCode2\OBJObject.h"
#include "shader.h"
#include "SkyBox.h"
#include "GLFWStarterProject\ShadowMap.h"
#include "GLFWStarterProject\gBuffers.h"

class Window
{
	struct CameraDirection {
		GLenum face;
		glm::vec3 target;
		glm::vec3 up;
	};

public:
	static int width;
	static int height;
	static glm::vec3 lightInvDir;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static glm::mat4 depthProjectionMatrix;
	static glm::mat4 depthViewMatrix;
	static void initialize_objects();
	static void clean_up();
	static void shadowPass();
	static void compositePass();
	static void skyboxPass();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static CameraDirection directions[6];
	static GLuint FramebufferName, depthTexture;
};

#endif
