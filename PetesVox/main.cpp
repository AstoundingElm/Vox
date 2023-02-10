#include "defines.h"
#include "Window.h"
#include "Math.h"
#include "Camera.h"
#include "Shader.h"
#include "Entity.h"
#include "Texture.h"
#include "Cube.h"
#include "Chunk.h"
#include "ChunkManager.h"
#include "Ray.h"

static Window window;
static Ray ray;
static Camera camera;

int main()
{
	window.windowWidth = 1280;
	window.windowHeight = 720;
	window.wireFrame = false;
	
	createWindow(&window);
	
	Shader shader;
    shader.setAttribute(0, "position");
    shader.setAttribute(1, "textureCoords");
    
    shader.createProgram("shader1");
	
    shader.setUniformLocation("transformationMatrix");
    shader.setUniformLocation("projectionMatrix");
    shader.setUniformLocation("viewMatrix");
	
	Shader simple_shader;
    simple_shader.setAttribute(0, "position");
	
    simple_shader.createProgram("simple_colour");
	
    simple_shader.setUniformLocation("transformationMatrix");
    simple_shader.setUniformLocation("projectionMatrix");
    simple_shader.setUniformLocation("viewMatrix");
	
    loadTexture("tex1.png");
	
    glm::mat4 projectionMatrix = createProjectionMatrix(glm::vec2(1280, 720), 90, 0.1, 1000);
	
    shader.Bind();
    shader.loadMatrix(shader.getUniformLocation("projectionMatrix"), projectionMatrix);
    shader.Unbind();
	
    simple_shader.Bind();
    simple_shader.loadMatrix(simple_shader.getUniformLocation("projectionMatrix"), projectionMatrix);
    simple_shader.Unbind();
	
	camera = {glm::vec3(0,0,0), glm::vec3(0,0,0),  };
	
    // Used when the player presses left mouse click
    glm::vec3 rayLastPosition;
	
	Entity selectedBlockBox;
	ChunkManager chunk_manager;
	chunk_manager.generate(2, 1, 2);
	
	bool running = true;
	while (running)
	{
		sf::Event e;
		while (window.m_window.pollEvent(e))    
		{
			if (e.type == sf::Event::Closed)
				running = false;
			if (e.type == sf::Event::Resized)
				glViewport(0, 0, e.size.width, e.size.height);
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Q)
				running = false;
			
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab)
			{
				
				if (window.wireFrame)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				};
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				
			}
			
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
			{
				// Raycasting
				int x = rayLastPosition.x;
				int y = rayLastPosition.y;
				int z = rayLastPosition.z;
				
				if (chunk_manager.getBlock(x, y, z))
				{
					chunk_manager.setBlock(x, y, z, 0);
					chunk_manager.getChunkFromWorldPosition(x, y, z)->Update();
				}
			}
		}
		
		//////////////////////////////
		//        RayTracing        //
		/////////////////////////////
		
		ray = {camera.m_position, camera.m_position, camera.m_rotation};
		
		for(float length = 0; (length = glm::distance(ray.m_rayStart, ray.m_rayEnd)) < 6;
			rayStep(&ray, 0.05f))
		{
			glm::vec3 position = ray.m_rayEnd;
			//printf("%f, %f, %f\n\n", position.x, position.y, position.z);
			
			int x = static_cast<int>(position.x);
			int y = static_cast<int>(position.y);
			int z = static_cast<int>(position.z);
			
			// Don't check the ray if we are outside of the chunks
			int w = chunk_manager.getChunksSize().x;
			int h = chunk_manager.getChunksSize().y;
			int d = chunk_manager.getChunksSize().z;
			
			if (x >= 0 && y >= 0 && z >= 0 && x < Width * w && y < Height * h && z < Depth * d)
			{
				if (chunk_manager.getBlock(x, y, z))
				{
					rayLastPosition = glm::vec3(x, y, z);
					
					float bx = x;
					float by = y;
					float bz = z;
					
					// Create block box border
					std::vector<GLfloat> linePoints = {
						bx + 0, by + 0, bz + 0,
						bx + 1, by + 0, bz + 0,
						bx + 0, by + 1, bz + 0,
						bx + 1, by + 1, bz + 0,
						bx + 0, by + 0, bz + 1,
						bx + 1, by + 0, bz + 1,
						bx + 0, by + 1, bz + 1,
						bx + 1, by + 1, bz + 1,
						
						bx + 0, by + 0, bz + 0,
						bx + 0, by + 1, bz + 0,
						bx + 1, by + 0, bz + 0,
						bx + 1, by + 1, bz + 0,
						bx + 0, by + 0, bz + 1,
						bx + 0, by + 1, bz + 1,
						bx + 1, by + 0, bz + 1,
						bx + 1, by + 1, bz + 1,
						
						bx + 0, by + 0, bz + 0,
						bx + 0, by + 0, bz + 1,
						bx + 1, by + 0, bz + 0,
						bx + 1, by + 0, bz + 1,
						bx + 0, by + 1, bz + 0,
						bx + 0, by + 1, bz + 1,
						bx + 1, by + 1, bz + 0,
						bx + 1, by + 1, bz + 1,
						
					};
					
					if (selectedBlockBox.VBOs.empty()) selectedBlockBox.setVBO(linePoints, 0, 3);
					else                               selectedBlockBox.VBOs[0]->setData(linePoints, 0, 3);
					
					break;
				}
			}
		}
		
		sf::Time elapsed = calculateElapsedTime(&window);
		//windEnableFPSCounter();
		
		clear();
		
		updateCam(&camera, elapsed, &window.m_window);
		
		
		// Render the chunk
		chunk_manager.render(&shader,  &camera);
		
		// Draw the border box of current voxel
		simple_shader.Bind();
		simple_shader.loadMatrix(simple_shader.getUniformLocation("viewMatrix"), createViewMatrix(&camera));
		
		selectedBlockBox.VAO.Bind();
		
		simple_shader.loadMatrix(simple_shader.getUniformLocation("transformationMatrix"), createTransformationMatrix({0, 0, 0}, { 0, 0, 0 }, { 1, 1, 1 }));
		
		glLineWidth(5);
		glDrawArrays(GL_LINES, 0, 72);
		
		selectedBlockBox.VAO.Unbind();
		simple_shader.Unbind();
		
		display(&window);
		
	}
	
	return 0;
}