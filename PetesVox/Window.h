#pragma once

struct Window
{
	
    sf::Window m_window;
    std::string m_title;
	
    sf::Time m_elapsed;
    sf::Clock m_clock;
	
	Window(int width, int height, std::string title)
	{
		sf::ContextSettings settings;
		settings.depthBits = 24;    // Add this to make the depth buffer work
		settings.stencilBits = 8;
		//settings.antialiasingLevel = 4;
		
		m_window.create(sf::VideoMode(width, height), title, sf::Style::Default, settings);
		
		m_title = title;
		
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
	}
	
	~Window()
	{
		m_window.close();
	}
	
	int ScreenWidth()
	{
		return m_window.getSize().x;
	}
	
	int ScreenHeight()
	{
		return m_window.getSize().y;
	}
	
	void EnableVSync(bool vsync)
	{
		m_window.setVerticalSyncEnabled(vsync);
	}
	
	void EnableFPSCounter()
	{
		m_window.setTitle(m_title + " - FPS: " + std::to_string(60.0f / m_elapsed.asSeconds()));
	}
	
	void EnableWireframe(bool wireframe)
	{
		if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	
	void display()
	{
		m_window.display();
	}
	
	sf::Time calculateElapsedTime()
	{
		m_elapsed = m_clock.restart();
		return m_elapsed;
	}
	
	sf::Window * getWindow()
	{
		return &m_window;
	}
	
};
