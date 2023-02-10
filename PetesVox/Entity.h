#pragma once

struct VertexArray
{
	
	GLuint VAO;
	
	VertexArray()
	{
		glGenVertexArrays(1, &VAO);
	}
	
	~VertexArray()
	{
		glDeleteVertexArrays(1, &VAO);
	}
	
	void Bind()
	{
		glBindVertexArray(VAO);
	}
	
	void Unbind()
	{
		glBindVertexArray(0);
	}
};

struct VertexBufferObject
{
	
	
	GLuint VBO;
	
	VertexBufferObject()
	{
		glGenBuffers(1, &VBO);
	}
	
	~VertexBufferObject()
	{
		glDeleteBuffers(1, &VBO);
		printf("VBO: Deconstructor called\n");
	}
	
	void setData(const std::vector<GLfloat>& data, int attributeID, int size, int DrawMode = GL_STATIC_DRAW)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), DrawMode);
		
		glEnableVertexAttribArray(attributeID);
		glVertexAttribPointer(attributeID, size, GL_FLOAT, false, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void setSubData(const std::vector<GLfloat>& data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * data.size(), data.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

struct ElementArrayBuffer
{
	
	GLuint EBO;
	GLuint size;
	
	ElementArrayBuffer()
	{
		glGenBuffers(1, &EBO);
	}
	
	~ElementArrayBuffer()
	{
		glDeleteBuffers(1, &EBO);
	}
	
	void setData(const std::vector<GLuint>& indicies, int DrawMode)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(), indicies.data(), DrawMode);
		
		size = indicies.size();
	}
	
	void setSubData(const std::vector<GLuint>& indicies)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * indicies.size(), indicies.data());
		
		size = indicies.size();
	}
	
};


struct Entity
{
	glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
	
    VertexArray VAO;
    ElementArrayBuffer EBO;
	std::vector<VertexBufferObject*> VBOs;
	
	Entity()
		: position(0, 0, 0)
		, rotation(0, 0, 0)
		, scale(1, 1, 1)
	{
	}
	
	~Entity()
	{
		for (auto& VBO : VBOs)
			delete VBO;
	}
	
	void setEBO(const std::vector<GLuint>& indicies, int DrawMode)
	{
		VAO.Bind();
		EBO.setData(indicies, DrawMode);
		VAO.Unbind();
	}
	
	void setVBO(const std::vector<GLfloat>& data, int attributeID, int size, int DrawMode = GL_STATIC_DRAW)
	{
		VAO.Bind();
		auto VBO = new VertexBufferObject();
		VBO->setData(data, attributeID, size, DrawMode);
		VBOs.push_back(VBO);
		VAO.Unbind();
	}
	
	void updateEBO(const std::vector<GLuint>& indicies)
	{
		VAO.Bind();
		EBO.setSubData(indicies);
		VAO.Unbind();
	}
	
	void updateVBO(const std::vector<GLfloat>& data, int index)
	{
		if (index >= VBOs.size())
		{
			printf("Error: VBO at '%d' does not exist!", index);
			return;
		}
		
		VAO.Bind();
		VBOs[index]->setSubData(data);
		VAO.Unbind();
	}
	
};

