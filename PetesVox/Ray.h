#pragma once

struct Ray
{
	
	
	
    glm::vec3 m_rayStart;
    glm::vec3 m_rayEnd;
    glm::vec3 m_direction;
	
	Ray(const glm::vec3 & position, const glm::vec3 & direction)
		: m_rayStart(position)
		, m_rayEnd(position)
		, m_direction(direction)
	{
	}
	
	void step(float scale)
	{
		float yaw   = glm::radians(m_direction.y + 90);
		float pitch = glm::radians(m_direction.x);
		
		m_rayEnd.x -= glm::cos(yaw)   * scale;
		m_rayEnd.z -= glm::sin(yaw)   * scale;
		m_rayEnd.y -= glm::tan(pitch) * scale;
	}
	
	const glm::vec3 & getEnd() const
	{
		return m_rayEnd;
	}
	
	float getLength() const
	{
		return glm::distance(m_rayStart, m_rayEnd);
	}
	
};

