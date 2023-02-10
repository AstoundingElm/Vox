#pragma once

struct Shader
{
    
	static const unsigned int NUM_SHADERS = 2;
	
	std::vector<std::pair<int, std::string>> m_attributes;
	std::map<std::string, int> m_uniformLocations;
	
	
	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	
	Shader()
	{
		m_program = -1;
	}
	
	~Shader()
	{
		for (unsigned int i = 0; i < NUM_SHADERS; i++)
		{
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
		
		glDeleteProgram(m_program);
	}
	
	void createProgram(const std::string & fileName)
	{
		m_program = glCreateProgram();
		m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
		m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
		
		for (unsigned int i = 0; i < NUM_SHADERS; i++)
			glAttachShader(m_program, m_shaders[i]);
		
		
		// Bind attributes
		for (auto& attribute : m_attributes)
		{
			glBindAttribLocation(m_program, attribute.first, attribute.second.c_str());
		}
		
		glLinkProgram(m_program);
		glValidateProgram(m_program);
	}
	
	void Bind()
	{
		glUseProgram(m_program);
	}
	
	void Unbind()
	{
		glUseProgram(0);
	}
	
	void setAttribute(int attributeID, std::string var_name)
	{
		m_attributes.push_back(std::make_pair(attributeID, var_name));
	}
	
	void setUniformLocation(std::string uniform_name)
	{
		if (m_program == -1)
			std::cout << "CreateProgram hasn't been called yet!\n";
		
		int location = glGetUniformLocation(m_program, uniform_name.c_str());
		m_uniformLocations.insert(std::make_pair(uniform_name, location));
	}
	
	int getUniformLocation(std::string uniform_name)
	{
		if (m_uniformLocations.find(uniform_name) == m_uniformLocations.end())
		{
			std::cout << "Found no location for the uniform: " << uniform_name << "\n";
			return -1;
		}
		else
			return m_uniformLocations.at(uniform_name);
	}
	
	void loadFloat(int location, float value)
	{
		glUniform1f(location, value);
	}
	
	void loadVector3(int location, glm::vec3 vector)
	{
		glUniform3f(location, vector.x, vector.y, vector.z);
	}
	
	void loadVector4(int location, glm::vec4 vector)
	{
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}
	
	void loadBool(int location, bool value)
	{
		float out = 0.0f;
		if (value)
			out = 1.0f;
		
		glUniform1f(location, out);
	}
	
	void loadMatrix(int location, const glm::mat4x4 & matrix)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	GLuint CreateShader(const std::string & text, unsigned int type)
	{
		GLuint shader = glCreateShader(type);
		
		if (shader == 0)
			std::cerr << "Error compiling shader type " << type << std::endl;
		
		const GLchar* p[1];
		p[0] = text.c_str();
		GLint lengths[1];
		lengths[0] = text.length();
		
		glShaderSource(shader, 1, p, lengths);
		glCompileShader(shader);
		
		return shader;
	}
	
	std::string LoadShader(const std::string & fileName)
	{
		std::ifstream file;
		file.open((fileName).c_str());
		
		std::string output;
		std::string line;
		
		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, line);
				output.append(line + "\n");
			}
		}
		else
		{
			std::cerr << "Unable to load shader: " << fileName << std::endl;
		}
		
		return output;
	}
};