#pragma once

GLFWwindow* window  = NULL;

/*
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwSwapInterval(1);
	
	window = glfwCreateWindow(800, 600, "Vox", NULL, NULL);
	if(window == NULL)
	{
		PERROR("Failed to create window");
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	glewInit();
	
	glViewport(800, 600, 0, 0);
	
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	
	shaders = {"vertex.glsl", "frag.glsl", 0, 0, 0, 0, 0, 0};
	
	initShaders(&shaders);
	
	linkShaders(&program);
	
	
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);
		
		
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 