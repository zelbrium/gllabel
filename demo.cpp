/*
 * Aidan Shafran <zelbrium@gmail.com>, 2016.
 * 
 * Demo code for GLLabel. Depends on GLFW3, GLEW, GLM, FreeType2, and C++11.
 * Makefile was created for use on Arch Linux. I haven't tested it elsewhere.
 */

#include "label.hpp"
#include <glfw3.h>
#include <glm/gtx/transform.hpp>
#include <locale>
#include <codecvt>

static const uint32_t kWidth = 1536;
static const uint32_t kHeight = 1152;

static GLLabel *Label;
bool spin = true;

void onKeyPress(GLFWwindow *window, int key, int scanCode, int action, int mods);
void onCharTyped(GLFWwindow *window, unsigned int codePoint);

int main()
{
	// Create a window
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW.\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(kWidth, kHeight, "GLLabel Demo", NULL, NULL);
	if(!window)
	{
		fprintf(stderr, "Failed to create GLFW window.");
		glfwTerminate();
		return -1;
	}
	
	glfwSetKeyCallback(window, onKeyPress);
	glfwSetCharCallback(window, onCharTyped);
	
	// Create OpenGL context
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW.\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	
	printf("GL Version: %s\n", glGetString(GL_VERSION));

	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	Label = new GLLabel();
	Label->SetText(U"type!");
	
	glm::mat4 kT = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
	glm::mat4 kS = glm::scale(glm::mat4(), glm::vec3(kHeight/12000000.0, kWidth/12000000.0, 1.0));

	int fpsFrame = 0;
	double fpsStartTime = glfwGetTime();
	while(!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		
		glClearColor(160/255.0, 169/255.0, 175/255.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glm::mat4 R = glm::rotate(glm::mat4(), time/3, glm::vec3(0.0,0.0,1.0));
		glm::mat4 S = glm::scale(glm::mat4(), glm::vec3(sin(time)/6000.0, cos(time)/12000.0, 1.0));
		Label->Render(time, spin ? (kT*R*S) : (kT*kS));
		
		glfwPollEvents();
		glfwSwapBuffers(window);
		
		// FPS Counter
		fpsFrame ++;
		if(fpsFrame >= 20)
		{
			double endTime = glfwGetTime();
			double fps = fpsFrame / (endTime - fpsStartTime);
			fpsFrame = 0;
			fpsStartTime = endTime;
			printf("FPS: %f\n", fps);
		}
	}
	
	// Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void onKeyPress(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
	if(action == GLFW_RELEASE)
		return;
		
	if(key == GLFW_KEY_BACKSPACE)
	{
		std::u32string text = Label->GetText();
		if(text.size() > 0)
			Label->SetText(text.substr(0, text.size()-1));
	}
	else if(key == GLFW_KEY_ENTER)
	{
		Label->AppendText(U"\n");
	}
	else if(key == GLFW_KEY_ESCAPE)
	{
		spin = !spin;
	}
}

void onCharTyped(GLFWwindow *window, unsigned int codePoint)
{
	Label->AppendText(std::u32string(1, codePoint));
}