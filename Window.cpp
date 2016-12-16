#include "window.h"

// On some systems you need to change this to the absolute path
#define SHADER_PATH "../shaders/"
#define SKYBOX_FACE_DIR "../skybox/"
#define TERRAIN_PATH "../terrain/"

const char* window_title = "Island";

//Renderable modules
SkyBox * skybox;
HeightMap* heightmap;
Water* water;
ShadowMap * shadowmap;
Sphere * sphere;
GLFWwindow * map;
ParticleManager * particleManager;
ShadowMapDebugger * tester;

//Camera
Camera camera(glm::vec3(0.0f, 5.0f, 50.0f));
//Variables to keep track camera movement
bool leftButtonDown;
bool firstMouseCallback;
bool escape_camera = false;
bool keys[1024];
glm::vec3 lastPoint;

//TimeKeeping
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;

//Shader programs
GLuint heightmapShaderProgram;
GLuint skyboxShaderProgram;
GLuint normalsShaderProgram;
GLuint waterShaderProgram;
GLuint depthShaderProgram;
GLuint shadowmapShaderProgram;
GLuint shadowmapDebuggerShaderProgram;
GLuint particleShaderProgram;

//Booleans to determine what to show/not show
bool renderParticles = true;
bool showMap = false;
bool showSun = false;

//ShadowMapping variables
glm::vec3 lightInvDir = glm::vec3(15, 10, 2);
glm::mat4 depthMVP;
glm::mat4 staticView;
glm::mat4 depthBiasMVP;
glm::mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

//Water reflection/refraction variables
glm::vec4 refract_clip = glm::vec4(0.f, -1.f, 0.f, 0.01f);
glm::vec4 reflect_clip = glm::vec4(0.f, 1.f, 0.f, -0.01f);

//Window Variables
int Window::width;
int Window::height;
glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top
	heightmapShaderProgram = LoadShaders(SHADER_PATH "heightmap.vert", SHADER_PATH "heightmap.frag");
	skyboxShaderProgram = LoadShaders(SHADER_PATH "skybox.vert", SHADER_PATH "skybox.frag");
	normalsShaderProgram = LoadShaders(SHADER_PATH "normals.vert", SHADER_PATH "normals.frag", SHADER_PATH "normals.gs");
	waterShaderProgram = LoadShaders(SHADER_PATH "water.vert", SHADER_PATH "water.frag");
	particleShaderProgram = LoadShaders(SHADER_PATH "particles.vert", SHADER_PATH "particles.frag");
	depthShaderProgram = LoadShaders(SHADER_PATH "depth.vert", SHADER_PATH "depth.frag");
	shadowmapShaderProgram = LoadShaders(SHADER_PATH "shadowMap.vert", SHADER_PATH "shadowMap.frag");
	shadowmapDebuggerShaderProgram = LoadShaders(SHADER_PATH "pass.vert", SHADER_PATH "pass.frag");

	//Create renderable objects
	water = new Water(200, 200);
	sphere = new Sphere();
	std::vector<const GLchar*> faces = {
		SKYBOX_FACE_DIR "right.jpg", SKYBOX_FACE_DIR "left.jpg", SKYBOX_FACE_DIR "top.jpg",
		SKYBOX_FACE_DIR "bottom.jpg", SKYBOX_FACE_DIR "back.jpg", SKYBOX_FACE_DIR "front.jpg"
	};
	skybox = new SkyBox(faces);
	heightmap = new HeightMap(200, 200);
	particleManager = new ParticleManager(particleShaderProgram);
	shadowmap = new ShadowMap(1280, 960);

	//Initialize shadowmap variables src: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
	glm::mat4 depthViewMatrix, depthProjectionMatrix;
	depthProjectionMatrix = glm::ortho<float>(-50, 50, -50, 50, -50, 100);
	depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));//lightInvDir is freaking out
	depthMVP = depthProjectionMatrix * depthViewMatrix * glm::mat4(1.0f);
	staticView = Window::V;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
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
	if (renderParticles)
		particleManager->generate(deltaTime, 200, 200);
	poll_movement();
}

//polls for WASD movements
void Window::poll_movement() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	Do_Movement();
	V = camera.GetViewMatrix();
	P = glm::perspective(camera.Zoom, (float)width / (float)height, 0.1f, 1000.0f);
}

void Window::display_callback(GLFWwindow* window)
{
	renderToShadowDepthBuffer();

	/*BUG: Can't stick shadowDebugger into renderToScreen() for some reason*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (showMap) {
		glViewport(width / 2, 0, width / 2, height);
		P = glm::perspective(camera.Zoom, (float)(width / 2) / (float)height, 0.1f, 1000.0f);

		glUseProgram(shadowmapDebuggerShaderProgram);
		shadowmap->debug_draw(shadowmapDebuggerShaderProgram);
	}

	renderToWaterRefractionBuffer();
	renderToWaterReflectionBuffer();

	if (showMap) {
		glViewport(0, 0, width / 2, height);
	}

	renderToScreen();
	
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(window);
}

//renders objects onto the scene plainly. (Use this for render-to-texture)
void Window::renderSceneObjects() {
	glUseProgram(skyboxShaderProgram);
	skybox->draw(skyboxShaderProgram, staticView);

	glUseProgram(shadowmapShaderProgram);
	heightmap->draw(shadowmapShaderProgram);
}

//renders to the shadow depth buffer
void Window::renderToShadowDepthBuffer() {
	shadowmap->bind();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, 1280, 960);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(depthShaderProgram);

	GLuint depthMatrixID = glGetUniformLocation(depthShaderProgram, "depthMVP");
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

	//**put object->draw() to render to depth buffer here**
	heightmap->quickDraw();

	shadowmap->unbind();
	glDisable(GL_CULL_FACE);

	depthBiasMVP = biasMatrix*depthMVP;
}

//renders to water refraction buffer
void Window::renderToWaterRefractionBuffer() {
	glUniform4f(glGetUniformLocation(shadowmapShaderProgram, "clippingPlane"), refract_clip.x, refract_clip.y, refract_clip.z, refract_clip.w);
	glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), refract_clip.x, refract_clip.y, refract_clip.z, refract_clip.w);
	water->bindFrameBuffer(Water::REFRACTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderSceneObjects();
	water->unbindFrameBuffer();
}

//renders to water reflection buffer
void Window::renderToWaterReflectionBuffer() {
	water->bindFrameBuffer(Water::REFLECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform4f(glGetUniformLocation(shadowmapShaderProgram, "clippingPlane"), reflect_clip.x, reflect_clip.y, reflect_clip.z, reflect_clip.w);
	glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), reflect_clip.x, reflect_clip.y, reflect_clip.z, reflect_clip.w);

	float dist = 2 * camera.Position.y - 0.01f;
	camera.Position.y -= dist;
	camera.Pitch *= -1;
	camera.updateCameraVectors();
	V = camera.GetViewMatrix();

	renderSceneObjects();

	glUseProgram(particleShaderProgram);
	particleManager->render(camera);

	if (showSun) {
		glUseProgram(shadowmapShaderProgram);
		glm::mat4 Model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(3 * glm::vec3(staticView* glm::vec4(lightInvDir, 0)).x, 3 * glm::vec3(staticView* glm::vec4(lightInvDir, 0)).y, glm::vec3(staticView* glm::vec4(lightInvDir, 0)).z + 1));//(staticView * glm::vec4(, 0.0)
		sphere->draw(Model, shadowmapShaderProgram);
	}

	camera.Position.y += dist;
	camera.Pitch *= -1;
	camera.updateCameraVectors();
	V = camera.GetViewMatrix();
	water->unbindFrameBuffer();
}

//renders to the screen
void Window::renderToScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Draw skybox
	glUseProgram(skyboxShaderProgram);
	glUniform4f(glGetUniformLocation(skyboxShaderProgram, "clippingPlane"), 0, 0, 0, 0);
	skybox->draw(skyboxShaderProgram, staticView);

	//Load shadow shader data
	glUseProgram(shadowmapShaderProgram);
	glUniform1i(glGetUniformLocation(shadowmapShaderProgram, "shadowMap"), 9); //sets uniform sampler2D texSampleri's texture bind loc.
	glUniformMatrix4fv(glGetUniformLocation(shadowmapShaderProgram, "depthBiasMVP"), 1, GL_FALSE, &depthBiasMVP[0][0]);
	glUniform3fv(glGetUniformLocation(shadowmapShaderProgram, "lightInvDir"), 1, &lightInvDir[0]);
	glUniform4f(glGetUniformLocation(shadowmapShaderProgram, "clippingPlane"), 0, 0, 0, 0);
	glUniformMatrix4fv(glGetUniformLocation(shadowmapShaderProgram, "staticview"), 1, GL_FALSE, &staticView[0][0]);
	glActiveTexture(GL_TEXTURE9); //switches texture bind location to GL_TEXTURE(0+i)
	glBindTexture(GL_TEXTURE_2D, shadowmap->depth); //bind texture to active location

	//Draw sun if enabled
	if (showSun) {
		glm::mat4 Model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(4 * glm::vec3(staticView* glm::vec4(lightInvDir, 0)).x, 4 * glm::vec3(staticView* glm::vec4(lightInvDir, 0)).y, glm::vec3(staticView* glm::vec4(lightInvDir, 0)).z));//(staticView * glm::vec4(, 0.0)
		sphere->draw(Model, shadowmapShaderProgram);
	}

	//Draw scene objects
	heightmap->draw(shadowmapShaderProgram);
	glUseProgram(particleShaderProgram);
	particleManager->render(camera);
	glUseProgram(waterShaderProgram);
	glUniform3f(glGetUniformLocation(waterShaderProgram, "camera_Position"), camera.Position.x, camera.Position.y, camera.Position.z);
	water->draw(waterShaderProgram);
}

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
		case GLFW_KEY_P:
			renderParticles = !renderParticles;
			break;
		case GLFW_KEY_R:
			heightmap->refresh(200, 200, 20.f);
			break;
		case GLFW_KEY_L:
			heightmap->refresh(TERRAIN_PATH "terrain.ppm", 20.f);
			break;
		case GLFW_KEY_M:
			if (showMap) {
				glfwSetWindowSize(window, width/ 2, height);
			}
			else {
				glfwSetWindowSize(window, width * 2, height);
			}
			showMap = !showMap;
			break;
		case GLFW_KEY_N:
			showSun = !showSun;
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
			leftButtonDown = true;
		}
		else if (GLFW_RELEASE == action) {
			leftButtonDown = false;
			firstMouseCallback = true;
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
	if (firstMouseCallback)
	{
		lastPoint.x = xpos;
		lastPoint.y = ypos;
		firstMouseCallback = false;
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