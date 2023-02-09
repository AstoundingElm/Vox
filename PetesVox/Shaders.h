#pragma once

struct Shaders
{
	const char* vertexShaderFilePath; const char * fragmentShaderFilePath;
	GLuint programID;
	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;
	char* vertShaderContent;
	char* fragShaderContent;
	int numAttributes;
	
};

PINLINE GLuint getUniformLocation(const char * uniName, Shaders* shaders)
{
	GLuint location = glGetUniformLocation(shaders->programID, uniName);
	if(location == GL_INVALID_INDEX)
	{
		PERROR("Wrong uni name");
	}
	return location;
};

//#define BUF_SIZE 1000

PINLINE char* get_shader_content(const char* fileName)
{
    FILE *fp;
	long size = 0;
	char* shaderContent;//[BUF_SIZE];
    
    /* Read File to get size */
    fp = fopen(fileName, "rb");
    if(fp == NULL) {
        exit(-1);
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp)+1;
    fclose(fp);
	
    /* Read File for Content */
    fp = fopen(fileName, "r");
	//shaderContent[BUF_SIZE];
    shaderContent = (char*)memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size-1, fp);
    fclose(fp);
	
	
    return shaderContent;
}

PINLINE void initShaders(Shaders* shaders)
{
	shaders->programID = glCreateProgram();
	shaders->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if(shaders->vertexShaderID == 0)
	{
		fprintf(stderr, "Failed to create vert shader");
		exit(-1);
	}
	
	
	{
		shaders->vertShaderContent = get_shader_content(shaders->vertexShaderFilePath);
		
		
		glShaderSource(shaders->vertexShaderID, 1, &shaders->vertShaderContent, NULL );
		glCompileShader(shaders->vertexShaderID);
		
		GLint success = 0;
		glGetShaderiv(shaders->vertexShaderID, GL_COMPILE_STATUS, &success);
		
		if(success == GL_FALSE)
		{
			GLint maxlength = 0;
			glGetShaderiv(shaders->vertexShaderID, GL_INFO_LOG_LENGTH, &maxlength);
			
			char *errorlog = (char*)malloc(sizeof(maxlength));
			
			glGetShaderInfoLog(shaders->vertexShaderID, maxlength, &maxlength, &errorlog[0]);
			glDeleteShader(shaders->vertexShaderID);
			
			printf("%s\n", &errorlog[0]);
			exit(-1);
		}
	}
	
	{
		shaders->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if(shaders->fragmentShaderID == 0)
		{
			fprintf(stderr, "Failed to create frag shader");
			exit(-1);
		}
		
		shaders->fragShaderContent = get_shader_content(shaders->fragmentShaderFilePath);
		glShaderSource(shaders->fragmentShaderID, 1, &shaders->fragShaderContent, NULL );
		glCompileShader(shaders->fragmentShaderID);
		
		GLint success = 0;
		glGetShaderiv(shaders->fragmentShaderID, GL_COMPILE_STATUS, &success);
		
		if(success == GL_FALSE)
		{
			GLint maxlength = 0;
			glGetShaderiv(shaders->fragmentShaderID, GL_INFO_LOG_LENGTH, &maxlength);
			
			char *errorlog = NULL;
			errorlog = (char*)malloc(sizeof(maxlength));
			
			glGetShaderInfoLog(shaders->fragmentShaderID, maxlength, &maxlength, &errorlog[0]);
			glDeleteShader(shaders->fragmentShaderID);
			
			printf("%s\n", &errorlog[0]);
			exit(-1);
			
		}
	}
}

PINLINE void linkShaders(Shaders* shaders)
{
	
	glAttachShader(shaders->programID, shaders->vertexShaderID);
	glAttachShader(shaders->programID, shaders->fragmentShaderID);
	
	glLinkProgram(shaders->programID);
	
	glDetachShader(shaders->programID,shaders->vertexShaderID);
	glDetachShader(shaders->programID, shaders->fragmentShaderID);
	glDeleteShader( shaders->vertexShaderID);
	glDeleteShader( shaders->fragmentShaderID);
}

PINLINE void addAttribute(const char* attributeName, Shaders* shaders)
{
	
	glBindAttribLocation(shaders->programID, shaders->numAttributes++, attributeName);
	
}

PINLINE void useProgram(Shaders* shaders)
{
	glUseProgram(shaders->programID);
	
	for(int i = 0; i < shaders->numAttributes; i++){
		glEnableVertexAttribArray(i);
		
	}
}

PINLINE void unUseProgram(Shaders* shaders)
{
	glUseProgram(0);
	
	for(int i = 0; i < shaders->numAttributes; i++){
		glDisableVertexAttribArray(i);
		
	}
}


/*

GLuint createShader(const char* fileName, unsigned int type, Shaders* shaders)
{
	shaders->programID = glCreateProgram();
	GLuint shader = glCreateShader(type);
	if(shader == 0)
		fprintf(stderr, "Couldnt't compile shader of type: %d", type);
	
	//shaders->vertShaderContent = get_shader_content(shaders->vertexShaderFilePath);
	
	
	glShaderSource(shaders->vertexShaderID, 1, &shaders->vertShaderContent, NULL );
	glCompileShader(shaders->vertexShaderID);
	
	
};*/
