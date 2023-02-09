#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "defines.h"
#define NUM_SHADERS 2U

GLuint m_program;
GLuint m_shaders[NUM_SHADERS];

char* loadShader(const char* fileName)
{
	FILE* fp;
	long size = 0;
	char* shaderContent;
	
	fp = fopen(fileName, "rb");
	if(fp == NULL) {
        exit(-1);
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp)+1;
    fclose(fp);s
		
		
		fp = fopen(fileName, "r");
	
	shaderContent = (char*)memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size-1, fp);
    fclose(fp);
	
    return shaderContent;
};

glm::mat4x4 static createProjectionMatrix(glm::vec2 screenSize, float FOV, float NEAR_PLANE, float FAR_PLANE)
{
	return glm::perspective(glm::radians(FOV), (screenSize.x / screenSize.y), NEAR_PLANE, FAR_PLANE);
}


GLuint createShader(const char* text, unsigned int type)
{
	GLuint shader = glCreateShader(type);
	if(shader == 0)
		PERROR("Failed to load shader");
	
	glShaderSource(shader, 1, &text, NULL );
	glCompileShader(shader);
	
	return shader;
}

void createProgram(const char* vertFileName, const char * fragFileName)
{
	m_program = glCreateProgram();
	m_shaders[0] = createShader(loadShader(vertFileName), GL_VERTEX_SHADER);
	m_shaders[1] = createShader(loadShader(fragFileName), GL_FRAGMENT_SHADER);
	
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
        glAttachShader(m_program, m_shaders[i]);
	
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "textureCoords");
	
	glLinkProgram(m_program);
    glValidateProgram(m_program);
};

GLuint texture;

void loadMatrix(int location, const glm::mat4x4  matrix)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


int main()
{
	
	sf::Window m_window;
	
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	
	m_window.create(sf::VideoMode(800, 600), "Vox", sf::Style::Default, settings);
	
	
	m_window.setVerticalSyncEnabled(true);
	glewInit();
	
	createProgram("shader1.vert", "shader1.frag");
	
	createProgram("simple_colour.vert", "simple_colour.frag");
	
	sf::Image image;
	
	if(!image.loadFromFile("tex1.png"))
	{
		PERROR("Failed to load texture here");
		
		
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glm::mat4 projectionMatrix = createProjectionMatrix(glm::vec2(WIDTH, HEIGHT), 90, 0.1, 1000);
	
	glUseProgram(m_program);
	loadMatrix(0, projectionMatrix);
	glUseProgram(0);
	
	
	puts("We reached herer fine");
	
	return 0;
}

