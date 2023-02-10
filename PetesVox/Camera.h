#pragma once
#include <memory>

struct Camera
{
	glm::vec3 m_position;
    glm::vec3 m_rotation;
};

void updateCam(Camera* camera, sf::Time elapsed, sf::Window* window, float MovementSpeed = 1.0f, float Sensetivity = 0.01f)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		// Calculate the directional vector and add it to camera position
		camera->m_position.z -= MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-camera->m_rotation.y));
		camera->m_position.x -= MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-camera->m_rotation.y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		camera->m_position.z += MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-camera->m_rotation.y));
		camera->m_position.x += MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-camera->m_rotation.y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		camera->m_position.x += MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-camera->m_rotation.y));
		camera->m_position.z -= MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-camera->m_rotation.y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		camera->m_position.x -= MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-camera->m_rotation.y));
		camera->m_position.z += MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-camera->m_rotation.y));
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		camera->m_position.y -= MovementSpeed * elapsed.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		camera->m_position.y += MovementSpeed * elapsed.asSeconds();
	}
	
	// Get the offset between mouse movements then add them to rotation
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2i screenCenter = sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
	
	if (window->hasFocus())
	{
		float xOffset = mousePos.x - screenCenter.x;
		float yOffset = mousePos.y - screenCenter.y;
		
		xOffset *= Sensetivity;
		yOffset *= Sensetivity;
		
		camera->m_rotation.x += yOffset;
		camera->m_rotation.y += xOffset;
		
		// Set max up/down rotation
		if (camera->m_rotation.x > 90.0f)
			camera->m_rotation.x = 90.0f;
		
		if (camera->m_rotation.x < -90.0f)
			camera->m_rotation.x = -90.0f;
		
		// Set mouse position to the center of the screen
		if (xOffset != 0 || yOffset != 0)
			sf::Mouse::setPosition(screenCenter, *window);
	}
}

glm::mat4x4 static createViewMatrix(Camera* camera)
{
	// The camera never moves the world moves opposite of the camera
	glm::mat4x4 view = glm::mat4x4(1.0f);
	
	view = glm::rotate(view, glm::radians(camera->m_rotation.x), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians(camera->m_rotation.y), glm::vec3(0, 1, 0));
	view = glm::rotate(view, glm::radians(camera->m_rotation.z), glm::vec3(0, 0, 1));
	view = glm::translate(view, -camera->m_position);
	
	return view;
}