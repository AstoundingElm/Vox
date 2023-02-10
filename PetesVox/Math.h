#pragma once

glm::mat4x4 static createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	// Set identity matrix
	glm::mat4x4 matrix = glm::mat4x4(1.0f);
	matrix = glm::translate(matrix, translation);
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, scale);
	
	return matrix;
}