#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <string>
#include <fstream>
#include <ostream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>


#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "Math.h"
#include "defines.h"
#include "Camera.h"
#include "Shader.h"
#include "Entity.h"
#include "Texture.h"

void bind(Shader*shader)
{
	glUseProgram(shader->m_program);
};
int getUniformLocation(Shader* shader, std::string uniform_name);


void loadMatrix(int location, const glm::mat4x4 & matrix)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


void unbind()
{
	glUseProgram(0);
}
#include "Cube.h"

#include "Chunk.h"
#include "Ray.h"

glm::mat4x4 static createProjectionMatrix(glm::vec2 screenSize, float FOV, float NEAR_PLANE, float FAR_PLANE)
{
	return glm::perspective(glm::radians(FOV), (screenSize.x / screenSize.y), NEAR_PLANE, FAR_PLANE);
}

int getUniformLocation(Shader* shader, std::string uniform_name)
{
    if (shader->m_uniformLocations.find(uniform_name) == shader->m_uniformLocations.end())
    {
        std::cout << "Found no location for the uniform: " << uniform_name << "\n";
        return -1;
    }
    else
        return shader->m_uniformLocations.at(uniform_name);
}
int main()
{
	
	glewInit();
    Window window(1280, 720, "My window");
    window.EnableVSync(true);
	
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
	
	Texture texture;
    texture.loadTexture("tex1.png");
	
    glm::mat4 projectionMatrix = createProjectionMatrix(glm::vec2(window.ScreenWidth(), window.ScreenHeight()), 90, 0.1, 1000);
	
    shader.Bind();
    shader.loadMatrix(shader.getUniformLocation("projectionMatrix"), projectionMatrix);
    shader.Unbind();
	
    simple_shader.Bind();
    simple_shader.loadMatrix(simple_shader.getUniformLocation("projectionMatrix"), projectionMatrix);
    simple_shader.Unbind();
	
    Camera camera;
	
    // Used when the player presses left mouse click
    glm::vec3 rayLastPosition;
	
    // Displays a border of white lines around the current looking block
    Entity selectedBlockBox;
	
    ChunkManager chunk_manager;
    chunk_manager.generate(2, 1, 2);
    
    bool wireframe = false;
    bool running = true;
    while (running)
    {
        sf::Event e;
        while (window.getWindow()->pollEvent(e))    
        {
            if (e.type == sf::Event::Closed)
                running = false;
            if (e.type == sf::Event::Resized)
                glViewport(0, 0, e.size.width, e.size.height);
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Q)
                running = false;
			
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab)
            {
                wireframe = !wireframe;
                if (wireframe) window.EnableWireframe(true);
                else           window.EnableWireframe(false);
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
        for (Ray ray(camera.getPosition(), camera.getRotation()); ray.getLength() < 6; ray.step(0.05f))
        {
            glm::vec3 position = ray.getEnd();
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
		
        sf::Time elapsed = window.calculateElapsedTime();
        window.EnableFPSCounter();
		
        window.clear();
		
        camera.Update(elapsed, window.getWindow());
		
        // ...Drawing
        auto Draw = [&](Entity* entity,  Shader* shader,  Texture* texture)
        {
            shader->Bind();
			
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture->texture);
			
            shader->loadMatrix(shader->getUniformLocation("viewMatrix"), createViewMatrix(&camera));
            entity->VAO.Bind();
			
            shader->loadMatrix(shader->getUniformLocation("transformationMatrix"), createTransformationMatrix(entity->position, entity->rotation, entity->scale));
			
            glDrawElements(GL_TRIANGLES, entity->EBO.size, GL_UNSIGNED_INT, 0);
            entity->VAO.Unbind();
			
            shader->Unbind();
        };
		
        // Render the chunk
        chunk_manager.render(&shader, &texture, &camera);
		
        // Draw the border box of current voxel
        simple_shader.Bind();
        simple_shader.loadMatrix(simple_shader.getUniformLocation("viewMatrix"), createViewMatrix(&camera));
		
        selectedBlockBox.VAO.Bind();
		
        simple_shader.loadMatrix(simple_shader.getUniformLocation("transformationMatrix"), createTransformationMatrix({0, 0, 0}, { 0, 0, 0 }, { 1, 1, 1 }));
		
        glLineWidth(5);
        glDrawArrays(GL_LINES, 0, 72);
		
        selectedBlockBox.VAO.Unbind();
        simple_shader.Unbind();
        
        window.display();
		// sleep(0.1f);
		
    }
	
	return 0;
}