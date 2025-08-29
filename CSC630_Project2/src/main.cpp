/* Lab 6 base code - transforms using matrix stack built on glm
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>\

//For animation
#include <chrono> // For std::this_thread and std::chrono_literals
#include <thread> // For std::this_thread::sleep_for

#include <filesystem>

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;

	// Shape to be used (from obj file)
	shared_ptr<Shape> appaShape;
	shared_ptr<Shape> rocktreeShape;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	float lightTrans = 0;
	float gRot = 0;
	float gTrans=0;


	int animateIndex = 2;
	bool left = true;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
		if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
			lightTrans--;
		}
		if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
			lightTrans++;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
			gRot+= 0.16;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
			gRot-= 0.16;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
			gTrans--;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
			gTrans++;
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			 glfwGetCursorPos(window, &posX, &posY);
			 cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("MatAmb");
		prog->addUniform("MatDif");
		prog->addUniform("lightP");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");
	}

	void initGeom(const std::string& objectDirectory)
	{

		std::cout << "Current working directory: "<< std::filesystem::current_path() << std::endl;
		// Initialize mesh.
		appaShape = make_shared<Shape>();
		//shape->loadMesh(resourceDirectory + "/SmoothSphere.obj");
		//shape->loadMesh(resourceDirectory + "/sphere.obj");
		appaShape->loadMesh(objectDirectory + "/appa.obj");
		appaShape->resize();
		appaShape->init();

		rocktreeShape = make_shared<Shape>();
		rocktreeShape->loadMesh(objectDirectory + "/rocktree.obj");
		rocktreeShape->resize();
		rocktreeShape->init();
	}

	void SetMaterial(int i) {

    	prog->bind();
    	switch (i) {
    		case 0: //shiny blue plastic
    			glUniform3f(prog->getUniform("MatAmb"), 0.02, 0.04, 0.2);
    			/*glUniform3f(prog->getUniform("MatDif"), 0.0, 0.16, 0.9);
    			glUniform3f(prog->getUniform("MatSpec"), 0.14, 0.2, 0.8);
    			glUniform1f(prog->getUniform("MatShine"), 120.0);*/
    		break;
    		case 1: // flat grey
    			glUniform3f(prog->getUniform("MatAmb"), 0.13, 0.13, 0.14);
    			/*glUniform3f(prog->getUniform("MatDif"), 0.3, 0.3, 0.4);
    			glUniform3f(prog->getUniform("MatSpec"), 0.3, 0.3, 0.4);
    			glUniform1f(prog->getUniform("MatShine"), 4.0);*/
    		break;
    		case 2: //brass
    			glUniform3f(prog->getUniform("MatAmb"), 0.3294, 0.2235, 0.02745);
    			/*glUniform3f(prog->getUniform("MatDif"), 0.7804, 0.5686, 0.11373);
    			glUniform3f(prog->getUniform("MatSpec"), 0.9922, 0.941176, 0.80784);
    			glUniform1f(prog->getUniform("MatShine"), 27.9);*/
    		break;
  		}
	}

	void render()
	{
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use the matrix stack for Lab 6
		float aspect = width/(float)height;

		// Create the matrix stacks - please leave these alone for now
		auto Projection = make_shared<MatrixStack>();
		auto View = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();

		// Apply perspective projection.
		Projection->pushMatrix();
		Projection->perspective(45.0f, aspect, 0.01f, 100.0f);

		// View is identity - for now
		View->pushMatrix();

		// Draw a stack of cubes with indiviudal transforms
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniform3f(prog->getUniform("lightP"), lightTrans+2, 3, 5);

		// draw the spheres
		Model->pushMatrix();
		  //global rotate (the whole scene )
		Model->rotate(gRot, vec3(0, 1, 0));
		//Alter positions of objects in world space in this code block below.

		//push appaShape

		Model->pushMatrix();
		Model->translate(vec3(animateIndex, 1, -5 + gTrans));
		if (left) {
			animateIndex--;
			if (animateIndex == -2) {
				left = false;
			}
			Model->rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Model->rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else {
			animateIndex++;
			if (animateIndex == 2) {
				left = true;
			}
			Model->rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Model->rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		Model->scale(vec3(1.5, 1.5, 1.5));
		SetMaterial(1);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		appaShape->draw(prog);

		Model->popMatrix();
		//end push appaShape

		//push rocktreeShape

		for (int i = 0; i <=2 ; i++) {

			Model->pushMatrix();

			Model->translate(vec3(-2 + i, -1.1, -5 + gTrans - 0.75*i));

			Model->rotate(glm::radians(i * 40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Model->scale(vec3(1.25, 1.25, 1.25));
			SetMaterial(2);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			rocktreeShape->draw(prog);

			Model->popMatrix();

		}
		for (int i = 1; i >=0; i--) {

			Model->pushMatrix();

			Model->translate(vec3(2-i, -1.1, -5 + gTrans - 0.75*i));

			Model->rotate(glm::radians(i * 40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Model->scale(vec3(1.25, 1.25, 1.25));
			SetMaterial(2);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			rocktreeShape->draw(prog);

			Model->popMatrix();

		}
		//end push rocktreeShape

		Model->popMatrix();

		prog->unbind();

		// Pop matrix stacks.
		Projection->popMatrix();
		View->popMatrix();

	}
};

int main(int argc, char *argv[])
{
	// Where the resources are loaded from
	std::string resourceDir = "resources";
	std::string objectDir = "objects";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	//windowManager->init(640, 480);
	windowManager->init(1600, 1200);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(objectDir);

	// Loop until the user closes the window.
#include <chrono> // For std::this_thread and std::chrono_literals
#include <thread> // For std::this_thread::sleep_for

	while (!glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Record the start time of rendering.
		auto startTime = std::chrono::steady_clock::now();

		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());

		// Poll for and process events.
		glfwPollEvents();

		// Calculate the time taken for rendering.
		auto endTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

		// Adjust the delay to achieve a desired frame rate.
		constexpr int targetFrameRate = 4; // Target frame rate in frames per second
		constexpr int targetFrameTime = 500 / targetFrameRate; // Target frame time in milliseconds
		int delayTime = targetFrameTime - elapsedTime.count();

		// Add delay if the rendering took less time than the target frame time.
		if (delayTime > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
		}
	}


	// Quit program.
	windowManager->shutdown();
	return 0;
}
