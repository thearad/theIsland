#include "window.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;
SkyBox * skybox;
HeightMap* heightmap;
Water* water;

GLint shaderProgram;
GLint skyboxShaderProgram;
GLint normalsShaderProgram;
GLint waterShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shaders/heightmap.vert"
#define FRAGMENT_SHADER_PATH "../shaders/heightmap.frag"

#define SKY_VERTEX_SHADER_PATH "../shaders/skyboxShader.vert"
#define SKY_FRAGMENT_SHADER_PATH "../shaders/skyboxShader.frag"
#define SKYBOX_FACE_DIR "../skybox/"

#define WATER_SHADER_PATH "../shaders/shader"
//std::string SKYBOX_DIR = "../skybox/";

Camera camera(glm::vec3(0.0f, 5.0f, 30.0f));
bool keys[1024];
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

bool Window::lbutton_down;
bool Window::first_time;
glm::vec3 Window::lastPoint;

void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShaderProgram = LoadShaders(SKY_VERTEX_SHADER_PATH, SKY_FRAGMENT_SHADER_PATH);
	normalsShaderProgram = LoadShaders("../shaders/normals.vert", "../shaders/normals.frag", "../shaders/normals.gs");
	waterShaderProgram = LoadShaders(WATER_SHADER_PATH ".vert", WATER_SHADER_PATH ".frag");

	//Render objects
	cube = new Cube();
	water = new Water(200, 200);

	std::vector<const GLchar*> faces = {
		SKYBOX_FACE_DIR "right.jpg", SKYBOX_FACE_DIR "left.jpg", SKYBOX_FACE_DIR "top.jpg",
		SKYBOX_FACE_DIR "bottom.jpg", SKYBOX_FACE_DIR "back.jpg", SKYBOX_FACE_DIR "front.jpg"
	};
	skybox = new SkyBox(faces);

	heightmap = new HeightMap(200, 200);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(shaderProgram);
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
		V = camera.GetViewMatrix();
		P = glm::perspective(camera.Zoom, (float)width / (float)height, 0.1f, 1000.0f);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
}

glm::vec4 refract_clip = glm::vec4(0.f, -1.f, 0.f, 0.01f);
glm::vec4 reflect_clip = glm::vec4(0.f, 1.f, 0.f, -0.01f);
void Window::display_callback(GLFWwindow* window)
{
	//ENABLE PLANE CLIPPING FOR WATER REFLECTION/REFRACTION
	glEnable(GL_CLIP_DISTANCE0);
	
	//DUMMY RENDER------------------------------------------------------------------------
	/*WEIRD....VERY WEIRD issue where the first pass rendering NEVER gets rendered...*/
	water->bindFrameBuffer(Water::REFRACTION);
	glUseProgram(skyboxShaderProgram);
	skybox->draw(skyboxShaderProgram);

	glUseProgram(shaderProgram);
	heightmap->draw(shaderProgram);

	////WATER REFRACTION--------------------------------------------------------------------------------------
	water->bindFrameBuffer(Water::REFRACTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform4f(glGetUniformLocation(shaderProgram, "clippingPlane"), refract_clip.x, refract_clip.y, refract_clip.z, refract_clip.w);
	glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), refract_clip.x, refract_clip.y, refract_clip.z, refract_clip.w);

	glUseProgram(skyboxShaderProgram);
	skybox->draw(skyboxShaderProgram);

	glUseProgram(shaderProgram);
	heightmap->draw(shaderProgram);

	//DISABLE PLANE CLIPPING FOR NORMAL RENDERING PASS
	V = camera.GetViewMatrix();
	
	//WATER REFLECTION------------------------------------------------------------------------
	water->bindFrameBuffer(Water::REFLECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float dist = 2 * camera.Position.y - 0.01f;
	camera.Position.y -= dist;
	camera.Pitch *= -1;
	camera.updateCameraVectors();
	V = camera.GetViewMatrix();

	glUniform4f(glGetUniformLocation(shaderProgram, "clippingPlane"), reflect_clip.x, reflect_clip.y, reflect_clip.z, reflect_clip.w);
	glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), reflect_clip.x, reflect_clip.y, reflect_clip.z, reflect_clip.w);

	glUseProgram(skyboxShaderProgram);
	skybox->draw(skyboxShaderProgram);

	glUseProgram(shaderProgram);
	heightmap->draw(shaderProgram);

	camera.Position.y += dist;
	camera.Pitch *= -1;
	camera.updateCameraVectors();
	V = camera.GetViewMatrix();

	//THIRD PASS ... RENDER SCENE NORMALLY----------------------------------------------------------------------------
	water->unbindFrameBuffer();
	glUniform4f(glGetUniformLocation(shaderProgram, "clippingPlane"), 0, 0, 0, 0);
	glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), 0, 0, 0, 0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3f(glGetUniformLocation(waterShaderProgram, "camera_Position"), camera.Position.x, camera.Position.y, camera.Position.z);
	
	glUseProgram(skyboxShaderProgram);
	skybox->draw(skyboxShaderProgram);

	glUseProgram(shaderProgram);
	heightmap->draw(shaderProgram);
	
	glUseProgram(waterShaderProgram);
	water->draw(waterShaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	Do_Movement();

	V = camera.GetViewMatrix();
	P = glm::perspective(camera.Zoom, (float)width / (float)height, 0.1f, 1000.0f);
	// Swap buffers
	glfwSwapBuffers(window);
}

bool escape_camera = false;
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
	if (action == GLFW_PRESS && mods == GLFW_MOD_ALT) {
		escape_camera = !escape_camera;
	};
	if (action == GLFW_PRESS && key == GLFW_KEY_R) {
		heightmap->refresh();
	};
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}
void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action) {
			lbutton_down = true;
		}
		else if (GLFW_RELEASE == action) {
			lbutton_down = false;
			first_time = true;
		}
	}
}

void Window::Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (first_time)
	{
		lastPoint.x = xpos;
		lastPoint.y = ypos;
		first_time = false;
	}

	GLfloat xoffset = xpos - lastPoint.x;
	GLfloat yoffset = lastPoint.y - ypos;  // Reversed since y-coordinates go from bottom to left

	lastPoint.x = xpos;
	lastPoint.y = ypos;
	if (escape_camera) return;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset/10.f);
}