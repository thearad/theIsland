#include "window.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;
OBJObject * bunny;
SkyBox * skybox;
gBuffers * gBuffer;

GLuint shaderProgram;
GLuint skyboxShaderProgram;
GLuint depthShaderProgram;
GLuint shadowShaderProgram;
GLuint compShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

#define DEPTH_VERTEX_SHADER_PATH "depth.vert"
#define DEPTH_FRAGMENT_SHADER_PATH "depth.frag"

#define SHADOW_VERTEX_SHADER_PATH "shadow.vert"
#define SHADOW_FRAGMENT_SHADER_PATH "shadow.frag"

#define COMP_VERTEX_SHADER_PATH "composite.vert"
#define COMP_FRAGMENT_SHADER_PATH "composite.frag"

#define SKY_VERTEX_SHADER_PATH "../skyboxShader.vert"
#define SKY_FRAGMENT_SHADER_PATH "../skyboxShader.frag"
#define SKYBOX_FACE_DIR "../skybox/"
//std::string SKYBOX_DIR = "../skybox/";


// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
GLuint Window::FramebufferName;
GLuint Window::depthTexture;
Window::CameraDirection Window::directions[6];
ShadowMap * dLightShadow;
glm::mat4 Window::depthViewMatrix, Window::depthProjectionMatrix;
glm::vec3 Window::lightInvDir = glm::vec3(2, 0, 2);
//glm::vec4 lightDir = glm::vec4(1, 1, 1, 0);
glm::mat4 Window::P;
glm::mat4 Window::V;

struct BaseLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	BaseLight Base;
	glm::vec3 Direction;
};

void Window::initialize_objects()
{
	//cube = new Cube();
	dLightShadow = new ShadowMap(Window::width, Window::height);
	gBuffer = new gBuffers(Window::width, Window::height);
	//dLightShadow = ShadowMap(2048,2048);
	bunny = new OBJObject("bunny.obj", glm::vec4(0.6, 1, 1, 1.0), 0.5f, glm::vec3(0.9, 0.7, 0.1));
	std::vector<const GLchar*> faces;
	faces.push_back(SKYBOX_FACE_DIR "right.jpg");
	faces.push_back(SKYBOX_FACE_DIR "left.jpg");
	faces.push_back(SKYBOX_FACE_DIR "top.jpg");
	faces.push_back(SKYBOX_FACE_DIR "bottom.jpg");
	faces.push_back(SKYBOX_FACE_DIR "back.jpg");
	faces.push_back(SKYBOX_FACE_DIR "front.jpg");
	skybox = new SkyBox(faces);

	depthProjectionMatrix = glm::ortho<float>(-100, 100, -100, 100, -10, 100);//maybebe wrong....fov should be 90 for cubemaps? But ortho?
	//glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	directions[0] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) };
	directions[1] = { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) };
	directions[2] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) };
	directions[3] = { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
	directions[4] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) };
	directions[5] = { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) };

	depthShaderProgram = LoadShaders(DEPTH_VERTEX_SHADER_PATH, DEPTH_FRAGMENT_SHADER_PATH);

	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	shadowShaderProgram = LoadShaders(SHADOW_VERTEX_SHADER_PATH, SHADOW_FRAGMENT_SHADER_PATH);
	compShaderProgram = LoadShaders(COMP_VERTEX_SHADER_PATH, COMP_FRAGMENT_SHADER_PATH);
	skyboxShaderProgram = LoadShaders(SKY_VERTEX_SHADER_PATH, SKY_FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShaderProgram);
	glDeleteProgram(shadowShaderProgram);
	glDeleteProgram(depthShaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(shadowShaderProgram);
	//gBuffer->bindDraw();
	//gBuffer->setDrawBuffers();
	//bunny->draw(shadowShaderProgram);
	
	//gBuffer->bindDraw();
	//gBuffer->setDrawLight();
	//glClear(GL_COLOR_BUFFER_BIT);
	//gBuffer->unbindDraw();

	shadowPass();
	//compositePass();
	//skyboxPass();
	//glUseProgram(skyboxShaderProgram);//SKYBOX OR DEPTH
	//skybox->draw(skyboxShaderProgram);
	//glUseProgram(shaderProgram);
	//bunny->draw(shaderProgram);
	
	
	//glUseProgram(0);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::shadowPass() {
	//glViewport(0, 0, 2048, 2048);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	dLightShadow->bind();
	glUseProgram(depthShaderProgram);
	/*glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < 6; i++) {*/
		
	depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;
	GLuint depthMatrixID = glGetUniformLocation(depthShaderProgram, "depthMVP");
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	skybox->quickDraw();
	bunny->quickDraw();
	
	dLightShadow->unbind();
	glDisable(GL_CULL_FACE);
	//glDepthMask(GL_FALSE);

	
}

void Window::compositePass() {
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(compShaderProgram);
	
	dLightShadow->bindRead(GL_TEXTURE1);

	GLuint invVID = glGetUniformLocation(compShaderProgram, "gEyeWorldPos");
	glUniform3fv(invVID, 1, &cam_pos[0]);
	
	glUniform1f(glGetUniformLocation(compShaderProgram, "gMatSpecularIntensity"), 0.3f);
	glUniform1f(glGetUniformLocation(compShaderProgram, "gSpecularPower"), 1.0f); 


	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, gBuffer->position);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, gBuffer->normal);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, gBuffer->color);
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, dLightShadow->depth);
	
	//glUniform1i(glGetUniformLocation(compShaderProgram, "positionMap"), 0);
	//glUniform1i(glGetUniformLocation(compShaderProgram, "normalMap"), 1);
	//glUniform1i(glGetUniformLocation(compShaderProgram, "colorMap"), 2);
	//glUniform1i(glGetUniformLocation(compShaderProgram, "shadowMap"), 5);

	//glUniform3fv(glGetUniformLocation(compShaderProgram,"l"),1, &glm::vec3(Window::V * glm::vec4(lightInvDir,0))[0]);
	depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;
	GLuint depthMatrixID = glGetUniformLocation(compShaderProgram, "gLightWVP");
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	
	//glUniform1i(glGetUniformLocation(compShaderProgram, "shadowMapWidth"), 2048);
	//glUniform1i(glGetUniformLocation(compShaderProgram, "shadowMapHeight"), 2048);
	//glUniform1i(glGetUniformLocation(compShaderProgram, "type"), 7);//0 - 7
	//finalPass.setUniformi("type", type);
	//glEnable(GL_FRAMEBUFFER_SRGB);

	glClear(GL_COLOR_BUFFER_BIT);

	bunny->draw(compShaderProgram);

}

void Window::skyboxPass() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glUseProgram(skyboxShaderProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	skybox->draw(skyboxShaderProgram);
	glDisable(GL_DEPTH_TEST);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}