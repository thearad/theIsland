#include "window.h"

const char* window_title = "Island";
Cube * cube;
SkyBox * skybox;
HeightMap* heightmap;
Water* water;
//Particles* p_mgr;
ParticleManager* p_mgr;

GLint heightmapShaderProgram;
GLint skyboxShaderProgram;
GLint normalsShaderProgram;
GLint waterShaderProgram;
GLint particleShaderProgram;

// On some systems you need to change this to the absolute path
#define SHADER_PATH "../shaders/"
#define SKYBOX_FACE_DIR "../skybox/"

Camera camera(glm::vec3(0.0f, 5.0f, 50.0f));
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
	heightmapShaderProgram = LoadShaders(SHADER_PATH "heightmap.vert", SHADER_PATH "heightmap.frag");
	skyboxShaderProgram = LoadShaders(SHADER_PATH "skybox.vert", SHADER_PATH "skybox.frag");
	normalsShaderProgram = LoadShaders(SHADER_PATH "normals.vert", SHADER_PATH "normals.frag", SHADER_PATH "normals.gs");
	waterShaderProgram = LoadShaders(SHADER_PATH "water.vert", SHADER_PATH "water.frag");
	//particleShaderProgram = LoadShaders(SHADER_PATH "particles.vert", SHADER_PATH "particles.frag");
	particleShaderProgram = LoadShaders(SHADER_PATH "particles_1.vert", SHADER_PATH "particles_1.frag");

	//Render objects
	cube = new Cube();
	water = new Water(200, 200);

	std::vector<const GLchar*> faces = {
		SKYBOX_FACE_DIR "right.jpg", SKYBOX_FACE_DIR "left.jpg", SKYBOX_FACE_DIR "top.jpg",
		SKYBOX_FACE_DIR "bottom.jpg", SKYBOX_FACE_DIR "back.jpg", SKYBOX_FACE_DIR "front.jpg"
	};
	skybox = new SkyBox(faces);

	heightmap = new HeightMap(200, 200);

	//p_mgr = new Particles(particleShaderProgram);
	p_mgr = new ParticleManager(particleShaderProgram);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(heightmapShaderProgram);
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

//renders objects on the scene that require no multirendering
void Window::render_scene() {
	glUseProgram(skyboxShaderProgram);
	skybox->draw(skyboxShaderProgram);

	//glUseProgram(heightmapShaderProgram);
	//heightmap->draw(heightmapShaderProgram);
}

//polls for WASD movements
void Window::poll_movement() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	Do_Movement();
}

glm::vec4 refract_clip = glm::vec4(0.f, -1.f, 0.f, 0.01f);
glm::vec4 reflect_clip = glm::vec4(0.f, 1.f, 0.f, -0.01f);
void Window::display_callback(GLFWwindow* window)
{
	////ENABLE PLANE CLIPPING FOR WATER REFLECTION/REFRACTION
	//glEnable(GL_CLIP_DISTANCE0);

	////FIRST PASS: SAVE TO WATER REFRACTION FBO-----------------------------------------------------------------
	//water->bindFrameBuffer(Water::REFRACTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUniform4f(glGetUniformLocation(heightmapShaderProgram, "clippingPlane"), refract_clip.x, refract_clip.y, refract_clip.z, refract_clip.w);
	//glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), refract_clip.x, refract_clip.y, refract_clip.z, refract_clip.w);

	//render_scene();
	//
	////SECOND PASS: SAVE TO WATER REFLECTION FBO------------------------------------------------------------------
	//water->bindFrameBuffer(Water::REFLECTION);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUniform4f(glGetUniformLocation(heightmapShaderProgram, "clippingPlane"), reflect_clip.x, reflect_clip.y, reflect_clip.z, reflect_clip.w);
	//glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), reflect_clip.x, reflect_clip.y, reflect_clip.z, reflect_clip.w);

	//float dist = 2 * camera.Position.y - 0.01f;
	//camera.Position.y -= dist;
	//camera.Pitch *= -1;
	//camera.updateCameraVectors();
	//V = camera.GetViewMatrix();

	//render_scene();

	//camera.Position.y += dist;
	//camera.Pitch *= -1;
	//camera.updateCameraVectors();
	//V = camera.GetViewMatrix();

	////THIRD PASS: RENDER SCENE NORMALLY ----------------------------------------------------------------------------
	//water->unbindFrameBuffer();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUniform4f(glGetUniformLocation(heightmapShaderProgram, "clippingPlane"), 0, 0, 0, 0);
	//glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), 0, 0, 0, 0);
	//glUniform3f(glGetUniformLocation(waterShaderProgram, "camera_Position"), camera.Position.x, camera.Position.y, camera.Position.z);

	render_scene();
	glUseProgram(particleShaderProgram);
	p_mgr->render(camera);
	
	//glUseProgram(waterShaderProgram);
	//water->draw(waterShaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	poll_movement();

	//update camera and perspective with polled movements
	V = camera.GetViewMatrix();
	P = glm::perspective(camera.Zoom, (float)width / (float)height, 0.1f, 1000.0f);

	// Swap buffers
	glfwSwapBuffers(window);
}

bool escape_camera = false;
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		switch (key) {
		case GLFW_KEY_W:
		case GLFW_KEY_A:
		case GLFW_KEY_S:
		case GLFW_KEY_D:
			keys[key] = true;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_Y:
			p_mgr->addParticles(200, 200);
			break;
		case GLFW_KEY_R:
			heightmap->refresh(200, 200, 20.f);
			break;
		}

		if (mods == GLFW_MOD_ALT) {
			escape_camera = !escape_camera;
		};

	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W:
		case GLFW_KEY_A:
		case GLFW_KEY_S:
		case GLFW_KEY_D:
			keys[key] = false;
			break;
		}
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