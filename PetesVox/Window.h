#pragma once

struct Window
{
	int windowWidth;
	int windowHeight;
	sf::Window m_window;
    sf::Time m_elapsed;
    sf::Clock m_clock;
	bool wireFrame;
	
};

void createWindow(Window* window)
{
	sf::ContextSettings settings;
	settings.depthBits = 24;    // Add this to make the depth buffer work
	settings.stencilBits = 8;
	//settings.antialiasingLevel = 4;
	
	window->m_window.create(sf::VideoMode(window->windowWidth, window->windowHeight), "Petes vox", sf::Style::Default, settings);
	
	
	
	// Load after window
	if(glewInit() != 0)
	{
		printf("Failed to gladLoadGL!");
		exit(-1);
	}
	
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version  = glGetString(GL_VERSION);
	printf("OpenGL %s\n", version);
	printf("OpenGL Renderer: %s\n", renderer);
	window->m_window.setVerticalSyncEnabled(true);
};

sf::Time calculateElapsedTime(Window* window)
{
	window->m_elapsed = window->m_clock.restart();
	return window->m_elapsed;
}

void clear(float r = 0.2f, float g = 0.3f, float b = 0.3f, float a = 1.0f)
{
	// Enable the depth buffer
	glEnable(GL_DEPTH_TEST);
	
	// CLEAR DEPTH BUFFER BIT so triangles don't overlap
	// CLEAR COLOR BUFFER for background screen
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void display(Window* window)
{
	window->m_window.display();
}

