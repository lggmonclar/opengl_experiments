#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "camera.h"

#include <math/math.h>
#include <debug/line.h>

#include "scene.h"
#include "cubemap_reflections.h"
#include "directional_shadow_mapping.h"
#include "omnidirectional_shadow_mapping.h"


void famebufferSizeCallback(GLFWwindow* window, int width, int height);
void cameraMovementMouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void toggleWireframeMode();
void toggleCursorActive(GLFWwindow* window);
void renderSceneMenu();
template <class T> void loadScene();

const unsigned int WINDOW_WIDTH = 1366;
const unsigned int WINDOW_HEIGHT = 768;

std::unique_ptr<Scene> currentScene = NULL;
Camera camera(Vector3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
int keyStates[512];
bool wireframeMode = false;
bool isCursorActive = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow* initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, famebufferSizeCallback);
	glfwSetCursorPosCallback(window, cameraMovementMouseCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	return window;
}

void setupGUI(GLFWwindow* window) {
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
}

int main() {
	GLFWwindow* window = initWindow();
	setupGUI(window);
	glfwSetKeyCallback(window, inputCallback);
	loadScene<CubemapReflectionsScene>();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		camera.processInput(keyStates, deltaTime);

		// render
		// ------
		glClearColor(0.2f, 0.6f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Matrix4 P = Matrix4::perspectiveProjection(deg2rad(camera.zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		currentScene->Draw(camera, P);
		renderSceneMenu();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

template <class T> void loadScene() {
	currentScene.reset(new T());
	currentScene->setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void renderSceneMenu() {
	static bool showSceneOptions = true;
	//UI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow(NULL);
	ImGui::Begin("Menu", NULL, 0);
	ImGui::Checkbox("Show scene options", &showSceneOptions);
	if (ImGui::CollapsingHeader("Cubemapping Reflections")) {
		ImGui::TextWrapped("Shows how a cubemap of a skybox can be used as a sample target that gives the illusion of reflection/refraction in a model");
		if (ImGui::Button("Load Scene")) {
			loadScene<CubemapReflectionsScene>();
		}
	}
	if (ImGui::CollapsingHeader("Directional Shadow Mapping")) {
		ImGui::TextWrapped("Shows how shadows can be simulated for directional lights by rendering its viewpoint to a depth-only framebuffer, which is then used to compare fragments and determine if they should be shadowed.");
		if (ImGui::Button("Load Scene")) {
			loadScene<DirectionalShadowMappingScene>();
		}
	}
	if (ImGui::CollapsingHeader("Omnidirectional Shadow Mapping")) {
		ImGui::TextWrapped("");
		if (ImGui::Button("Load Scene")) {
			loadScene<OmnidirectionalShadowMappingScene>();
		}
	}
	ImGui::End();

	if (showSceneOptions) {
		ImGui::Begin("Scene Options", &showSceneOptions, 0);
		currentScene->DrawGUIOptions();
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void inputCallback(GLFWwindow* window, int key, int scancode, int state, int mods) {
	keyStates[key] = state;

	if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_M && state == GLFW_RELEASE)
		toggleWireframeMode();

	if (key == GLFW_KEY_LEFT_CONTROL && state == GLFW_PRESS)
		toggleCursorActive(window);
}

void toggleCursorActive(GLFWwindow* window) {
	isCursorActive = !isCursorActive;
	if (isCursorActive) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void toggleWireframeMode() {
	wireframeMode = !wireframeMode;
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void famebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	currentScene->setViewport(width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void cameraMovementMouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (isCursorActive)
		return;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll(yoffset);
}