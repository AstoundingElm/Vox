#pragma once

struct Face
{
	std::vector<GLfloat> verticies;
	std::vector<GLuint>  indicies = {
		0,1,3,
		3,1,2
	};
	std::vector<GLfloat> textureCoords = {
		0,0,
		0,1,
		1,1,
		1,0
	};
};

enum CubeFace
{
	TOP, BOTTOM, LEFT, RIGHT, BACK, FRONT
};

Face getCubeFace(CubeFace face)
{
	if (face == BACK)
		return Face {
		{
			0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,1.0f,0.0f
		},
	};
	else if (face == FRONT)
		return Face{
		{
			0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,
			1.0f,0.0f,1.0f,
			1.0f,1.0f,1.0f
		}
	};
	else if (face == RIGHT)
		return Face{
		{
			1.0f,1.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,0.0f,1.0f,
			1.0f,1.0f,1.0f
		}
	};
	else if (face == LEFT)
		return Face{
		{
			0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,
			0.0f,1.0f,1.0f
		}
	};
	else if (face == TOP)
		return Face{
		{
			0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,
			1.0f,1.0f,0.0f,
			1.0f,1.0f,1.0f
		}
	};
	else if (face == BOTTOM)
		return Face{
		{
			0.0f,0.0f,1.0f,
			0.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,0.0f,1.0f
		}
	};
}

std::vector<GLfloat> verticies = {
	// Back face
	0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,1.0f,0.0f,
	
	// Front face
	0.0f,1.0f,1.0f,
	0.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,1.0f,1.0f,
	
	// Right face
	1.0f,1.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,1.0f,
	1.0f,1.0f,1.0f,
	
	// Left Face
	0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,
	0.0f,1.0f,1.0f,
	
	// Top face
	0.0f,1.0f,1.0f,
	0.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,1.0f,
	
	// Bottom face
	0.0f,0.0f,1.0f,
	0.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,1.0f
};

std::vector<GLfloat> textureCoords = {
	0,0,
	0,1,
	1,1,
	1,0,
	
	0,0,
	0,1,
	1,1,
	1,0,
	
	0,0,
	0,1,
	1,1,
	1,0,
	
	0,0,
	0,1,
	1,1,
	1,0,
	
	0,0,
	0,1,
	1,1,
	1,0,
	
	0,0,
	0,1,
	1,1,
	1,0
};

std::vector<GLuint> indicies = {
	0,1,3,
	3,1,2,
	4,5,7,
	7,5,6,
	8,9,11,
	11,9,10,
	12,13,15,
	15,13,14,
	16,17,19,
	19,17,18,
	20,21,23,
	23,21,22
};

