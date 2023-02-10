#pragma once

struct Ray
{
	glm::vec3 m_rayStart;
    glm::vec3 m_rayEnd;
    glm::vec3 m_direction;
};

void rayStep(Ray* ray, float scale)
{
	float yaw   = glm::radians(ray->m_direction.y + 90);
	float pitch = glm::radians(ray->m_direction.x);
	
	ray->m_rayEnd.x -= glm::cos(yaw)   * scale;
	ray->m_rayEnd.z -= glm::sin(yaw)   * scale;
	ray->m_rayEnd.y -= glm::tan(pitch) * scale;
}

